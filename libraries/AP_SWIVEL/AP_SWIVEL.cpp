#include "AP_SWIVEL.h"

#if AP_SWIVEL_ENABLED

#include "AP_SWIVEL_Backend.h"
#include "AP_SWIVEL_Analog.h"
#include "AP_SWIVEL_DroneCAN.h"

#ifndef BOARD_SWIVEL_DEFAULT
#define BOARD_SWIVEL_DEFAULT 0
#endif

const AP_Param::GroupInfo AP_SWIVEL::var_info[] = {
    // @Param: TYPE
    // @DisplayName: SWIVEL Type
    // @Description: Swivel steer angle feedback type
    // @Values: 0:Disabled,1:AnalogPin,2:DroneCAN
    // @User: Standard
    AP_GROUPINFO_FLAGS("TYPE", 0, AP_SWIVEL, swivel_type,  BOARD_SWIVEL_DEFAULT, AP_PARAM_FLAG_ENABLE),

    // @Param: ANA_PIN
    // @DisplayName: Receiver SWIVEL sensing pin
    // @Description: Pin used to read the SWIVEL voltage
    // @Values: 8:V5 Nano,11:Pixracer,13:Pixhawk ADC4,14:Pixhawk ADC3,15:Pixhawk ADC6/Pixhawk2 ADC,50:AUX1,51:AUX2,52:AUX3,53:AUX4,54:AUX5,55:AUX6,103:Pixhawk SBUS
    // @User: Standard
    AP_GROUPINFO("PIN", 1, AP_SWIVEL, analog_pin,  8),

    AP_GROUPEND
};

AP_SWIVEL::AP_SWIVEL(void)
{
    AP_Param::setup_object_defaults(this, var_info);
    if (_singleton != nullptr) {
        AP_HAL::panic("AP_SWIVEL must be singleton");
    }
    _singleton = this;
}

/*
  initialise the AP_SWIVEL class.
 */
void AP_SWIVEL::init(void)
{
    switch (Type(swivel_type.get())) {
    case Type::NONE:
        break;
    case Type::ANALOG:
        driver = new AP_SWIVEL_Analog(*this, state);
        break;
    case Type::DRONECAN:
        driver = new AP_SWIVEL_DroneCAN(*this, state);
        break;

    }
}

void AP_SWIVEL::update(void)
{
    WITH_SEMAPHORE(sem);
    if (driver != nullptr)
    {
        driver->update();
    }
}

bool AP_SWIVEL::healthy() const
{
    return (driver != nullptr);
}

bool AP_SWIVEL::enabled() const
{
    return (driver != nullptr);
}

bool AP_SWIVEL::get_angle(float &angle) const
{
    if (driver != nullptr)
    {
        angle = state.angle;
        return true;
    }
    return false;
}

AP_SWIVEL *AP_SWIVEL::_singleton;

namespace AP {

AP_SWIVEL *swivel()
{
    return AP_SWIVEL::get_singleton();
}

}

#endif  // AP_SWIVEL_ENABLED
