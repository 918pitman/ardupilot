#include <AP_SWIVEL/AP_SWIVEL_Params.h>

// table of user settable parameters
const AP_Param::GroupInfo AP_SWIVEL_Params::var_info[] = {
    // @Param: TYPE
    // @DisplayName: SWIVEL type
    // @Description: What type of SWIVEL sensor is connected
    // @Values: 0:None,1:Analog,2:DroneCAN
    // @User: Standard
    AP_GROUPINFO_FLAGS("TYPE", 1, AP_SWIVEL_Params, type, 0, AP_PARAM_FLAG_ENABLE),

    // @Param: PIN
    // @DisplayName: Input pin number
    // @Description: Analog Pin to measure angle from
    // @Values: -1:Disabled,50:AUX1,51:AUX2,52:AUX3,53:AUX4,54:AUX5,55:AUX6
    // @User: Standard
    AP_GROUPINFO("PIN", 2, AP_SWIVEL_Params, pin, 8),

    // @Param: VOLT_MIN
    // @DisplayName: Input pin number
    // @Description: Analog Pin to measure angle from
    // @Units: V
    // @User: Standard
    AP_GROUPINFO("VOLT_MIN", 3, AP_SWIVEL_Params, volt_min, 0.0f),

    // @Param: VOLT_MAX
    // @DisplayName: Input pin number
    // @Description: Analog Pin to measure angle from
    // @Units: V
    // @User: Standard
    AP_GROUPINFO("VOLT_MAX", 4, AP_SWIVEL_Params, volt_max, 3.3f),

    AP_GROUPEND
};

AP_SWIVEL_Params::AP_SWIVEL_Params(void)
{
    AP_Param::setup_object_defaults(this, var_info);
}
