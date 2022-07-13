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
 * @file ServiceRegistry.hpp
 */

#ifndef _DDSROUTERCORE_TYPES_DDS_SERVICEREGISTRY_HPP_
#define _DDSROUTERCORE_TYPES_DDS_SERVICEREGISTRY_HPP_

#include <map>
#include <mutex>

#include <fastdds/rtps/common/SampleIdentity.h>

// #include <ddsrouter_core/library/library_dll.h>
#include <ddsrouter_core/types/dds/Guid.hpp>
#include <ddsrouter_core/types/participant/ParticipantId.hpp>
#include <ddsrouter_core/types/topic/RealTopic.hpp>

namespace eprosima {
namespace ddsrouter {
namespace core {
namespace types {

using SampleIdentity = eprosima::fastrtps::rtps::SampleIdentity;

using SequenceNumber = eprosima::fastrtps::rtps::SequenceNumber_t;

// TODO: move to different folder?
class ServiceRegistry
{
public:

    ServiceRegistry(
            const RealTopic& topic,
            const ParticipantId& server_participant_id);

    void set_related_sample_identity(
            const Guid& reply_reader_guid);

    SampleIdentity related_sample_identity();

    void add(
            SequenceNumber idx,
            std::pair<ParticipantId, SampleIdentity> new_entry);

    std::pair<ParticipantId, SampleIdentity> get(
            SequenceNumber idx);

    void erase(
            SequenceNumber idx);

    std::string service_name();

    RealTopic request_topic();

    RealTopic reply_topic();

    ParticipantId server_participant_id();

    // TODO: move these functions to utils or somewhere else??
    static bool is_request_topic(
            const RealTopic& topic);

    static bool is_reply_topic(
            const RealTopic& topic);

    static bool is_service_topic(
            const RealTopic& topic);

protected:

    std::string service_name_;

    RealTopic request_topic_;

    RealTopic reply_topic_;

    ParticipantId server_participant_id_;

    SampleIdentity related_sample_identity_;

    std::map<SequenceNumber, std::pair<ParticipantId, SampleIdentity>> registry_;

    std::mutex mutex_;

    static const std::string request_prefix_str;
    static const std::string reply_prefix_str;
    static const std::string request_str;
    static const std::string reply_str;
    static const std::string response_str;
};

} /* namespace types */
} /* namespace core */
} /* namespace ddsrouter */
} /* namespace eprosima */

#endif /* _DDSROUTERCORE_TYPES_DDS_SERVICEREGISTRY_HPP_ */
