#include "Exercise.h"

void Exercise::addPlace(int id)
{
    m_places.insert(id);
}

void Exercise::addPlaces(const QSet<int>& newPlaceIds)
{
    m_places += newPlaceIds;
}

void Exercise::addWeather(int id)
{
    m_weathers.insert(id);
}

void Exercise::addWeathers(const QSet<int>& newWeatherIds)
{
    m_weathers += newWeatherIds;
}
