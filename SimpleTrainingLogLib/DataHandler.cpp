#include "DataHandler.h"

QString DataHandler::getShoeString(int id)
{
    for (int i = 0; i < shoes.size(); ++i)
        if (shoes[i]->getId() == id) return shoes[i]->getName();
    return "<unknown>";
}

QString DataHandler::getSportString(int id)
{
    for (int i = 0; i < sports.size(); ++i)
        if (sports[i]->getId() == id) return sports[i]->getName();
    return "<unknown>";
}

QString DataHandler::getPlaceString(int id)
{
    for (int i = 0; i < places.size(); ++i)
        if (places[i]->getId() == id) return places[i]->getName();
    return "<unknown>";
}

QString DataHandler::getWeatherString(int id)
{
    for (int i = 0; i < weathers.size(); ++i)
        if (weathers[i]->getId() == id) return weathers[i]->getName();
    return "<unknown>";
}

QStringList DataHandler::getSportStrings()
{
    QStringList sportList;
    for (int i = 0; i < sports.size(); ++i)
        sportList << sports[i]->getName();
    return sportList;
}

QStringList DataHandler::getShoeStrings()
{
    QStringList shoeList;
    for (int i = 0; i < shoes.size(); ++i)
        shoeList << shoes[i]->getName();
    return shoeList;
}

QStringList DataHandler::getPlaceStrings()
{
    QStringList placeList;
    for (int i = 0; i < places.size(); ++i)
        placeList << places[i]->getName();
    return placeList;
}

QStringList DataHandler::getWeatherStrings()
{
    QStringList weatherList;
    for (int i = 0; i < weathers.size(); ++i)
        weatherList << weathers[i]->getName();
    return weatherList;
}

int DataHandler::getShoeId(const QString& name)
{
    for (int i = 0; i < shoes.size(); ++i)
        if (name == shoes[i]->getName()) return i;
    return -1;
}

int DataHandler::getSportId(const QString& name)
{
    for (int i = 0; i < sports.size(); ++i)
        if (name == sports[i]->getName()) return i;
    return -1;
}

int DataHandler::getPlaceId(const QString& name)
{
    for (int i = 0; i < places.size(); ++i)
        if (name == places[i]->getName()) return i;
    return -1;
}

int DataHandler::getWeatherId(const QString& name)
{
    for (int i = 0; i < weathers.size(); ++i)
        if (name == weathers[i]->getName()) return i;
    return -1;
}

Exercise *DataHandler::getExerciseById(int id)
{
    for (int i = 0; i < exercises.size(); ++i)
        if (exercises[i]->getId() == id) return exercises[i];
    return nullptr;
}

Sport *DataHandler::getSportById(int id)
{
    for (int i = 0; i < sports.size(); ++i)
        if (sports[i]->getId() == id) return sports[i];
    return nullptr;
}

QList<Place *> DataHandler::getPlacesById(const QSet<int>& ids)
{
    QList<Place *> resultPlaces;
    for (int i = 0; i < places.size(); ++i) {
        for (const auto& id : ids) {
            if (places[i]->getId() == id) {
                resultPlaces.append(places[i]);
            }
        }
    }
    return resultPlaces;
}

QList<Weather *> DataHandler::getWeathersById(const QSet<int>& ids)
{
    QList<Weather *> resultWeathers;
    for (int i = 0; i < weathers.size(); ++i) {
        for (const auto& id : ids) {
            if (weathers[i]->getId() == id) {
                resultWeathers.append(weathers[i]);
            }
        }
    }
    return resultWeathers;
}

void DataHandler::removeExerciseById(int id)
{
    for (int i = 0; i < exercises.size(); ++i)
        if (exercises[i]->getId() == id) exercises.removeAt(i);
}

void DataHandler::removeSportById(int id)
{
    for (int i = 0; i < sports.size(); ++i)
        if (sports[i]->getId() == id) sports.removeAt(i);
}

void DataHandler::removeShoeById(int id)
{
    for (int i = 0; i < shoes.size(); ++i)
        if (shoes[i]->getId() == id) shoes.removeAt(i);
}

void DataHandler::removePlaceById(int id)
{
    for (int i = 0; i < places.size(); ++i)
        if ((places[i])->getId() == id) places.removeAt(i);
}

void DataHandler::removeWeatherById(int id)
{
    for (int i = 0; i < weathers.size(); ++i)
        if (weathers[i]->getId() == id) weathers.removeAt(i);
}

QString DataHandler::encodeComment(const QString& comment)
{
    // Replace all '"' and '\n' characters. '\n' characters are not supported
    // yet in comments. TODO: Handle more escape sequences.
    return QString(comment).replace('"', "\\\"").replace('\n', ' ');
}

QString DataHandler::decodeComment(const QString& comment)
{
    return QString(comment).replace("\\\"", "\"");
}
