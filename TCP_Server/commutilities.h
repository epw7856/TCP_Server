#ifndef COMMUTILITIES_H
#define COMMUTILITIES_H

#include <QByteArray>
#include <QDataStream>
#include <vector>

std::vector<unsigned> toUint32Vector(const QByteArray& byteArray);
QByteArray toByteArray(const std::vector<unsigned>& uint32Vector);
QByteArray toByteArray(const std::vector<unsigned>& uint32Vector, QDataStream::ByteOrder specifiedByteOrder);
std::vector<unsigned> reverseEndian(const std::vector<unsigned>& dataIn);

#endif // COMMUTILITIES_H
