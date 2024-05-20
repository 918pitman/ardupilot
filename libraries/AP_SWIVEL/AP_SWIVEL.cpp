#include "AP_SWIVEL_config.h"

#if AP_SWIVEL_ENABLED

#include <AP_SWIVEL/AP_SWIVEL.h>
#include <GCS_MAVLink/GCS.h>
#include <RC_Channel/RC_Channel.h>

#include <utility>

extern const AP_HAL::HAL& hal;

#ifndef BOARD_SWIVEL_DEFAULT
#define BOARD_SWIVEL_DEFAULT 0
#endif

const AP_Param::GroupInfo AP_SWIVEL::var_info[] = {
    // @Param: TYPE
    // @DisplayName: SWIVEL Type
    // @Description: Swivel steer angle feedback type
    // @Values: 0:Disabled,1:AnalogPin,2:DroneCAN
    // @User: Standard
    AP_GROUPINFO_FLAGS("TYPE", 0, AP_SWIVEL, swivel_type,  BOARD_SWIVEL_DEFAULT, AP_PARAM_FLAG_ENABLE),

    // @Param: ANA_PIN
    // @DisplayName: Receiver SWIVEL sensing pin
    // @Description: Pin used to read the SWIVEL voltage
    // @Values: 8:V5 Nano,11:Pixracer,13:Pixhawk ADC4,14:Pixhawk ADC3,15:Pixhawk ADC6/Pixhawk2 ADC,50:AUX1,51:AUX2,52:AUX3,53:AUX4,54:AUX5,55:AUX6,103:Pixhawk SBUS
    // @User: Standard
    AP_GROUPINFO("PIN", 1, AP_SWIVEL, swivel_analog_pin,  8),

    AP_GROUPEND
};

// Public
// ------

// constructor
AP_SWIVEL::AP_SWIVEL()
{       
    AP_Param::setup_object_defaults(this, var_info);
    if (_singleton) {
        AP_HAL::panic("Too many SWIVEL sensors");
    }
    _singleton = this;
}

// destructor
AP_SWIVEL::~AP_SWIVEL(void){}

/*
 * Get the AP_SWIVEL singleton
 */
AP_SWIVEL *AP_SWIVEL::get_singleton()
{
    return _singleton;
}

// Initialize the swivel object and prepare it for use
void AP_SWIVEL::init()
{   
    swivel_analog_source = hal.analogin->channel(ANALOG_INPUT_NONE);    
}

void AP_SWIVEL::update(void)
{
    if (!swivel_analog_source || !swivel_analog_source->set_pin(swivel_analog_pin)) {
        _measurement = 0.0f;
    }
    _measurement = swivel_analog_source->voltage_average();
}

float AP_SWIVEL::get_angle()
{
    return _measurement;
}

AP_SWIVEL *AP_SWIVEL::_singleton = nullptr;

namespace AP {

AP_SWIVEL *swivel()
{
    return AP_SWIVEL::get_singleton();
}

};

#endif  // AP_SWIVEL_ENABLED
