#pragma once

#include "AP_SWIVEL.h"

#if AP_SWIVEL_ENABLED
class AP_SWIVEL_Backend
{
public:
    AP_SWIVEL_Backend(AP_SWIVEL &_ap_swivel, AP_SWIVEL::SWIVEL_State &_state);
    virtual ~AP_SWIVEL_Backend(void) {}
    virtual void update() = 0;

protected:
    int8_t get_pin(void) const { return ap_swivel.analog_pin.get(); }
    AP_SWIVEL &ap_swivel;
    AP_SWIVEL::SWIVEL_State &state;
};

#endif   // AP_SWIVEL_ENABLED
