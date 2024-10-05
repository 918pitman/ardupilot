#pragma once

#include <AP_Common/AP_Common.h>
#include <AP_Math/AP_Math.h>
#include <AP_Param/AP_Param.h>
#include <AC_PID/AC_PID.h>
#include <AP_Swivel/AP_Swivel.h>

// swivel rate control defaults
#define AP_SWIVEL_RATE_MAX_DEFAULT   1.57f
#define AP_SWIVEL_RATE_CONTROL_FF    8.00f
#define AP_SWIVEL_RATE_CONTROL_P     2.00f
#define AP_SWIVEL_RATE_CONTROL_I     2.00f
#define AP_SWIVEL_RATE_CONTROL_IMAX  1.00f
#define AP_SWIVEL_RATE_CONTROL_D     0.01f
#define AP_SWIVEL_RATE_CONTROL_FILT  0.00f
#define AP_SWIVEL_RATE_CONTROL_DT    0.02f
#define AP_SWIVEL_RATE_CONTROL_TIMEOUT_MS 200

class AP_SwivelControl {

public:

    AP_SwivelControl(const AP_Swivel &swivel_ref);

    // returns true if a wheel encoder and rate control PID are available for this instance
    bool enabled();

    // get throttle output in the range -100 to +100 given a desired rate expressed as a percentage of the rate_max (-100 to +100)
    // instance can be 0 or 1
    float get_rate_controlled_swivel(float desired_rate_pct, float dt);

    // get rate maximum in radians/sec
    float get_rate_max_rads() const { return MAX(_rate_max, 0.0f); }

    // get pid objects for reporting
    AC_PID& get_pid();

    // set the PID notch sample rates
    void set_notch_sample_rate(float sample_rate);

    static const struct AP_Param::GroupInfo        var_info[];

private:

    // parameters
    AP_Int8         _enabled;   // top level enable/disable control
    AP_Float        _rate_max;  // wheel maximum rotation rate in rad/s
    AC_PID          _rate_pid{AP_SWIVEL_RATE_CONTROL_P, AP_SWIVEL_RATE_CONTROL_I, AP_SWIVEL_RATE_CONTROL_D, AP_SWIVEL_RATE_CONTROL_FF, AP_SWIVEL_RATE_CONTROL_IMAX, AP_SWIVEL_RATE_CONTROL_FILT, AP_SWIVEL_RATE_CONTROL_FILT, AP_SWIVEL_RATE_CONTROL_FILT, AP_SWIVEL_RATE_CONTROL_DT};

    // limit flags
    struct AP_MotorsUGV_limit {
        bool    lower;  // reached this instance's lower limit on last iteration
        bool    upper;  // reached this instance's upper limit on last iteration
    } _limit;

    // internal variables
    const AP_Swivel&        _swivel;     // pointer to accompanying wheel encoder
    uint32_t                _last_update_ms;    // system time of last call to get_rate_controlled_throttle
};
