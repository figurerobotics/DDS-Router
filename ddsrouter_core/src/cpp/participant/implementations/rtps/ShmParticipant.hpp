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
 * @file ShmParticipant.hpp
 */

#ifndef __SRC_DDSROUTERCORE_PARTICIPANT_IMPLEMENTATIONS_RTPS_SHMPARTICIPANT_HPP_
#define __SRC_DDSROUTERCORE_PARTICIPANT_IMPLEMENTATIONS_RTPS_SHMPARTICIPANT_HPP_

#include <ddsrouter_core/configuration/participant/SimpleParticipantConfiguration.hpp>
#include <participant/implementations/rtps/CommonRTPSRouterParticipant.hpp>

namespace eprosima {
namespace ddsrouter {
namespace core {
namespace rtps {

/**
 * Participant with Simple Discovery Protocol.
 *
 * Standard RTPS Participant with Simple Discovery and default attributes.
 */
class ShmParticipant : public CommonRTPSRouterParticipant<configuration::SimpleParticipantConfiguration>
{
public:

    /**
     * @brief Construct a new Shm Participant object
     *
     * It uses the \c BaseParticipant constructor.
     * Apart from BaseParticipant, it creates a new RTPSParticipant with only SHM transport and domain given
     * by configuration.
     *
     * @throw \c InitializationException in case any internal error has ocurred while creating RTPSParticipant
     * @throw \c IConfigurationException in case configuration was incorrectly set
     */
    ShmParticipant(
            const configuration::SimpleParticipantConfiguration participant_configuration,
            std::shared_ptr<PayloadPool> payload_pool,
            std::shared_ptr<DiscoveryDatabase> discovery_database);

    /**
     * @brief Override the default RTPSParticipant attributes by using only SHM.
     *
     * @return participant attributes
     */
    fastrtps::rtps::RTPSParticipantAttributes participant_attributes_() const override;
};

} /* namespace rtps */
} /* namespace core */
} /* namespace ddsrouter */
} /* namespace eprosima */

#endif /* __SRC_DDSROUTERCORE_PARTICIPANT_IMPLEMENTATIONS_RTPS_SHMPARTICIPANT_HPP_ */
