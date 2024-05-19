#include "AP_Periph.h"

#ifdef HAL_PERIPH_ENABLE_SWIVEL

/*
  swivel support
 */

#include <dronecan_msgs.h>

void AP_Periph_FW::can_swivel_update()
{
    if (swivel.get_type() == AP_SWIVEL::Type::NONE) {
        return;
    }

    uint32_t now = AP_HAL::millis();
    static uint32_t last_update_ms;
    if (now - last_update_ms < 100) {
        return;
    }
    last_update_ms = now;
    swivel.update();
    AP_Proximity::Status status = proximity.get_status();
    if (status <= AP_Proximity::Status::NoData) {
        // don't send any data
        return;
    }

    ardupilot_equipment_feedback_SteerAngle pkt {};

    const uint8_t obstacle_count = proximity.get_obstacle_count();

    // if no objects return
    if (obstacle_count == 0) {
        return;
    }

    // calculate maximum roll, pitch values from objects
    for (uint8_t i=0; i<obstacle_count; i++) {
        if (!proximity.get_obstacle_info(i, pkt.yaw, pkt.pitch, pkt.distance)) {
            // not a valid obstacle
            continue;
        }

        pkt.sensor_id = proximity.get_address(0);

        switch (status) {
        case AP_Proximity::Status::NotConnected:
            pkt.reading_type = ARDUPILOT_EQUIPMENT_PROXIMITY_SENSOR_PROXIMITY_READING_TYPE_NOT_CONNECTED;
            break;
        case AP_Proximity::Status::Good:
            pkt.reading_type = ARDUPILOT_EQUIPMENT_PROXIMITY_SENSOR_PROXIMITY_READING_TYPE_GOOD;
            break;
        case AP_Proximity::Status::NoData:
        default:
            pkt.reading_type = ARDUPILOT_EQUIPMENT_PROXIMITY_SENSOR_PROXIMITY_READING_TYPE_NO_DATA;
            break;
        }

        uint8_t buffer[ARDUPILOT_EQUIPMENT_PROXIMITY_SENSOR_PROXIMITY_MAX_SIZE] {};
        uint16_t total_size = ardupilot_equipment_proximity_sensor_Proximity_encode(&pkt, buffer, !periph.canfdout());

        canard_broadcast(ARDUPILOT_EQUIPMENT_PROXIMITY_SENSOR_PROXIMITY_SIGNATURE,
                        ARDUPILOT_EQUIPMENT_PROXIMITY_SENSOR_PROXIMITY_ID,
                        CANARD_TRANSFER_PRIORITY_LOW,
                        &buffer[0],
                        total_size);

    }
}

#endif // HAL_PERIPH_ENABLE_PROXIMITY
