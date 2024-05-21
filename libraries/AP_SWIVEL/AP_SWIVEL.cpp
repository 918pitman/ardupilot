#include "AP_SWIVEL.h"

#if AP_SWIVEL_ENABLED

#include "AP_SWIVEL_Backend.h"
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
    switch (_params.type) {
        case SWIVEL_TYPE_NONE:
            return;
        case SWIVEL_TYPE_ANALOG:
            return;
        case SWIVEL_TYPE_DRONECAN:
            driver = new AP_SWIVEL_DroneCAN(*this, state);
            break;
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
    return (_params.type != SWIVEL_TYPE_NONE);
}

bool AP_SWIVEL::get_angle(float &angle_value) const
{
    if (!enabled()) {
        return false;
    }
    angle_value = state.angle;
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
