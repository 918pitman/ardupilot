#pragma once

#include "AP_SWIVEL_config.h"

#if AP_SWIVEL_DRONECAN_ENABLED

#include "AP_SWIVEL_Backend.h"
#include <AP_DroneCAN/AP_DroneCAN.h>

class AP_SWIVEL_DroneCAN : public AP_SWIVEL_Backend
{
public:

    AP_SWIVEL_DroneCAN(AP_SWIVEL &_ap_swivel, AP_SWIVEL::SWIVEL_State &_state);

    static void subscribe_msgs(AP_DroneCAN* ap_dronecan);

    void update(void) override;

private:

    static void handle_angle(AP_DroneCAN *ap_dronecan, const CanardRxTransfer& transfer, const uavcan_equipment_actuator_Status &msg);

    float angle;
    float rate;
    uint32_t last_reading_ms;

    static AP_SWIVEL_DroneCAN *_driver;
    static HAL_Semaphore _driver_sem;

};

#endif // AP_SWIVEL_DRONECAN_ENABLED
