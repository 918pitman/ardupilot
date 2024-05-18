#include "AP_SWIVEL_config.h"

#if AP_SWIVEL_ENABLED

#include <AP_SWIVEL/AP_SWIVEL.h>
#include <GCS_MAVLink/GCS.h>
#include <RC_Channel/RC_Channel.h>

#include <utility>

extern const AP_HAL::HAL& hal;

#ifndef BOARD_SWIVEL_ANA_PIN
#define BOARD_SWIVEL_ANA_PIN 10
#endif

const AP_Param::GroupInfo AP_SWIVEL::var_info[] = {

    // @Param: ANA_PIN
    // @DisplayName: Receiver SWIVEL sensing pin
    // @Description: Pin used to read the SWIVEL voltage or PWM value
    // @User: Standard
    AP_GROUPINFO("ANA_PIN", 0, AP_SWIVEL, rssi_analog_pin,  BOARD_SWIVEL_ANA_PIN),

    AP_GROUPEND
};

// Public
// ------

// constructor
AP_SWIVEL::AP_SWIVEL()
{       
    AP_Param::setup_object_defaults(this, var_info);
    if (_singleton) {
        AP_HAL::panic("Too many Steer feedback sensors");
    }
    _singleton = this;
}

// destructor
AP_SWIVEL::~AP_SWIVEL(void)
{       
}

/*
 * Get the AP_SWIVEL singleton
 */
AP_SWIVEL *AP_SWIVEL::get_singleton()
{
    return _singleton;
}

// Initialize the rssi object and prepare it for use
void AP_SWIVEL::init()
{
    // a pin for reading the receiver SWIVEL voltage. The scaling by 0.25 
    // is to take the 0 to 1024 range down to an 8 bit range for MAVLink    
    rssi_analog_source = hal.analogin->channel(ANALOG_INPUT_NONE);    
}

// read the SWIVEL value from an analog pin - returns float in range 0.0 to 1.0
// 0.0 represents weakest signal, 1.0 represents maximum signal.
bool AP_SWIVEL::update()
{
    if (!rssi_analog_source || !rssi_analog_source->set_pin(rssi_analog_pin)) {
        return false;
    }
    _measurement = rssi_analog_source->read_latest();
    return true;
}

bool AP_SWIVEL::get_angle(float &angle)
{
    angle = _measurement;
    return true;
}

AP_SWIVEL *AP_SWIVEL::_singleton = nullptr;

namespace AP {

AP_SWIVEL *swivel()
{
    return AP_SWIVEL::get_singleton();
}

};

#endif  // AP_SWIVEL_ENABLED
