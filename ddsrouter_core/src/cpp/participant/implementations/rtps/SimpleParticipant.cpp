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
 * @file SimpleRTPSRouterParticipant.cpp
 */

#include <memory>

#include <fastrtps/rtps/participant/RTPSParticipant.h>
#include <fastrtps/rtps/RTPSDomain.h>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>
#include <fastdds/rtps/transport/shared_mem/SharedMemTransportDescriptor.h>

#include <participant/implementations/rtps/SimpleParticipant.hpp>

namespace eprosima {
namespace ddsrouter {
namespace core {
namespace rtps {

using namespace eprosima::ddsrouter::core::types;

SimpleParticipant::SimpleParticipant(
        std::shared_ptr<configuration::SimpleParticipantConfiguration> participant_configuration,
        std::shared_ptr<PayloadPool> payload_pool,
        std::shared_ptr<DiscoveryDatabase> discovery_database)
    : CommonParticipant(
        participant_configuration,
        payload_pool,
        discovery_database,
        participant_configuration->domain,
        get_participant_attributes_(participant_configuration.get()))
{
}

fastrtps::rtps::RTPSParticipantAttributes
SimpleParticipant::get_participant_attributes_(
        const configuration::SimpleParticipantConfiguration* configuration)
{
    // Use default as base attributes
    fastrtps::rtps::RTPSParticipantAttributes params = CommonParticipant::get_participant_attributes_(configuration);

    if (!configuration->whitelist.empty())
    {
        params.useBuiltinTransports = false;

        std::shared_ptr<eprosima::fastdds::rtps::SharedMemTransportDescriptor> shm_transport =
                std::make_shared<eprosima::fastdds::rtps::SharedMemTransportDescriptor>();

        std::shared_ptr<eprosima::fastdds::rtps::UDPv4TransportDescriptor> udp_transport =
                std::make_shared<eprosima::fastdds::rtps::UDPv4TransportDescriptor>();


        for (const types::IpType& ip : configuration->whitelist)
        {
            if (types::Address::is_ipv4_correct(ip))
            {
                udp_transport->interfaceWhiteList.emplace_back(ip);
                logInfo(DDSPIPE_SIMPLE_PARTICIPANT,
                    "Adding " << ip << " to whitelist interfaces " <<
                    " in Participant " << configuration->id << " initialization.");
            }
            else
            {
                // Invalid address, continue with next one
                logWarning(DDSPIPE_SIMPLE_PARTICIPANT,
                        "Not valid IPv4. Discarding whitelist interface " << ip <<
                        " in Participant " << configuration->id << " initialization.");
            }
        }

        params.userTransports.push_back(shm_transport);
        params.userTransports.push_back(udp_transport);
    }


    return params;
}

} /* namespace rtps */
} /* namespace core */
} /* namespace ddsrouter */
} /* namespace eprosima */
