#include <AP_SWIVEL/AP_SWIVEL.h>

#if AP_SWIVEL_ENABLED

#include <GCS_MAVLink/GCS.h>
#include <RC_Channel/RC_Channel.h>
#include <utility>

extern const AP_HAL::HAL& hal;

AP_SWIVEL *AP_SWIVEL::_singleton;

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


// constructor
AP_SWIVEL::AP_SWIVEL(void)
{       
    AP_Param::setup_object_defaults(this, var_info);
    // if (_singleton != nullptr) {
    //     AP_HAL::panic("AP_SWIVEL must be singleton");
    // }
    _singleton = this;
}

void AP_SWIVEL::init(void)
{   
    rssi_analog_source = hal.analogin->channel(ANALOG_INPUT_NONE);    
}

void AP_SWIVEL::update(void)
{
    if (!rssi_analog_source || !rssi_analog_source->set_pin(rssi_analog_pin)) {
        _measurement = 0;
    }
    else {
        _measurement = rssi_analog_source->read_latest();
    }

}

bool AP_SWIVEL::get_angle(float &angle)
{
    angle = _measurement;
    return true;
}

namespace AP {

AP_SWIVEL &swivel()
{
    return *AP_SWIVEL::get_singleton();
}

}

#endif  // AP_SWIVEL_ENABLED
