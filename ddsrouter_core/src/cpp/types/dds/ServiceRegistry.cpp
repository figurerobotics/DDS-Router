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
 * @file ServiceRegistry.cpp
 *
 */

// #include <sstream>
#include <regex>
#include <string>

#include <ddsrouter_core/types/dds/ServiceRegistry.hpp>

namespace eprosima {
namespace ddsrouter {
namespace core {
namespace types {

const std::string ServiceRegistry::request_prefix_str = "rq/";
const std::string ServiceRegistry::reply_prefix_str = "rr/";
const std::string ServiceRegistry::request_str = "Request";
const std::string ServiceRegistry::reply_str = "Reply";
const std::string ServiceRegistry::response_str = "Response";

ServiceRegistry::ServiceRegistry(
            const RealTopic& topic,
            const ParticipantId& server_participant_id)
            : server_participant_id_(server_participant_id)
{
    // logDebug()

    if (is_request_topic(topic))
    {
        request_topic_ = topic;

        reply_topic_ = topic;
        reply_topic_.topic_name(std::regex_replace(reply_topic_.topic_name(), std::regex(request_prefix_str), reply_prefix_str));
        reply_topic_.topic_name(std::regex_replace(reply_topic_.topic_name(), std::regex(request_str), reply_str));
        reply_topic_.topic_type(std::regex_replace(reply_topic_.topic_type(), std::regex(request_str), response_str));

        service_name_ = std::regex_replace(topic.topic_name(), std::regex(request_prefix_str + "|" + request_str), "");
    }
    else if (is_reply_topic(topic))
    {
        reply_topic_ = topic;

        request_topic_ = topic;
        request_topic_.topic_name(std::regex_replace(request_topic_.topic_name(), std::regex(reply_prefix_str), request_prefix_str));
        request_topic_.topic_name(std::regex_replace(request_topic_.topic_name(), std::regex(reply_str), request_str));
        request_topic_.topic_type(std::regex_replace(request_topic_.topic_type(), std::regex(response_str), request_str));

        service_name_ = std::regex_replace(topic.topic_name(), std::regex(reply_prefix_str + "|" + reply_str), "");
    }
    else
    {
        // tsnh or throw exception
        std::cout << "Error" << std::endl;
    }
}

void ServiceRegistry::set_related_sample_identity(const Guid& reply_reader_guid)
{
    // std::lock_guard<std::mutex> lock(mutex_);  // not needed now as only set before bridges are enabled
    related_sample_identity_.writer_guid(reply_reader_guid);;
}

SampleIdentity ServiceRegistry::related_sample_identity()
{
    // std::lock_guard<std::mutex> lock(mutex_);
    return related_sample_identity_;
}

void ServiceRegistry::add(SequenceNumber idx, std::pair<ParticipantId, SampleIdentity> new_entry)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (registry_.count(idx))
    {
        // logError
        std::cout << "Error" << std::endl;
        return;
    }

    registry_[idx] = new_entry;
}

std::pair<ParticipantId, SampleIdentity> ServiceRegistry::get(SequenceNumber idx)
{
    std::lock_guard<std::mutex> lock(mutex_);

    std::pair<ParticipantId, SampleIdentity> ret;
    if (registry_.count(idx))
    {
        ret = registry_[idx];
    }
    else
    {
        // logError
        std::cout << "Error" << std::endl;
        ret = {ParticipantId(), SampleIdentity()};
    }

    return ret;
}

void ServiceRegistry::erase(SequenceNumber idx)
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (registry_.count(idx))
    {
        registry_.erase(idx);
    }
    else
    {
        // logError
        std::cout << "Error" << std::endl;
    }
}

std::string ServiceRegistry::service_name()
{
    return service_name_;
}

RealTopic ServiceRegistry::request_topic()
{
    return request_topic_;
}

RealTopic ServiceRegistry::reply_topic()
{
    return reply_topic_;
}

ParticipantId ServiceRegistry::server_participant_id()
{
    return server_participant_id_;
}

// TODO: move these functions to utils??
bool ServiceRegistry::is_request_topic(const RealTopic& topic)
{
    std::string topic_name = topic.topic_name();
    std::string topic_type = topic.topic_type();

    return (topic_name.find(request_prefix_str) == 0) && (topic_name.rfind(request_str) + request_str.length() == topic_name.length()) && (topic_type.rfind(request_str) + request_str.length() + 1 == topic_type.length());
}

bool ServiceRegistry::is_reply_topic(const RealTopic& topic)
{
    std::string topic_name = topic.topic_name();
    std::string topic_type = topic.topic_type();

    return (topic_name.find(reply_prefix_str) == 0) && (topic_name.rfind(reply_str) + reply_str.length() == topic_name.length()) && (topic_type.rfind(response_str) + response_str.length() + 1 == topic_type.length());
}

bool ServiceRegistry::is_service_topic(const RealTopic& topic)
{
    return is_request_topic(topic) || is_reply_topic(topic);
}

} /* namespace types */
} /* namespace core */
} /* namespace ddsrouter */
} /* namespace eprosima */
