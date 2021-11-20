#pragma once

#include <QString>

class Weather
{
public:
    explicit Weather(int id, const QString& name) : m_id(id), m_name(name) { }

    inline int getId() const { return m_id; }
    inline const QString& getName() const { return m_name; }
    void setName(const QString& name) { m_name = name; }

private:
    int m_id;
    QString m_name;
};
