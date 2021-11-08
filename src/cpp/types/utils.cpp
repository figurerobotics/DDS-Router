// Copyright 2021 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file configuration_tags.cpp
 *
 */

#include <set>

#if defined(_WIN32)
#include "Shlwapi.h"
#else
#include <fnmatch.h>
#endif // if defined(_WIN32)

#include <databroker/types/utils.hpp>
#include <databroker/types/configuration_tags.hpp>

namespace eprosima {
namespace databroker {

bool match_pattern(
        const std::string& pattern,
        const std::string& str)
{
#if defined(_WIN32)
    // Windows implementation
    return PathMatchSpec(str.c_str(), pattern.c_str());
#else
    // Posix implementation
    return (fnmatch(pattern.c_str(), str.c_str(), FNM_NOESCAPE) == 0);
#endif // defined(_WIN32)
}

} /* namespace databroker */
} /* namespace eprosima */
