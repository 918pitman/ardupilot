#include "AP_SWIVEL_config.h"

#if AP_SWIVEL_PIN_ENABLED

#include "AP_SWIVEL_Analog.h"
#include <AP_HAL/AP_HAL.h>

extern const AP_HAL::HAL& hal;

AP_SWIVEL_Analog::AP_SWIVEL_Analog(AP_SWIVEL &_ap_swivel, AP_SWIVEL::SWIVEL_State &_state) :
    AP_SWIVEL_Backend(_ap_swivel, _state)
{
    source = hal.analogin->channel(ANALOG_INPUT_NONE);
}

void AP_SWIVEL_Analog::update(void)
{
    if (!source || !source->set_pin(get_pin())) {
        state.angle = 0;
    }
    float voltage = source->voltage_average();
    state.angle = (voltage - v_center) / v_per_radian;
    state.last_reading_ms = AP_HAL::millis();
}

#endif  // AP_SWIVEL_PIN_ENABLED
