#include <QFile>
#include <QtDebug>

#include "DataHandler.h"

QString DataHandler::getShoeString(int id)
{
    for (const auto& shoe : shoes) {
        if (shoe->getId() == id) {
            return shoe->getName();
        }
    }
    return "<unknown>";
}

QString DataHandler::getSportString(int id)
{
    for (const auto& sport : sports) {
        if (sport->getId() == id) {
            return sport->getName();
        }
    }
    return "<unknown>";
}

QString DataHandler::getPlaceString(int id)
{
    for (const auto& place : places) {
        if (place->getId() == id) {
            return place->getName();
        }
    }
    return "<unknown>";
}

QString DataHandler::getWeatherString(int id)
{
    for (const auto& weather : weathers) {
        if (weather->getId() == id) {
            return weather->getName();
        }
    }
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

bool DataHandler::exportData(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qWarning() << QObject::tr("Cannot write file: %1: %2").arg(fileName).arg(file.errorString());
        return false;
    }
    QTextStream out(&file);
    // TODO: Move these to member functions of the corresponding data classes.
    QStringList outData("{\n  \"shoes\": [\n");
    for (int i = 0; i < shoes.size(); ++i) {
        outData << QString("    { \"id\": %1").arg(i) <<
            QString(", \"name\": \"%1\"").arg(shoes[i]->getName()) <<
            QString(", \"buy\": %1").arg(shoes[i]->getBuy()) <<
            QString(", \"comment\": \"%1\" }").arg(shoes[i]->getComment());
        if (i != shoes.size() - 1) outData << ",";
        outData << "\n";
    }
    outData << "  ],\n  \"sports\": [\n";
    for (int i = 0; i < sports.size(); ++i) {
        outData << QString("    { \"id\": %1").arg(i);
        outData << QString(", \"name\": \"%1\"").arg(sports[i]->getName());
        outData << QString(", \"color\": \"%1\" }").arg((sports[i]->getColor()).name());
        if (i != sports.size() - 1) outData << ",";
        outData << "\n";
    }
    outData << "  ],\n  \"places\": [\n";
    for (int i = 0; i < places.size(); ++i) {
        outData << QString("    { \"id\": %1").arg(i) <<
            QString(", \"name\": \"%1\" }").arg(places[i]->getName());
        if (i != places.size() - 1) outData << ",";
        outData << "\n";
    }
    outData << "  ],\n  \"weathers\": [\n";
    for (int i = 0; i < weathers.size(); ++i) {
        outData << QString("    { \"id\": %1").arg(i) <<
            QString(", \"name\": \"%1\" }").arg(weathers[i]->getName());
        if (i != weathers.size() - 1) outData << ",";
        outData << "\n";
    }
    outData << "  ],\n  \"exercises\": [\n";
    for (int i = 0; i < exercises.size(); ++i) {
        Exercise *e = exercises[i];
        outData << QString("    { \"id\": %1").arg(i);
        outData << QString(", \"date\": \"%1\"").arg(e->getDate());
        outData << QString(", \"time\": \"%1\"").arg(e->getTime());
        outData << QString(", \"distance\": %1").arg(e->getDistance());
        outData << QString(", \"duration\": \"%1\"").arg(e->getDuration());
        outData << QString(", \"sport\": %1").arg(e->getSport());
        QStringList placeNumberList;
        const auto& placeList = e->getPlaces();
        for (const auto& place : placeList) placeNumberList << QString::number(place);
        outData << QString(", \"place\": \"%1\"").arg(placeNumberList.join(","));
        outData << QString(", \"shoe\": %1").arg(e->getShoe());
        outData << QString(", \"comment\": \"%1\"").arg(encodeComment(e->getComment()));
        QStringList weatherNumberList;
        const auto& weatherList = e->getWeathers();
        for (const auto& weather : weatherList) weatherNumberList << QString::number(weather);
        outData << QString(", \"weather\": \"%1\"").arg(weatherNumberList.join(","));
        outData << QString(", \"pulse\": \"%1/%2\"").arg(e->getMaxPulse()).arg(e->getAvgPulse());
        outData << QString(", \"calories\": \"%1/%2\" }").arg(e->getCal()).arg(e->getFat());
        if (i != exercises.size() - 1) outData << ",";
        outData << "\n";
    }
    outData << "  ]\n}\n";
    out << outData.join("");
    file.close();
    return true;
}
