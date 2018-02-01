%module imgsrch
%{
#include "engine.h"
// #include <map>
// #include <string>
%}

%include "std_map.i"
%include "std_string.i"

namespace std {
    %template() map<string, float>;
}

%include "engine.h"
