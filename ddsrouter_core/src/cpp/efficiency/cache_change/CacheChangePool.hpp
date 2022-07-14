// Copyright 2022 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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
 * @file PoolConfiguration.hpp
 */

#ifndef __SRC_DDSROUTERCORE_EFFICIENCY_CACHECHANGE_CACHACHANGEPOOL_HPP_
#define __SRC_DDSROUTERCORE_EFFICIENCY_CACHECHANGE_CACHACHANGEPOOL_HPP_

#include <vector>

#include <fastdds/rtps/history/IChangePool.h>

#include <efficiency/cache_change/CacheChangePoolConfiguration.hpp>

namespace eprosima {
namespace ddsrouter {
namespace core {

class CacheChangePool : public fastrtps::rtps::IChangePool // public ChangeChangePool TODO anton see if we can reuse some code from it
{
public:

    CacheChangePool(
            CacheChangePoolConfiguration configuration);

    ~CacheChangePool();

    virtual bool reserve_cache(
            fastrtps::rtps::CacheChange_t*& cache_change) override;

    virtual bool release_cache(
            fastrtps::rtps::CacheChange_t* cache_change) override;

protected:

    std::vector<fastrtps::rtps::CacheChange_t*> free_values_;

    unsigned int reserved_;

    unsigned int index_first_free_available_;

    CacheChangePoolConfiguration configuration_;
};

} /* namespace core */
} /* namespace ddsrouter */
} /* namespace eprosima */

#endif /* __SRC_DDSROUTERCORE_EFFICIENCY_CACHECHANGE_CACHACHANGEPOOL_HPP_ */