#include "AP_SWIVEL_config.h"

#if AP_SWIVEL_DRONECAN_ENABLED

#include "AP_SWIVEL_DroneCAN.h"
#include <AP_BoardConfig/AP_BoardConfig.h>

AP_SWIVEL_DroneCAN* AP_SWIVEL_DroneCAN::_driver;
HAL_Semaphore AP_SWIVEL_DroneCAN::_driver_sem;

AP_SWIVEL_DroneCAN::AP_SWIVEL_DroneCAN(AP_SWIVEL &_ap_swivel, AP_SWIVEL::SWIVEL_State &_state) :
    AP_SWIVEL_Backend(_ap_swivel, _state)
{
    WITH_SEMAPHORE(_driver_sem);
    _driver = this;
}

void AP_SWIVEL_DroneCAN::subscribe_msgs(AP_DroneCAN* ap_dronecan)
{
    if (ap_dronecan == nullptr) {
        return;
    }

    if (Canard::allocate_sub_arg_callback(ap_dronecan, &handle_angle, ap_dronecan->get_driver_index()) == nullptr) {
        AP_BoardConfig::allocation_error("swivel_sub");
    }
}

// Receive new CAN message
void AP_SWIVEL_DroneCAN::handle_angle(AP_DroneCAN *ap_dronecan, const CanardRxTransfer& transfer, const uavcan_equipment_actuator_Status &msg)
{
    WITH_SEMAPHORE(_driver_sem);
    if (_driver == nullptr) {
        return;
    }
    _driver->last_reading_ms = AP_HAL::millis();
    _driver->angle = msg.position;
    _driver->rate = msg.speed;
}

void AP_SWIVEL_DroneCAN::update(void)
{
    WITH_SEMAPHORE(_driver_sem);
    state.last_reading_ms = last_reading_ms;
    state.angle = angle;
    state.rate = rate;
    if ((AP_HAL::millis() - state.last_reading_ms) > 1000) {
        state.angle = 0;
        state.rate = 0;
    }
}

#endif // AP_SWIVEL_DRONECAN_ENABLED
