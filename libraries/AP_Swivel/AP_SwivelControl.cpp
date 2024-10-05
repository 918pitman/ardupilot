#include <AP_Swivel/AP_SwivelControl.h>

extern const AP_HAL::HAL& hal;

const AP_Param::GroupInfo AP_SwivelControl::var_info[] = {
    // @Param: _ENABLE
    // @DisplayName: Wheel rate control enable/disable
    // @Description: Enable or disable swivel rate control
    // @Values: 0:Disabled,1:Enabled
    // @User: Standard
    AP_GROUPINFO_FLAGS("_ENABLE", 1, AP_SwivelControl, _enabled, 0, AP_PARAM_FLAG_ENABLE),

    // @Param: _RATE_MAX
    // @DisplayName: Wheel max rotation rate
    // @Description: Wheel max rotation rate
    // @Units: rad/s
    // @Range: 0 200
    // @User: Standard
    AP_GROUPINFO("_RATE_MAX", 2, AP_SwivelControl, _rate_max, AP_SWIVEL_RATE_MAX_DEFAULT),

    // @Param: _RATE_FF
    // @DisplayName: Wheel rate control feed forward gain
    // @Description: Wheel rate control feed forward gain.  Desired rate (in radians/sec) is multiplied by this constant and output to output (in the range -1 to +1)
    // @Range: 0.100 2.000
    // @User: Standard

    // @Param: _RATE_P
    // @DisplayName: Wheel rate control P gain
    // @Description: Wheel rate control P gain.  Converts rate error (in radians/sec) to output (in the range -1 to +1)
    // @Range: 0.100 2.000
    // @User: Standard

    // @Param: _RATE_I
    // @DisplayName: Wheel rate control I gain
    // @Description: Wheel rate control I gain.  Corrects long term error between the desired rate (in rad/s) and actual
    // @Range: 0.000 2.000
    // @User: Standard

    // @Param: _RATE_IMAX
    // @DisplayName: Wheel rate control I gain maximum
    // @Description: Wheel rate control I gain maximum.  Constrains the output (range -1 to +1) that the I term will generate
    // @Range: 0.000 1.000
    // @User: Standard

    // @Param: _RATE_D
    // @DisplayName: Wheel rate control D gain
    // @Description: Wheel rate control D gain.  Compensates for short-term change in desired rate vs actual
    // @Range: 0.000 0.400
    // @User: Standard

    // @Param: _RATE_FILT
    // @DisplayName: Wheel rate control filter frequency
    // @Description: Wheel rate control input filter.  Lower values reduce noise but add delay.
    // @Range: 1.000 100.000
    // @Units: Hz
    // @User: Standard

    // @Param: _RATE_FLTT
    // @DisplayName: Wheel rate control target frequency in Hz
    // @Description: Wheel rate control target frequency in Hz
    // @Range: 1 50
    // @Increment: 1
    // @Units: Hz
    // @User: Standard

    // @Param: _RATE_FLTE
    // @DisplayName: Wheel rate control error frequency in Hz
    // @Description: Wheel rate control error frequency in Hz
    // @Range: 1 50
    // @Increment: 1
    // @Units: Hz
    // @User: Standard

    // @Param: _RATE_FLTD
    // @DisplayName: Wheel rate control derivative frequency in Hz
    // @Description: Wheel rate control derivative frequency in Hz
    // @Range: 1 50
    // @Increment: 1
    // @Units: Hz
    // @User: Standard

    // @Param: _RATE_SMAX
    // @DisplayName: Wheel rate slew rate limit
    // @Description: Sets an upper limit on the slew rate produced by the combined P and D gains. If the amplitude of the control action produced by the rate feedback exceeds this value, then the D+P gain is reduced to respect the limit. This limits the amplitude of high frequency oscillations caused by an excessive gain. The limit should be set to no more than 25% of the actuators maximum slew rate to allow for load effects. Note: The gain will not be reduced to less than 10% of the nominal value. A value of zero will disable this feature.
    // @Range: 0 200
    // @Increment: 0.5
    // @User: Advanced

    // @Param: _RATE_PDMX
    // @DisplayName: Wheel rate control PD sum maximum
    // @Description: Wheel rate control PD sum maximum.  The maximum/minimum value that the sum of the P and D term can output
    // @Range: 0.000 1.000

    // @Param: _RATE_D_FF
    // @DisplayName: Wheel rate Derivative FeedForward Gain
    // @Description: FF D Gain which produces an output that is proportional to the rate of change of the error
    // @Range: 0.000 0.400
    // @Increment: 0.001
    // @User: Advanced

    // @Param: _RATE_NTF
    // @DisplayName: Wheel rate Target notch filter index
    // @Description: Wheel rate Target notch filter index
    // @Range: 1 8
    // @User: Advanced

    // @Param: _RATE_NEF
    // @DisplayName: Wheel rate Error notch filter index
    // @Description: Wheel rate Error notch filter index
    // @Range: 1 8
    // @User: Advanced

    AP_SUBGROUPINFO(_rate_pid, "_RATE_", 3, AP_SwivelControl, AC_PID)

    AP_GROUPEND
};

AP_SwivelControl::AP_SwivelControl(const AP_Swivel &swivel_ref) :
        _swivel(swivel_ref)
{
    AP_Param::setup_object_defaults(this, var_info);
}

// returns true if a swivel and rate control PID are available for this instance
bool AP_SwivelControl::enabled()
{
    // sanity check
    if (_enabled == 0) {
        return false;
    }
    // swivel enabled
    return _swivel.enabled();
}

// get output in the range -100 to +100 given a desired rate expressed as a percentage of the rate_max (-100 to +100)
float AP_SwivelControl::get_rate_controlled_throttle(float desired_rate_pct, float dt)
{
    if (!enabled()) {
        return 0;
    }

    // check for timeout
    uint32_t now = AP_HAL::millis();
    if (now - _last_update_ms > AP_SWIVEL_RATE_CONTROL_TIMEOUT_MS) {
        _rate_pid.reset_filter();
        _rate_pid.reset_I();
        _limit.lower = false;
        _limit.upper = false;
    }
    _last_update_ms = now;

    // convert desired rate as a percentage to radians/sec
    float desired_rate = desired_rate_pct * 0.01f * get_rate_max_rads();

    // get actual rate from swivel
    float actual_rate = 0; 
    _swivel.get_rate(actual_rate);

    // constrain and set limit flags
    float output = _rate_pid.update_all(desired_rate, actual_rate, dt, (_limit.lower || _limit.upper));
    output += _rate_pid.get_ff();

    // set limits for next iteration
    _limit.upper = output >= 100.0f;
    _limit.lower = output <= -100.0f;

    return output;
}

// get pid objects for reporting
AC_PID& AP_SwivelControl::get_pid() { return _rate_pid; }

void AP_SwivelControl::set_notch_sample_rate(float sample_rate)
{
#if AP_FILTER_ENABLED
    _rate_pid.set_notch_sample_rate(sample_rate);
#endif
}
