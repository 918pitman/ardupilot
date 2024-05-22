#pragma once

#include "AP_SWIVEL_config.h"

#if AP_SWIVEL_PIN_ENABLED

#include "AP_SWIVEL_Backend.h"

class AP_SWIVEL_Analog : public AP_SWIVEL_Backend
{
public:

    AP_SWIVEL_Analog(AP_SWIVEL &_ap_swivel, AP_SWIVEL::SWIVEL_State &_state);

    void update(void) override;

private:

    AP_HAL::AnalogSource *source;

    const float v_min = get_v_min();
    const float v_max = get_v_max();
    const float v_range = v_max - v_min;
    const float v_center = v_min + (v_range / 2);
    const float v_per_radian = v_range / M_PI;
};

#endif  // AP_SWIVEL_PIN_ENABLED
