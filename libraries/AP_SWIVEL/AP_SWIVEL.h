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

    bool update(void);

    bool get_angle(float &angle);

    // parameter block
    static const struct AP_Param::GroupInfo var_info[];

private:

    static AP_SWIVEL *_singleton;

    // SWIVEL parameters
    AP_Int8         rssi_analog_pin;                        // Analog pin SWIVEL value found on
    // Analog Inputs
    // a pin for reading the receiver SWIVEL voltage. 
    AP_HAL::AnalogSource *rssi_analog_source;
    float _measurement = 0.0;
};

namespace AP {
    AP_SWIVEL *swivel();
};

#endif  // AP_SWIVEL_ENABLED
