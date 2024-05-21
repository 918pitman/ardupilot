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

    enum SWIVEL_Type {
        SWIVEL_TYPE_NONE     = 0,
        SWIVEL_TYPE_ANALOG   = 1,
        SWIVEL_TYPE_DRONECAN = 2,
    };

    struct SWIVEL_State {
        float                  angle;
        uint32_t               last_reading_ms;
    };

    AP_SWIVEL_Params _params;

    static const struct AP_Param::GroupInfo var_info[];

    void init(void);
    void update(void);
    bool get_angle(float &angle_value) const;
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
