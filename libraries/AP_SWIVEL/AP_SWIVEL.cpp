#include "AP_SWIVEL.h"

#if AP_SWIVEL_ENABLED

#include "AP_SWIVEL_Backend.h"
#include "AP_SWIVEL_Analog.h"
#include "AP_SWIVEL_DroneCAN.h"

extern const AP_HAL::HAL& hal;

const AP_Param::GroupInfo AP_SWIVEL::var_info[] = {
    // @Group: 1_
    // @Path: AP_SWIVEL_Params.cpp
    AP_SUBGROUPINFO(_params, "1_", 0, AP_SWIVEL, AP_SWIVEL_Params),

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

void AP_SWIVEL::init(void)
{
    switch (get_type()) {
        case Type::NONE:
            return;
#if AP_SWIVEL_PIN_ENABLED
        case Type::ANALOG:
            driver = new AP_SWIVEL_Analog(*this, state);
            break;
#endif
#if AP_SWIVEL_DRONECAN_ENABLED
        case Type::DRONECAN:
            driver = new AP_SWIVEL_DroneCAN(*this, state);
            break;
#endif
    }
}

void AP_SWIVEL::update(void)
{
    if (driver != nullptr) {
        driver->update();
    }
}

bool AP_SWIVEL::enabled() const
{
    return (get_type() != Type::NONE);
}

bool AP_SWIVEL::get_angle(float &angle_value) const
{
    if (!enabled()) {
        return false;
    }
    angle_value = state.angle;
    return true;
}

bool AP_SWIVEL::get_rate(float &rate_value) const
{
    if (!enabled()) {
        return false;
    }
    rate_value = state.rate;
    return true;
}

AP_SWIVEL *AP_SWIVEL::_singleton;

namespace AP {

AP_SWIVEL *swivel()
{
    return AP_SWIVEL::get_singleton();
}

}

#endif  // AP_SWIVEL_ENABLED
