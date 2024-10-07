#include <AP_Swivel/AP_SwivelControl.h>

extern const AP_HAL::HAL& hal;

const AP_Param::GroupInfo AP_SwivelControl::var_info[] = {
    // @Param: _ENABLE
    // @DisplayName: Swivel rate control enable/disable
    // @Description: Enable or disable swivel rate control
    // @Values: 0:Disabled,1:Enabled
    // @User: Standard
    AP_GROUPINFO_FLAGS("_ENABLE", 1, AP_SwivelControl, _enabled, 0, AP_PARAM_FLAG_ENABLE),

    // @Param: _WHL_BASE
    // @DisplayName: Swivel Steer Wheelbase
    // @Description: Distance between Swivel and vehicle's center of rotation
    // @Units: meters
    // @Range: 0 2.0
    // @User: Advanced
    AP_GROUPINFO("_WHL_BASE", 2, AP_SwivelControl, _wheelbase, AP_SWIVEL_WHEELBASE),

    // @Param: _WHL_DIST
    // @DisplayName: Swivel Steer Trackwidth
    // @Description: Distance between Swivel's wheels
    // @Units: meters
    // @Range: 0 1.0
    // @User: Advanced
    AP_GROUPINFO("_WHL_DIST", 3, AP_SwivelControl, _trackwidth, AP_SWIVEL_TRACKWIDTH),

    // @Param: _POS_P
    // @DisplayName: Swivel Steer Gain
    // @Description: Translates Swivel angle error into steering input for skid-steer mixer.
    // @Range: 0 1.0
    // @User: Advanced
    AP_GROUPINFO("_POS_P", 4, AP_SwivelControl, _pos_p, AP_SWIVEL_POS_CONTROL_P),

    // @Param: _PWM_MAX
    // @DisplayName: Swivel max steering output allowed for correcting angle
    // @Description: Swivel max steering output allowed for correcting angle
    // @Range: 0 1.0
    // @User: Advanced
    AP_GROUPINFO("_PWM_MAX", 5, AP_SwivelControl, _pwm_max, AP_SWIVEL_PWM_MAX_DEFAULT),

    // @Param: _RATE_MAX
    // @DisplayName: Swivel max rotation rate
    // @Description: Swivel max rotation rate
    // @Units: deg/s
    // @Range: 0 200
    // @User: Standard
    AP_GROUPINFO("_RATE_MAX", 6, AP_SwivelControl, _rate_max, AP_SWIVEL_RATE_MAX_DEFAULT),

    // @Param: _RATE_FF
    // @DisplayName: Swivel rate control feed forward gain
    // @Description: Swivel rate control feed forward gain.  Desired rate (in radians/sec) is multiplied by this constant and output to output (in the range -1 to +1)
    // @Range: 0.100 2.000
    // @User: Standard

    // @Param: _RATE_P
    // @DisplayName: Swivel rate control P gain
    // @Description: Swivel rate control P gain.  Converts rate error (in radians/sec) to output (in the range -1 to +1)
    // @Range: 0.100 2.000
    // @User: Standard

    // @Param: _RATE_I
    // @DisplayName: Swivel rate control I gain
    // @Description: Swivel rate control I gain.  Corrects long term error between the desired rate (in rad/s) and actual
    // @Range: 0.000 2.000
    // @User: Standard

    // @Param: _RATE_IMAX
    // @DisplayName: Swivel rate control I gain maximum
    // @Description: Swivel rate control I gain maximum.  Constrains the output (range -1 to +1) that the I term will generate
    // @Range: 0.000 1.000
    // @User: Standard

    // @Param: _RATE_D
    // @DisplayName: Swivel rate control D gain
    // @Description: Swivel rate control D gain.  Compensates for short-term change in desired rate vs actual
    // @Range: 0.000 0.400
    // @User: Standard

    // @Param: _RATE_FILT
    // @DisplayName: Swivel rate control filter frequency
    // @Description: Swivel rate control input filter.  Lower values reduce noise but add delay.
    // @Range: 1.000 100.000
    // @Units: Hz
    // @User: Standard

    // @Param: _RATE_FLTT
    // @DisplayName: Swivel rate control target frequency in Hz
    // @Description: Swivel rate control target frequency in Hz
    // @Range: 1 50
    // @Increment: 1
    // @Units: Hz
    // @User: Standard

    // @Param: _RATE_FLTE
    // @DisplayName: Swivel rate control error frequency in Hz
    // @Description: Swivel rate control error frequency in Hz
    // @Range: 1 50
    // @Increment: 1
    // @Units: Hz
    // @User: Standard

    // @Param: _RATE_FLTD
    // @DisplayName: Swivel rate control derivative frequency in Hz
    // @Description: Swivel rate control derivative frequency in Hz
    // @Range: 1 50
    // @Increment: 1
    // @Units: Hz
    // @User: Standard

    // @Param: _RATE_SMAX
    // @DisplayName: Swivel rate slew rate limit
    // @Description: Sets an upper limit on the slew rate produced by the combined P and D gains. If the amplitude of the control action produced by the rate feedback exceeds this value, then the D+P gain is reduced to respect the limit. This limits the amplitude of high frequency oscillations caused by an excessive gain. The limit should be set to no more than 25% of the actuators maximum slew rate to allow for load effects. Note: The gain will not be reduced to less than 10% of the nominal value. A value of zero will disable this feature.
    // @Range: 0 200
    // @Increment: 0.5
    // @User: Advanced

    // @Param: _RATE_PDMX
    // @DisplayName: Swivel rate control PD sum maximum
    // @Description: Swivel rate control PD sum maximum.  The maximum/minimum value that the sum of the P and D term can output
    // @Range: 0.000 1.000

    // @Param: _RATE_D_FF
    // @DisplayName: Swivel rate Derivative FeedForward Gain
    // @Description: FF D Gain which produces an output that is proportional to the rate of change of the error
    // @Range: 0.000 0.400
    // @Increment: 0.001
    // @User: Advanced

    // @Param: _RATE_NTF
    // @DisplayName: Swivel rate Target notch filter index
    // @Description: Swivel rate Target notch filter index
    // @Range: 1 8
    // @User: Advanced

    // @Param: _RATE_NEF
    // @DisplayName: Swivel rate Error notch filter index
    // @Description: Swivel rate Error notch filter index
    // @Range: 1 8
    // @User: Advanced

    AP_SUBGROUPINFO(_rate_pid, "_RATE_", 7, AP_SwivelControl, AC_PID),

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

// get steering output for correcting the swivel angle using rate control.
float AP_SwivelControl::get_swivel_position_correction(float target, float throttle, float dt)
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

    // get actual angel and rate from swivel
    float current_angle = 0;
    float actual_rate = 0; 
    _swivel.get_angle(current_angle);
    _swivel.get_rate(actual_rate);

    float swivel_error = target - current_angle;
    float desired_rate = swivel_error * _pos_p;

    desired_rate = constrain_float(degrees(desired_rate), -_rate_max, _rate_max);

    // constrain and set limit flags
    float output = _rate_pid.update_all(desired_rate, degrees(actual_rate), dt, (_limit.lower || _limit.upper));
    output += _rate_pid.get_ff();

    // Temporary fix for flipped output
    output *= -1;

    // Use current angle and throttle to determine torque vector
    float torque_vector = 0;
    if(!is_zero(current_angle)) {
        float turn_radius = _wheelbase / sinf(current_angle);
        float torque_ratio = _trackwidth * 0.5 / turn_radius;
        torque_vector = torque_ratio * throttle * 0.01f * 4500.0f;
    }
    output += torque_vector;

    // set limits for next iteration
    _limit.upper = output >= _pwm_max * 4500.0f;
    _limit.lower = output <= _pwm_max * -4500.0f;

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
