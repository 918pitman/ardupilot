#include "Rover.h"

void ModeHold::update()
{
    float throttle = 0.0f;

    // if vehicle is balance bot, calculate actual throttle required for balancing
    if (rover.is_balancebot()) {
        rover.balancebot_pitch_control(throttle);
    }

    // relax mainsail
    g2.motors.set_mainsail(100.0f);
    g2.motors.set_wingsail(0.0f);

    // hold position - stop motors and center steering
    g2.motors.set_throttle(throttle);

    float steering = 0.0f;

    if (g2.motors.have_swivel_steering()) {
        steering = is_positive(g2.motors.get_swivel_actual()) ? 4500.0f : -4500.0f;
    }

    g2.motors.set_steering(steering, false);
}
