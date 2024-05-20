#include "AP_SWIVEL_config.h"

#if AP_SWIVEL_PIN_ENABLED

#include "AP_SWIVEL_Analog.h"

extern const AP_HAL::HAL& hal;

AP_SWIVEL_Analog::AP_SWIVEL_Analog(AP_SWIVEL &_ap_swivel, AP_SWIVEL::SWIVEL_State &_state) :
    AP_SWIVEL_Backend(_ap_swivel, _state)
{
    source = hal.analogin->channel(ANALOG_INPUT_NONE);
}

void AP_SWIVEL_Analog::update(void)
{
    WITH_SEMAPHORE(sem);
    if (!source || !source->set_pin(get_pin())) {
        state.angle = 0;
    }
    // assume we get readings at at least 1Hz, otherwise reset quality to zero
    if (AP_HAL::millis() - state.last_reading_ms > 1000) {
        state.angle = 0;
    }
    state.angle = source->voltage_average();
}

#endif  // AP_SWIVEL_PIN_ENABLED
