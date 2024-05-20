#pragma once

#include "AP_SWIVEL_config.h"

#if AP_SWIVEL_ENABLED

#include <AP_Param/AP_Param.h>

class AP_SWIVEL_Backend;

class AP_SWIVEL
{
    friend class AP_SWIVEL_Backend;

public:
    AP_SWIVEL();

    CLASS_NO_COPY(AP_SWIVEL);  /* Do not allow copies */

    // SWIVEL driver types
    enum class Type {
        NONE     = 0,
        ANALOG   = 1,
        DRONECAN = 2,
    };

    // The SWIVEL_State structure is filled in by the backend driver
    struct SWIVEL_State {
        float                  angle;
        uint32_t               last_reading_ms;
    };

    Type get_type() const { return Type(swivel_type.get()); }

    static const struct AP_Param::GroupInfo var_info[];

    void init(void);

    void update(void);

    bool get_angle(float &angle) const;

    bool healthy() const;

    bool enabled() const;

    static AP_SWIVEL *get_singleton() { return _singleton; }

private:

    static AP_SWIVEL *_singleton;
    AP_Int8 swivel_type;
    AP_Int8 analog_pin;
    SWIVEL_State state;
    AP_SWIVEL_Backend *driver;
};

namespace AP {
    AP_SWIVEL *swivel();
};

#endif  // AP_SWIVEL_ENABLED
