/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "AP_SWIVEL_config.h"

#if AP_SWIVEL_ENABLED

#include <AP_SWIVEL/AP_SWIVEL.h>
#include <GCS_MAVLink/GCS.h>
#include <RC_Channel/RC_Channel.h>

#include <utility>

extern const AP_HAL::HAL& hal;

#ifndef BOARD_RSSI_DEFAULT
#define BOARD_RSSI_DEFAULT 0
#endif

const AP_Param::GroupInfo AP_SWIVEL::var_info[] = {

    // @Param: ANA_PIN
    // @DisplayName: Receiver SWIVEL sensing pin
    // @Description: Pin used to read the SWIVEL voltage
    // @Values: 8:V5 Nano,11:Pixracer,13:Pixhawk ADC4,14:Pixhawk ADC3,15:Pixhawk ADC6/Pixhawk2 ADC,50:AUX1,51:AUX2,52:AUX3,53:AUX4,54:AUX5,55:AUX6,103:Pixhawk SBUS
    // @User: Standard
    AP_GROUPINFO("ANA_PIN", 0, AP_SWIVEL, swivel_analog_pin,  8),

    AP_GROUPEND
};

// Public
// ------

// constructor
AP_SWIVEL::AP_SWIVEL()
{       
    AP_Param::setup_object_defaults(this, var_info);
    // if (_singleton) {
    //     AP_HAL::panic("Too many SWIVEL sensors");
    // }
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

// Initialize the swivel object and prepare it for use
void AP_SWIVEL::init()
{
    // a pin for reading the receiver SWIVEL voltage. The scaling by 0.25 
    // is to take the 0 to 1024 range down to an 8 bit range for MAVLink    
    swivel_analog_source = hal.analogin->channel(ANALOG_INPUT_NONE);    
}
void AP_SWIVEL::update(void)
{
    GCS_SEND_TEXT(MAV_SEVERITY_WARNING, "Measurement from Scheduler: %f", _measurement*3.3);
}
// Read the receiver SWIVEL value as a float 0.0f - 1.0f.
// 0.0 represents weakest signal, 1.0 represents maximum signal.
float AP_SWIVEL::read_receiver_swivel()
{
    if (!swivel_analog_source || !swivel_analog_source->set_pin(swivel_analog_pin)) {
        return 0;
    }
    float current_analog_voltage = swivel_analog_source->voltage_average();

    _measurement = scale_and_constrain_float_swivel(current_analog_voltage, 0.0f, 3.3f);
    return _measurement;
}

// Only valid for RECEIVER type SWIVEL selections. Returns -1 if protocol does not provide link quality report.
float AP_SWIVEL::read_receiver_link_quality()
{
    return -1;
}

// Read the receiver SWIVEL value as an 8-bit integer
// 0 represents weakest signal, 255 represents maximum signal.
uint8_t AP_SWIVEL::read_receiver_swivel_uint8()
{
    return read_receiver_swivel() * 255; 
}

// Scale and constrain a float swivel value to 0.0 to 1.0 range 
float AP_SWIVEL::scale_and_constrain_float_swivel(float current_swivel_value, float low_swivel_range, float high_swivel_range)
{    
    float swivel_value_range = fabsf(high_swivel_range - low_swivel_range);
    if (is_zero(swivel_value_range)) {
        // User range isn't meaningful, return 0 for SWIVEL (and avoid divide by zero)
        return 0.0f;   
    }
    // Note that user-supplied ranges may be inverted and we accommodate that here. 
    // (Some radio receivers put out inverted ranges for SWIVEL-type values).    
    bool range_is_inverted = (high_swivel_range < low_swivel_range);
    // Constrain to the possible range - values outside are clipped to ends 
    current_swivel_value = constrain_float(current_swivel_value, 
                                        range_is_inverted ? high_swivel_range : low_swivel_range, 
                                        range_is_inverted ? low_swivel_range : high_swivel_range);    

    if (range_is_inverted)
    {
        // Swap values so we can treat them as low->high uniformly in the code that follows
        current_swivel_value = high_swivel_range + fabsf(current_swivel_value - low_swivel_range);
        std::swap(low_swivel_range, high_swivel_range);        
    }

    // Scale the value down to a 0.0 - 1.0 range
    float swivel_value_scaled = (current_swivel_value - low_swivel_range) / swivel_value_range;
    // Make absolutely sure the value is clipped to the 0.0 - 1.0 range. This should handle things if the
    // value retrieved falls outside the user-supplied range.
    return constrain_float(swivel_value_scaled, 0.0f, 1.0f);
}

AP_SWIVEL *AP_SWIVEL::_singleton = nullptr;

namespace AP {

AP_SWIVEL *swivel()
{
    return AP_SWIVEL::get_singleton();
}

};

#endif  // AP_SWIVEL_ENABLED
