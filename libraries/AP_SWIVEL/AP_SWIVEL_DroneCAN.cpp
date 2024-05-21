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

// Subscribe to incoming swivel messages
void AP_SWIVEL_DroneCAN::subscribe_msgs(AP_DroneCAN* ap_dronecan)
{
    if (ap_dronecan == nullptr) {
        return;
    }

    if (Canard::allocate_sub_arg_callback(ap_dronecan, &handle_actuator, ap_dronecan->get_driver_index()) == nullptr) {
        AP_BoardConfig::allocation_error("actuator_sub");
    }
}

// Receive new CAN message
void AP_SWIVEL_DroneCAN::handle_actuator(AP_DroneCAN *ap_dronecan, const CanardRxTransfer& transfer, const uavcan_equipment_actuator_Status &msg)
{
    WITH_SEMAPHORE(_driver_sem);
    _driver->_last_reading_ms = AP_HAL::millis();
    _driver->_angle = msg.position;
}

void AP_SWIVEL_DroneCAN::update(void)
{
    WITH_SEMAPHORE(_driver_sem);
    state.last_reading_ms = _last_reading_ms;
    state.angle = _angle;
}

#endif // AP_SWIVEL_DRONECAN_ENABLED
