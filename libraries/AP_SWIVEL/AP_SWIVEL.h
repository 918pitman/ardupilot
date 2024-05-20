#pragma once

#include "AP_SWIVEL_config.h"

#if AP_SWIVEL_ENABLED

#include <AP_HAL/AP_HAL.h>
#include <AP_Param/AP_Param.h>
#include <AP_Math/AP_Math.h>

class AP_SWIVEL
{
public:
    enum class Type {
        NONE        = 0,
        ANALOG      = 1,
        DRONECAN    = 2,
    };
    AP_SWIVEL();

    /* Do not allow copies */
    CLASS_NO_COPY(AP_SWIVEL);

    // destructor
    ~AP_SWIVEL(void);

    static AP_SWIVEL *get_singleton();

    void init();
    void update(void);
    Type get_type() const { return Type(swivel_type.get()); }
    float get_angle();
    float _measurement;
    static const struct AP_Param::GroupInfo var_info[];

private:
    static AP_SWIVEL *_singleton;
    AP_Int8         swivel_type;
    AP_Int8         swivel_analog_pin;
    AP_HAL::AnalogSource *swivel_analog_source;
};

namespace AP {
    AP_SWIVEL *swivel();
};

#endif  // AP_SWIVEL_ENABLED
