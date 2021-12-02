#pragma once

#include <QList>
#include <QStringList>
#include <QString>

#include "Exercise.h"
#include "Place.h"
#include "Shoe.h"
#include "Sport.h"
#include "Weather.h"

class DataHandler {
public:
    QString getShoeString(int id);
    QString getSportString(int id);
    QString getPlaceString(int id);
    QString getWeatherString(int id);
    QStringList getSportStrings();
    QStringList getShoeStrings();
    QStringList getPlaceStrings();
    QStringList getWeatherStrings();
    int getShoeId(const QString& name);
    int getSportId(const QString& name);
    int getPlaceId(const QString& name);
    int getWeatherId(const QString& name);
    Exercise *getExerciseById(int id);
    Sport *getSportById(int id);
    QList<Place *> getPlacesById(const QSet<int>& ids);
    QList<Weather *> getWeathersById(const QSet<int>& ids);
    void removeExerciseById(int id);
    void removeSportById(int id);
    void removeShoeById(int id);
    void removePlaceById(int id);
    void removeWeatherById(int id);

    QString encodeComment(const QString& comment);
    QString decodeComment(const QString& comment);

    void loadDatabase(const QString& fileName);
    bool saveDatabase(const QString& fileName);
    bool exportDatabaseAsHtml(const QString& fileName);

    const QString months[12] = {
        "Jan.",
        "Feb.",
        "Mar.",
        "Apr.",
        "May",
        "Jun.",
        "Jul.",
        "Aug.",
        "Sept.",
        "Oct.",
        "Nov.",
        "Dec."
    };

    QList<Shoe *> shoes;
    QList<Sport *> sports;
    QList<Place *> places;
    QList<Weather *> weathers;
    QList<Exercise *> exercises;
};
