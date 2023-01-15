#include "debug_functions.hpp"

long long debug_millis(){
#ifdef DEBUG
    return clock();
#else
    return millis();
#endif
}