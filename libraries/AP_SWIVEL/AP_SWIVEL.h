#pragma once

#include <AP_HAL/AP_HAL_Boards.h>

#ifndef AP_SWIVEL_ENABLED
#define AP_SWIVEL_ENABLED 1
#endif

#if AP_SWIVEL_ENABLED

#include <AP_HAL/AP_HAL.h>
#include <AP_Param/AP_Param.h>
#include <AP_Math/AP_Math.h>

class AP_SWIVEL {
public:
    // constructor
    AP_SWIVEL(void);

    /* Do not allow copies */
    CLASS_NO_COPY(AP_SWIVEL);

    static const struct AP_Param::GroupInfo var_info[];

    // set analog source
    void init(void);

    // take a reading and update measurement
    void update(void);

    static AP_SWIVEL *get_singleton(void) {
        return _singleton;
    }

    // get measurement
    bool get_angle(float &angle);

private:

    static AP_SWIVEL *_singleton;

    AP_Int8 rssi_analog_pin;
    AP_HAL::AnalogSource *rssi_analog_source;
    float _measurement = 0.0;
};

namespace AP {
    AP_SWIVEL &swivel();
};

#endif  // AP_SWIVEL_ENABLED
