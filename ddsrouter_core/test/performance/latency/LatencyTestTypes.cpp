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
 * @file LatencyTestTypes.cpp
 *
 */

#include "LatencyTestTypes.hpp"

#include <cstring>
#include <cstddef>

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;

const size_t LatencyType::overhead = offsetof(LatencyType, data) +
        SerializedPayload_t::representation_header_size;
const std::string LatencyDataType::type_name_ = "LatencyType";

bool LatencyDataType::compare_data(
        const LatencyType& lt1,
        const LatencyType& lt2) const
{
    if (lt1.seqnum != lt2.seqnum)
    {
        return false;
    }

    // bouncing time is ignored on comparisson

    return 0 == memcmp(lt1.data, lt2.data, buffer_size_);
}

void LatencyDataType::copy_data(
        const LatencyType& src,
        LatencyType& dst) const
{

    dst.seqnum = src.seqnum;
    dst.bounce = src.bounce;
    memcpy(dst.data, src.data, buffer_size_);
}

bool LatencyDataType::serialize(
        void* data,
        SerializedPayload_t* payload)
{
    static uint8_t encapsulation[4] = { 0x0, 0x1, 0x0, 0x0 };
    LatencyType* lt = (LatencyType*)data;

    auto ser_data = payload->data;
    memcpy(ser_data, encapsulation, SerializedPayload_t::representation_header_size);
    ser_data += SerializedPayload_t::representation_header_size;
    memcpy(ser_data, &lt->seqnum, sizeof(lt->seqnum));
    ser_data += sizeof(lt->seqnum);
    memcpy(ser_data, &lt->bounce, sizeof(lt->bounce));
    ser_data += sizeof(lt->bounce);
    memcpy(ser_data, lt->data, buffer_size_);
    payload->length = m_typeSize;
    return true;
}

bool LatencyDataType::deserialize(
        SerializedPayload_t* payload,
        void* data)
{
    // Payload members endianness matches local machine
    LatencyType* lt = (LatencyType*)data;
    auto ser_data = payload->data + SerializedPayload_t::representation_header_size;
    lt->seqnum = *reinterpret_cast<uint32_t*>(ser_data);
    ser_data += sizeof(lt->seqnum);
    lt->bounce = *reinterpret_cast<uint32_t*>(ser_data);
    ser_data += sizeof(lt->bounce);
    std::copy(ser_data, ser_data + buffer_size_, lt->data);
    return true;
}

std::function<uint32_t()> LatencyDataType::getSerializedSizeProvider(
        void*)
{
    uint32_t size = m_typeSize;
    return [size]() -> uint32_t
           {
               return size;
           };
}

void* LatencyDataType::createData()
{
    return (void*)new uint8_t[m_typeSize];
}

void LatencyDataType::deleteData(
        void* data)
{
    delete[] (uint8_t*)(data);
}

const std::string LatencyDataType::type_name()
{
    return type_name_;
}

const std::string TestCommandDataType::type_name_ = "TestCommandType";

bool TestCommandDataType::serialize(
        void* data,
        SerializedPayload_t* payload)
{
    TestCommandType* t = (TestCommandType*)data;
    memcpy(payload->data, &t->command_, sizeof(t->command_));
    payload->length = 4;
    return true;
}

bool TestCommandDataType::deserialize(
        SerializedPayload_t* payload,
        void* data)
{
    TestCommandType* t = (TestCommandType*)data;
    //	cout << "PAYLOAD LENGTH: "<<payload->length << endl;
    //	cout << "PAYLOAD FIRST BYTE: "<< (int)payload->data[0] << endl;
    memcpy(&t->command_, payload->data, sizeof(payload->length));
    //	cout << "COMMAND: "<<t->command<< endl;
    return true;
}

std::function<uint32_t()> TestCommandDataType::getSerializedSizeProvider(
        void*)
{
    return []() -> uint32_t
           {
               uint32_t size = 0;

               size = (uint32_t)sizeof(uint32_t);

               return size;
           };
}

void* TestCommandDataType::createData()
{

    return (void*)new TestCommandType();
}

void TestCommandDataType::deleteData(
        void* data)
{

    delete((TestCommandType*)data);
}

const std::string TestCommandDataType::type_name()
{
    return type_name_;
}