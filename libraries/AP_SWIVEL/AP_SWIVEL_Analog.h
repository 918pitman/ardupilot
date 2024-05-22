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
    
    int8_t volt_pin        = ap_swivel._params.volt_pin.get();
    float  volt_min        = ap_swivel._params.volt_min.get();
    float  volt_max        = ap_swivel._params.volt_max.get();
    float  volt_range      = volt_max - volt_min;
    float  volt_center     = volt_min + (volt_range / 2);
    float  volt_per_radian = volt_range / M_PI;
};

#endif  // AP_SWIVEL_PIN_ENABLED
