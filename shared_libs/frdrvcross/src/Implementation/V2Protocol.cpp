//
// Created by megaxela on 05.04.17.
//

#include "Implementation/V2Protocol.h"

V2Protocol::V2Protocol()
{

}

V2Protocol::~V2Protocol()
{

}

ByteArray V2Protocol::wrapData(const ByteArray &b)
{
    return ByteArray();
}

ByteArray V2Protocol::proceedReceivedData(const ByteArray &raw)
{
    return ByteArray();
}

bool V2Protocol::needResponseAfterReceivedData() const
{
    return false;
}

ByteArray V2Protocol::responseAfterReceivedData()
{
    return ByteArray();
}

ByteArray V2Protocol::receiveDataFromInterface(InterfacePtr inter, uint32_t responseBytesExpected)
{
    return ByteArray();
}

Protocol::Error V2Protocol::prepareDeviceToWrite(InterfacePtr physicalInterface)
{
    return Protocol::Error::NoError;
}

bool V2Protocol::checkConnection(InterfacePtr physicalInterface)
{
    return false;
}
