%module imgsrch
%{
#include "imgsrch.h"
#include <map>
#include <set>
#include <string>
%}

%include "std_map.i"
%include "std_set.i"
%include "std_string.i"

namespace std {
    %template(map_scores) map<string, double>;
    %template(set_string) set<string>;
}

%include "imgsrch.h"
