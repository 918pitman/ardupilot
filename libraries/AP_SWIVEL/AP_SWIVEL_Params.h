#pragma once
#include <AP_Param/AP_Param.h>
#include "AP_SWIVEL_config.h"

class AP_SWIVEL_Params {

public:

    AP_SWIVEL_Params(void);

    AP_Int8  type;
    AP_Int8  volt_pin;
    AP_Float volt_min;
    AP_Float volt_max;

    static const struct AP_Param::GroupInfo var_info[];

};
