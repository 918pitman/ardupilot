#pragma once

#include "AP_SWIVEL.h"

#if AP_SWIVEL_ENABLED

class AP_SWIVEL_Backend
{
public:

    AP_SWIVEL_Backend(AP_SWIVEL &_ap_swivel, AP_SWIVEL::SWIVEL_State &_state);

    virtual ~AP_SWIVEL_Backend(void) {}

    virtual void update() = 0;

    int8_t get_pin(void) const { return ap_swivel._params.pin.get(); }
    int8_t get_volt_min(void) const { return ap_swivel._params.volt_min.get(); }
    int8_t get_volt_max(void) const { return ap_swivel._params.volt_max.get(); }

protected:

    AP_SWIVEL &ap_swivel;
    AP_SWIVEL::SWIVEL_State &state;
};

#endif   // AP_SWIVEL_ENABLED
