#include "Exercise.h"

void Exercise::addPlace(int id)
{
    if (!m_places.contains(id)) {
        m_places.append(id);
    }
}

void Exercise::setPlaces(const QList<int>& ids)
{
    for (const auto& id : ids) {
        if (!m_places.contains(id)) {
            m_places.append(id);
        }
    }
}

void Exercise::addWeather(int id)
{
    if (!m_weathers.contains(id)) {
        m_weathers.append(id);
    }
}

void Exercise::setWeathers(const QList<int>& ids)
{
    for (const auto& id : ids) {
        if (!m_weathers.contains(id)) {
            m_weathers.append(id);
        }
    }
}
