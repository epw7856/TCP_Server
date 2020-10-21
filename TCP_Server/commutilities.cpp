#include "commutilities.h"

std::vector<unsigned> toUint32Vector(const QByteArray& byteArray)
{
    std::vector<unsigned> uint32Vector;
    unsigned size = static_cast<unsigned>(byteArray.size()) / sizeof (unsigned);
    uint32Vector.resize(size);
    QDataStream stream(byteArray);
    stream.setByteOrder(QDataStream::LittleEndian);

    for(unsigned i = 0; i < size; ++i)
    {
        stream >> uint32Vector[i];
    }

    return uint32Vector;
}

QByteArray toByteArray(const std::vector<unsigned>& uint32Vector)
{
    // Assumption: Little Endian input parameter
    return toByteArray(uint32Vector, QDataStream::LittleEndian);
}

QByteArray toByteArray(const std::vector<unsigned>& uint32Vector, QDataStream::ByteOrder specifiedByteOrder)
{
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream.setByteOrder(specifiedByteOrder);

    for(auto i : uint32Vector)
    {
        stream << i;
    }

    return byteArray;
}

std::vector<unsigned> reverseEndian(const std::vector<unsigned>& dataIn)
{
    QByteArray bytes = toByteArray(dataIn, QDataStream::BigEndian);
    return toUint32Vector(bytes);
}
