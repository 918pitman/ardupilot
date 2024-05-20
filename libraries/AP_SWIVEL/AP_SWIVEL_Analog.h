#pragma once

#include "AP_SWIVEL_config.h"

#if AP_SWIVEL_PIN_ENABLED

#include "AP_SWIVEL_Backend.h"

class AP_SWIVEL_Analog : public AP_SWIVEL_Backend
{
public:

    AP_SWIVEL_Analog(AP_SWIVEL &_ap_swivel, AP_SWIVEL::SWIVEL_State &_state);
    // update state
    void update(void) override;

private:
    AP_HAL::AnalogSource *source;
};

#endif  // AP_SWIVEL_PIN_ENABLED
