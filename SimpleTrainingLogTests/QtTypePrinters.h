#pragma once

#include <ostream>

#include <QString>

QT_BEGIN_NAMESPACE
inline void PrintTo(const QString &qString, ::std::ostream *os)
{
    *os << qString.toStdString();
}
QT_END_NAMESPACE
