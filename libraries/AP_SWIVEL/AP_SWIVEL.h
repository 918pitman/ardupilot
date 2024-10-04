#pragma once

#include "AP_SWIVEL_config.h"

#if AP_SWIVEL_ENABLED

#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL_Boards.h>
#include <AP_Param/AP_Param.h>
#include <AP_Math/AP_Math.h>
#include "AP_SWIVEL_Params.h"

class AP_SWIVEL_Backend;

class AP_SWIVEL
{
    friend class AP_SWIVEL_Backend;

public:

    AP_SWIVEL();

    CLASS_NO_COPY(AP_SWIVEL);

    enum class Type {
        NONE     = 0,
#if AP_SWIVEL_PIN_ENABLED
        ANALOG   = 1,
#endif
#if AP_SWIVEL_DRONECAN_ENABLED
        DRONECAN = 2,
#endif
    };

    struct SWIVEL_State {
        float                  angle;
        float                  rate;
        uint32_t               last_reading_ms;
    };

    AP_SWIVEL_Params _params;

    static const struct AP_Param::GroupInfo var_info[];

    void init(void);
    void update(void);
    Type get_type() const { return (Type)((uint8_t)_params.type); }
    bool get_angle(float &angle_value) const;
    bool get_rate(float &rate_value) const;
    bool enabled() const;

    static AP_SWIVEL *get_singleton() { return _singleton; }

    int8_t get_dronecan_sensor_id() const;

private:

    static AP_SWIVEL *_singleton;
    SWIVEL_State state;
    AP_SWIVEL_Backend *driver;
};

namespace AP {
    AP_SWIVEL *swivel();
};

#endif  // AP_SWIVEL_ENABLED
