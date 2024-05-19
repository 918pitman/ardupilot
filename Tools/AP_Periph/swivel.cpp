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

    uavcan_equipment_actuator_Status pkt {};

    pkt.actuator_id = 0;
    pkt.position = swivel.get_angle();
    pkt.force = NAN;
    pkt.speed = NAN;
    pkt.power_rating_pct = 127;

    uint8_t buffer[UAVCAN_EQUIPMENT_ACTUATOR_STATUS_MAX_SIZE] {};
    uint16_t total_size = uavcan_equipment_actuator_Status_encode(&pkt, buffer, !periph.canfdout());

    canard_broadcast(UAVCAN_EQUIPMENT_ACTUATOR_STATUS_SIGNATURE,
                    UAVCAN_EQUIPMENT_ACTUATOR_STATUS_ID,
                    CANARD_TRANSFER_PRIORITY_LOW,
                    &buffer[0],
                    total_size);

    // ardupilot_equipment_feedback_SteerAngle pkt {};

    // pkt.steer_angle = swivel.get_angle();

    // uint8_t buffer[ARDUPILOT_EQUIPMENT_FEEDBACK_STEERANGLE_MAX_SIZE] {};
    // uint16_t total_size = ardupilot_equipment_feedback_SteerAngle_encode(&pkt, buffer, !periph.canfdout());

    // canard_broadcast(ARDUPILOT_EQUIPMENT_FEEDBACK_STEERANGLE_SIGNATURE,
    //                  ARDUPILOT_EQUIPMENT_FEEDBACK_STEERANGLE_ID,
    //                  CANARD_TRANSFER_PRIORITY_LOW,
    //                  &buffer[0],
    //                  total_size);
}


#endif // HAL_PERIPH_ENABLE_PROXIMITY
