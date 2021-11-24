#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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

void DataHandler::loadDatabase(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot read file:" << fileName << ":" << file.errorString();
        return;
    }
    QString curLogAsString;
    curLogAsString = file.readAll();
    file.close();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(curLogAsString.toUtf8());
    if (jsonDocument.isNull()) {
        qWarning() << "Invalid JSON document:" << fileName;
        return;
    }

    QJsonObject jsonObject = jsonDocument.object();
    QJsonArray shoesFromJsonAsArray = jsonObject.value("shoes").toArray();
    QJsonArray sportsFromJsonAsArray = jsonObject.value("sports").toArray();
    QJsonArray placesFromJsonAsArray = jsonObject.value("places").toArray();
    QJsonArray weathersFromJsonAsArray = jsonObject.value("weathers").toArray();
    QJsonArray exercisesFromJsonAsArray = jsonObject.value("exercises").toArray();

    for (int i = 0; i < shoesFromJsonAsArray.size(); i++) {
        const auto& shoe = shoesFromJsonAsArray[i].toObject();
        const auto& shoeId = shoe["id"].toInt();
        if (shoeId < 0) {
            qWarning() << "Invalid shoe identifier:" << shoeId;
            continue;
        }
        const auto& shoeName = shoe["name"].toString();
        const auto& shoeBuy = shoe["buy"].toInt();
        const auto& shoeComment = shoe["comment"].toString();
        shoes.append(new Shoe(shoeId, shoeName, shoeBuy, shoeComment));
    }

    for (int i = 0; i < sportsFromJsonAsArray.size(); i++) {
        const auto& sport = sportsFromJsonAsArray[i].toObject();
        const auto& sportId = sport["id"].toInt();
        if (sportId < 0) {
            qWarning() << "Invalid sport identifier:" << sportId;
            continue;
        }
        const auto& sportName = sport["name"].toString();
        const auto& sportColor = sport["color"].toString();
        sports.append(new Sport(sportId, sportName, sportColor));
    }

    for (int i = 0; i < placesFromJsonAsArray.size(); i++) {
        const auto& place = placesFromJsonAsArray[i].toObject();
        const auto& placeId = place["id"].toInt();
        if (placeId < 0) {
            qWarning() << "Invalid place identifier:" << placeId;
            continue;
        }
        const auto& placeName = place["name"].toString();
        places.append(new Place(placeId, placeName));
    }

    for (int i = 0; i < weathersFromJsonAsArray.size(); i++) {
        const auto& weather = weathersFromJsonAsArray[i].toObject();
        const auto& weatherId = weather["id"].toInt();
        if (weatherId < 0) {
            qWarning() << "Invalid weather identifier:" << weatherId;
            continue;
        }
        const auto& weatherName = weather["name"].toString();
        weathers.append(new Weather(weatherId, weatherName));
    }

    for (int i = 0; i < exercisesFromJsonAsArray.size(); i++) {
        const auto& exercise = exercisesFromJsonAsArray[i].toObject();
        const auto& exerciseId = exercise["id"].toInt();
        if (exerciseId < 0) {
            qWarning() << "Invalid exercise identifier:" << exerciseId;
            continue;
        }
        const auto& exerciseDate = exercise["date"].toString();
        const auto& exerciseTime = exercise["time"].toString();
        const auto& exerciseDistance = exercise["distance"].toInt();
        const auto& exerciseDuration = exercise["duration"].toString();
        const auto& exerciseSport = exercise["sport"].toInt();
        const auto& exercisePlace = exercise["place"].toString().split(",");
        const auto& exerciseShoe = exercise["shoe"].toInt();
        const auto& exerciseComment = exercise["comment"].toString();
        const auto& exerciseWeather = exercise["weather"].toString().split(",");
        const auto& exercisePulse = exercise["pulse"].toString().split("/"); // MaxBpm/AvgBpm
        const auto& exerciseCalories = exercise["calories"].toString().split("/"); // Calories/Fat%
        Exercise *newExercise = new Exercise(exerciseId);
        newExercise->setDate(exerciseDate);
        newExercise->setTime(exerciseTime);
        newExercise->setDistance(exerciseDistance);
        newExercise->setDuration(exerciseDuration);
        newExercise->setSport(exerciseSport);
        QSet<int> places;
        for (const auto& place : exercisePlace) { places.insert(place.toInt()); }
        newExercise->addPlaces(places);
        newExercise->setShoe(exerciseShoe);
        newExercise->setComment(exerciseComment);
        QSet<int> weathers;
        for (const auto& weather : exerciseWeather) { weathers.insert(weather.toInt()); }
        newExercise->addWeathers(weathers);
        newExercise->setMaxPulse(exercisePulse[0].toInt());
        newExercise->setAvgPulse(exercisePulse[1].toInt());
        newExercise->setCal(exerciseCalories[0].toInt());
        newExercise->setFat(exerciseCalories[1].toInt());
        exercises.append(newExercise);
    }
}

bool DataHandler::saveDatabase(const QString& fileName)
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
