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
#pragma once

#include "AP_SWIVEL_config.h"

#if AP_SWIVEL_ENABLED

#include <AP_HAL/AP_HAL.h>
#include <AP_Param/AP_Param.h>
#include <AP_Math/AP_Math.h>

class AP_SWIVEL
{
public:
    AP_SWIVEL();

    /* Do not allow copies */
    CLASS_NO_COPY(AP_SWIVEL);

    // destructor
    ~AP_SWIVEL(void);

    static AP_SWIVEL *get_singleton();

    // Initialize the swivel object and prepare it for use
    void init();

    void update(void);
    // return true if swivel reading is enabled
    bool enabled() const { return true; }

    // Read the receiver SWIVEL value as a float 0.0f - 1.0f.
    // 0.0 represents weakest signal, 1.0 represents maximum signal.
    float read_receiver_swivel();
    float read_receiver_link_quality();
    // Read the receiver SWIVEL value as an 8-bit integer
    // 0 represents weakest signal, 255 represents maximum signal.
    uint8_t read_receiver_swivel_uint8();   
    float _measurement;
    // parameter block
    static const struct AP_Param::GroupInfo var_info[];

private:

    static AP_SWIVEL *_singleton;

    AP_Int8         swivel_analog_pin;                        // Analog pin SWIVEL value found on

    // Analog Inputs
    // a pin for reading the receiver SWIVEL voltage. 
    AP_HAL::AnalogSource *swivel_analog_source;

    // Scale and constrain a float swivel value to 0.0 to 1.0 range
    float scale_and_constrain_float_swivel(float current_swivel_value, float low_swivel_range, float high_swivel_range);
};

namespace AP {
    AP_SWIVEL *swivel();
};

#endif  // AP_SWIVEL_ENABLED
