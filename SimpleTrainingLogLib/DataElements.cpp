#include "DataElements.h"

QList<Shoe*> shoes;
QList<Sport*> sports;
QList<Exercise*> exercises;
QList<Place*> places;
QList<Weather*> weathers;

void Exercise::addPlace(int id)
{
    if (!m_places.contains(id)) m_places.append(id);
}

void Exercise::setPlaces(QList<int> ids)
{
    for (int i = 0; i < ids.size(); ++i)
        if (!m_places.contains(ids[i])) m_places.append(ids[i]);
}

void Exercise::addWeather(int id)
{
    if (!m_weathers.contains(id)) m_weathers.append(id);
}

void Exercise::setWeathers(QList<int> ids)
{
    for (int i = 0; i < ids.size(); ++i)
        if (!m_weathers.contains(ids[i])) m_weathers.append(ids[i]);
}

QString getSportString(int id)
{
    for (int i = 0; i < sports.size(); ++i)
        if (sports[i]->getId() == id) return sports[i]->getName();
    return "<unknown>";
}

int getSportId(const QString& name)
{
    for (int i = 0; i < sports.size(); ++i)
        if (name == sports[i]->getName()) return i;
    return -1;
}

QString getShoeString(int id)
{
    for (int i = 0; i < shoes.size(); ++i)
        if (shoes[i]->getId() == id) return shoes[i]->getName();
    return "<unknown>";
}

int getShoeId(const QString& name)
{
    for (int i = 0; i < shoes.size(); ++i)
        if (name == shoes[i]->getName()) return i;
    return -1;
}

QString getPlaceString(int id)
{
    for (int i = 0; i < places.size(); ++i)
        if (places[i]->getId() == id) return places[i]->getName();
    return "<unknown>";
}

int getPlaceId(const QString& name)
{
    for (int i = 0; i < places.size(); ++i)
        if (name == places[i]->getName()) return i;
    return -1;
}

QString getWeatherString(int id)
{
    for (int i = 0; i < weathers.size(); ++i)
        if (weathers[i]->getId() == id) return weathers[i]->getName();
    return "<unknown>";
}

int getWeatherId(const QString& name)
{
    for (int i = 0; i < weathers.size(); ++i)
        if (name == weathers[i]->getName()) return i;
    return -1;
}

Sport *getSportById(int id)
{
    for (int i = 0; i < sports.size(); ++i)
        if (sports[i]->getId() == id) return sports[i];
    return nullptr;
}

void removeSportById(int id)
{
    for (int i = 0; i < sports.size(); ++i)
        if (sports[i]->getId() == id) sports.removeAt(i);
}

QList<Place*> getPlacesById(QList<int> ids)
{
    QList<Place*> resultPlaces;
    for (int i = 0; i < places.size(); ++i)
        for (int j = 0; j < ids.size(); ++j)
            if (places[i]->getId() == ids[j])
                resultPlaces.append(places[i]);
    return resultPlaces;
}

void removePlaceById(int id)
{
    for (int i = 0; i < places.size(); ++i)
        if ((places[i])->getId() == id) places.removeAt(i);
}

Shoe *getShoeById(int id)
{
    for (int i = 0; i < shoes.size(); ++i)
        if ((shoes[i])->getId() == id) return shoes[i];
    return nullptr;
}

void removeShoeById(int id)
{
    for (int i = 0; i < shoes.size(); ++i)
        if (shoes[i]->getId() == id) shoes.removeAt(i);
}

QList<Weather*> getWeathersById(QList<int> ids)
{
    QList<Weather*> resultWeathers;
    for (int i = 0; i < weathers.size(); ++i)
        for (int j = 0; j < ids.size(); ++j)
            if (weathers[i]->getId() == ids[j])
                resultWeathers.append(weathers[i]);
    return resultWeathers;
}

void removeWeatherById(int id)
{
    for (int i = 0; i < weathers.size(); ++i)
        if (weathers[i]->getId() == id) weathers.removeAt(i);
}

void removeExerciseById(int id)
{
    for (int i = 0; i < exercises.size(); ++i)
        if (exercises[i]->getId() == id) exercises.removeAt(i);
}

Exercise *getExerciseById(int id)
{
    for (int i = 0; i < exercises.size(); ++i)
        if (exercises[i]->getId() == id) return exercises[i];
    return nullptr;
}

QStringList getSportStrings()
{
    QStringList sportList;
    for (int i = 0; i < sports.size(); ++i)
        sportList << sports[i]->getName();
    return sportList;
}

QStringList getShoeStrings()
{
    QStringList shoeList;
    for (int i = 0; i < shoes.size(); ++i)
        shoeList << shoes[i]->getName();
    return shoeList;
}

QStringList getPlaceStrings()
{
    QStringList placeList;
    for (int i = 0; i < places.size(); ++i)
        placeList << places[i]->getName();
    return placeList;
}

QStringList getWeatherStrings()
{
    QStringList weatherList;
    for (int i = 0; i < weathers.size(); ++i)
        weatherList << weathers[i]->getName();
    return weatherList;
}

QString encodeComment(const QString& comment)
{
  // Replace all '"' and '\n' characters.  '\n' characters are not supported
  // yet in comments.  TODO Handle more escape sequences.
  return QString(comment).replace('"', "\\\"").replace('\n', ' ');
}

QString decodeComment(const QString& comment)
{
  return QString(comment).replace("\\\"", "\"");
}
