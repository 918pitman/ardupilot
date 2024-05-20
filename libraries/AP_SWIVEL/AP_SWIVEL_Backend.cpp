#include "AP_SWIVEL_Backend.h"

#if AP_SWIVEL_ENABLED

#include "AP_SWIVEL.h"

AP_SWIVEL_Backend::AP_SWIVEL_Backend(AP_SWIVEL &_ap_swivel, AP_SWIVEL::SWIVEL_State &_state) :
    ap_swivel(_ap_swivel),
    state(_state) 
{}

#endif  // AP_SWIVEL_ENABLED
