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

#pragma once

#include <sstream>

#include <ddspipe_core/types/dds/DomainId.hpp>
#include <ddspipe_core/types/dds/GuidPrefix.hpp>
#include <ddspipe_core/types/participant/ParticipantId.hpp>

#include <ddspipe_participants/types/address/Address.hpp>

#include <ddspipe_yaml/Yaml.hpp>
#include <ddspipe_yaml/yaml_configuration_tags.hpp>

namespace eprosima {
namespace ddspipe {
namespace yaml {
namespace test {

template <typename T>
struct YamlField
{
    YamlField()
        : present(false)
    {
    }

    YamlField(
            T arg_value)
        : value(arg_value)
        , present(true)
    {
    }

    T value;
    bool present;
};

template <typename T>
void add_field_to_yaml(
        Yaml& yml,
        const YamlField<T>& field,
        const std::string& tag)
{
    if (field.present)
    {
        yml[tag] = field.value;
    }
}

void guid_prefix_to_yaml(
        Yaml& yml,
        const core::types::GuidPrefix& guid_prefix)
{
    std::stringstream ss;
    ss << guid_prefix;

    test::add_field_to_yaml(
        yml,
        test::YamlField<std::string>(ss.str()),
        DISCOVERY_SERVER_GUID_TAG);
}

void discovery_server_guid_prefix_to_yaml(
        Yaml& yml,
        const core::types::GuidPrefix& guid_prefix)
{
    Yaml yml_guid;
    guid_prefix_to_yaml(yml_guid, guid_prefix);

    yml[DISCOVERY_SERVER_GUID_PREFIX_TAG] = yml_guid;
}

void address_to_yaml(
        Yaml& yml,
        const participants::types::Address& address)
{
    test::add_field_to_yaml(
        yml,
        test::YamlField<participants::types::IpType>(address.ip()),
        ADDRESS_IP_TAG);

    test::add_field_to_yaml(
        yml,
        test::YamlField<participants::types::PortType>(address.port()),
        ADDRESS_PORT_TAG);

    if (address.transport_protocol() == participants::types::TransportProtocol::udp)
    {
        test::add_field_to_yaml(
            yml,
            test::YamlField<std::string>(ADDRESS_TRANSPORT_UDP_TAG),
            ADDRESS_TRANSPORT_TAG);
    }
    else if (address.transport_protocol() == participants::types::TransportProtocol::tcp)
    {
        test::add_field_to_yaml(
            yml,
            test::YamlField<std::string>(ADDRESS_TRANSPORT_TCP_TAG),
            ADDRESS_TRANSPORT_TAG);
    }

    if (address.ip_version() == participants::types::IpVersion::v4)
    {
        test::add_field_to_yaml(
            yml,
            test::YamlField<std::string>(ADDRESS_IP_VERSION_V4_TAG),
            ADDRESS_IP_VERSION_TAG);
    }
    else if (address.ip_version() == participants::types::IpVersion::v6)
    {
        test::add_field_to_yaml(
            yml,
            test::YamlField<std::string>(ADDRESS_IP_VERSION_V6_TAG),
            ADDRESS_IP_VERSION_TAG);
    }
}

void participantid_to_yaml(
        Yaml& yml,
        const core::types::ParticipantId& id)
{
    test::add_field_to_yaml(
        yml,
        test::YamlField<std::string>(id),
        PARTICIPANT_NAME_TAG);
}

void domain_to_yaml(
        Yaml& yml,
        const core::types::DomainId& domain)
{
    test::add_field_to_yaml(
        yml,
        test::YamlField<core::types::DomainIdType>(domain.domain_id()),
        DOMAIN_ID_TAG);
}

void repeater_to_yaml(
        Yaml& yml,
        const bool& repeater)
{
    test::add_field_to_yaml(
        yml,
        test::YamlField<bool>(repeater),
        IS_REPEATER_TAG);
}

} /* namespace test */
} /* namespace yaml */
} /* namespace ddspipe */
} /* namespace eprosima */
