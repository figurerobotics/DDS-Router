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
 * @file ReplyWriter.hpp
 */

#ifndef __SRC_DDSROUTERCORE_WRITER_IMPLEMENTATIONS_RTPS_REPLYWRITER_HPP_
#define __SRC_DDSROUTERCORE_WRITER_IMPLEMENTATIONS_RTPS_REPLYWRITER_HPP_

// #include <fastdds/rtps/rtps_fwd.h>
// #include <fastrtps/rtps/attributes/HistoryAttributes.h>
// #include <fastrtps/attributes/TopicAttributes.h>
// #include <fastrtps/qos/WriterQos.h>
// #include <fastrtps/rtps/history/WriterHistory.h>
// #include <fastrtps/rtps/attributes/WriterAttributes.h>
// #include <fastrtps/rtps/writer/RTPSWriter.h>
// #include <fastrtps/rtps/writer/WriterListener.h>

// #include <ddsrouter_core/types/participant/ParticipantId.hpp>
#include <ddsrouter_core/types/dds/ServiceRegistry.hpp>

#include <writer/implementations/rtps/Writer.hpp>

namespace eprosima {
namespace ddsrouter {
namespace core {
namespace rtps {

class ReplyWriter : public Writer
{
public:

    ReplyWriter(
            const types::ParticipantId& participant_id,
            const types::RealTopic& topic,
            std::shared_ptr<PayloadPool> payload_pool,
            fastrtps::rtps::RTPSParticipant* rtps_participant,
            std::shared_ptr<types::ServiceRegistry> service_registry);

    virtual ~ReplyWriter();

protected:

    /**
     * @brief Write specific method
     *
     * Store new data as message to send (asynchronously) (it could use PayloadPool to not copy payload).
     * Take next Untaken Change.
     * Set \c data with the message taken (data payload must be stored from PayloadPool).
     * Remove this change from Reader History and release.
     *
     * It does not require mutex, it will be guarded by RTPS Writer mutex in internal methods.
     *
     * @param data : oldest data to take
     * @return \c RETCODE_OK if data has been correctly taken
     * @return \c RETCODE_NO_DATA if \c data_to_send_ is empty
     * @return \c RETCODE_NO_DATA if \c data_to_send_ is empty
     */
    virtual utils::ReturnCode write_(
            std::unique_ptr<types::DataReceived>& data) noexcept override;

    std::shared_ptr<types::ServiceRegistry> service_registry_;
};

} /* namespace rtps */
} /* namespace core */
} /* namespace ddsrouter */
} /* namespace eprosima */

#endif /* __SRC_DDSROUTERCORE_WRITER_IMPLEMENTATIONS_RTPS_REPLYWRITER_HPP_ */
