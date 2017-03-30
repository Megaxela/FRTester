//
// Created by megaxela on 13.12.16.
//

#include "Tools/ByteArrayReader.h"

ByteArrayReader::ByteArrayReader(const ByteArray &reader) :
    m_pos(0),
    m_reader(reader)
{

}

void ByteArrayReader::seek(uint32_t pos)
{
    m_pos = pos;
}

void ByteArrayReader::move(int val)
{
    m_pos += val;
}

uint64_t ByteArrayReader::readPart(uint8_t size, ByteArray::ByteOrder order)
{
    auto result = m_reader.readPart(m_pos, size, order);

    m_pos += size;

    return result;
}

std::string ByteArrayReader::readString(uint32_t size)
{
    assert(m_reader.length() > m_pos + size);

    std::string result = std::string((const char*) m_reader.data() + m_pos, size);

    m_pos += size;

    return result;
}
