#pragma once

#include <AP_Common/AP_Common.h>
#include <AP_Math/AP_Math.h>
#include <AP_Param/AP_Param.h>
#include <AC_PID/AC_PID.h>
#include <AP_Swivel/AP_Swivel.h>

// swivel rate control defaults
#define AP_SWIVEL_WHEELBASE          0.775f
#define AP_SWIVEL_TRACKWIDTH         0.40f
#define AP_SWIVEL_POS_CONTROL_P      1.00f
#define AP_SWIVEL_PWM_MAX_DEFAULT    0.50f
#define AP_SWIVEL_RATE_MAX_DEFAULT   90.0f
#define AP_SWIVEL_RATE_CONTROL_FF    6.00f
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

    // Returns true if SwivelControl is enabled
    bool enabled();

    // Returns the steering output required to achieve desired Swivel angle
    float get_swivel_position_correction(float target, float throttle, float dt);

    // get rate maximum in radians/sec
    float get_rate_max_rads() const { return MAX(_rate_max, 0.0f); }

    // get pid object for reporting
    AC_PID& get_pid();

    // set the PID notch sample rate
    void set_notch_sample_rate(float sample_rate);

    static const struct AP_Param::GroupInfo        var_info[];

private:

    // parameters
    AP_Int8         _enabled;    // Swivel rate control enable/disable
    AP_Float        _wheelbase;  // Distance between Swivel and vehicle's center of rotation
    AP_Float        _trackwidth; // Distance between Swivel's wheels
    AP_Float        _pos_p;      // Angle position error to angular rate gain
    AP_Float        _pwm_max;    // Swivel max steering output allowed for correcting angle
    AP_Float        _rate_max;   // Swivel max rotation rate
    AC_PID          _rate_pid{AP_SWIVEL_RATE_CONTROL_P, AP_SWIVEL_RATE_CONTROL_I, AP_SWIVEL_RATE_CONTROL_D, AP_SWIVEL_RATE_CONTROL_FF, AP_SWIVEL_RATE_CONTROL_IMAX, AP_SWIVEL_RATE_CONTROL_FILT, AP_SWIVEL_RATE_CONTROL_FILT, AP_SWIVEL_RATE_CONTROL_FILT, AP_SWIVEL_RATE_CONTROL_DT};

    // limit flags
    struct AP_MotorsUGV_limit {
        bool    lower;  // reached this instance's lower limit on last iteration
        bool    upper;  // reached this instance's upper limit on last iteration
    } _limit;

    // internal variables
    const AP_Swivel&        _swivel;     // pointer to accompanying swivel
    uint32_t                _last_update_ms;    // system time of last call to get_rate_controlled_throttle
};
