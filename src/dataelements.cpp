/***************************************************************************
 *   Copyright (C) 2008, 2009 by Ferenc Kovács                             *
 *   derefer@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "dataelements.h"

extern QList<Shoe*> shoes;
extern QList<Sport*> sports;
extern QList<Exercise*> exercises;
extern QList<Place*> places;
extern QList<Weather*> weathers;

void Exercise::addPlace(int id)
{
    if (!m_places.contains(id))
        m_places.append(id);
}

void Exercise::setPlaces(QList<int> ids)
{
    for (int i = 0; i < ids.size(); ++i)
        if (!m_places.contains(ids.at(i)))
            m_places.append(ids.at(i));
}

void Exercise::addWeather(int id)
{
    if (!m_weathers.contains(id))
        m_weathers.append(id);
}

void Exercise::setWeathers(QList<int> ids)
{
    for (int i = 0; i < ids.size(); ++i)
        if (!m_weathers.contains(ids.at(i)))
            m_weathers.append(ids.at(i));
}

QString getSportString(int id)
{
    for (int i = 0; i < sports.size(); ++i)
        if ((sports.at(i))->getId() == id) return (sports.at(i))->getName();
    return "<unknown>";
}

int getSportId(const QString& name)
{
    for (int i = 0; i < sports.size(); ++i)
        if (name == (sports.at(i))->getName()) return i;
    return -1;
}

QString getShoeString(int id)
{
    for (int i = 0; i < shoes.size(); ++i)
        if ((shoes.at(i))->getId() == id) return (shoes.at(i))->getName();
    return "<unknown>";
}

int getShoeId(const QString& name)
{
    for (int i = 0; i < shoes.size(); ++i)
        if (name == (shoes.at(i))->getName()) return i;
    return -1;
}

QString getPlaceString(int id)
{
    for (int i = 0; i < places.size(); ++i)
        if ((places.at(i))->getId() == id) return (places.at(i))->getName();
    return "<unknown>";
}

int getPlaceId(const QString& name)
{
    for (int i = 0; i < places.size(); ++i)
        if (name == (places.at(i))->getName()) return i;
    return -1;
}

QString getWeatherString(int id)
{
    for (int i = 0; i < weathers.size(); ++i)
        if ((weathers.at(i))->getId() == id) return (weathers.at(i))->getName();
    return "<unknown>";
}

int getWeatherId(const QString& name)
{
    for (int i = 0; i < weathers.size(); ++i)
        if (name == (weathers.at(i))->getName()) return i;
    return -1;
}

Sport *getSportById(int id)
{
    for (int i = 0; i < sports.size(); ++i)
        if ((sports.at(i))->getId() == id) return sports.at(i);
    return NULL;
}

void removeSportById(int id)
{
    for (int i = 0; i < sports.size(); ++i)
        if ((sports.at(i))->getId() == id) sports.removeAt(i);
}

QList<Place*> getPlacesById(QList<int> ids)
{
    QList<Place*> resultPlaces;
    for (int i = 0; i < places.size(); ++i)
        for (int j = 0; j < ids.size(); ++j)
            if ((places.at(i))->getId() == ids.at(j))
                resultPlaces.append(places.at(i));
    return resultPlaces;
}

void removePlaceById(int id)
{
    for (int i = 0; i < places.size(); ++i)
        if ((places.at(i))->getId() == id) places.removeAt(i);
}

Shoe *getShoeById(int id)
{
    for (int i = 0; i < shoes.size(); ++i)
        if ((shoes.at(i))->getId() == id) return shoes.at(i);
    return NULL;
}

void removeShoeById(int id)
{
    for (int i = 0; i < shoes.size(); ++i)
        if ((shoes.at(i))->getId() == id) shoes.removeAt(i);
}

QList<Weather*> getWeathersById(QList<int> ids)
{
    QList<Weather*> resultWeathers;
    for (int i = 0; i < weathers.size(); ++i)
        for (int j = 0; j < ids.size(); ++j)
            if ((weathers.at(i))->getId() == ids.at(j))
                resultWeathers.append(weathers.at(i));
    return resultWeathers;
}

void removeWeatherById(int id)
{
    for (int i = 0; i < weathers.size(); ++i)
        if ((weathers.at(i))->getId() == id) weathers.removeAt(i);
}

void removeExerciseById(int id)
{
    for (int i = 0; i < exercises.size(); ++i)
        if ((exercises.at(i))->getId() == id) exercises.removeAt(i);
}

Exercise *getExerciseById(int id)
{
    for (int i = 0; i < exercises.size(); ++i)
        if ((exercises.at(i))->getId() == id) return exercises.at(i);
    return NULL;
}

QStringList getSportStrings()
{
    QStringList sportList;
    for (int i = 0; i < sports.size(); ++i)
        sportList << (sports.at(i))->getName();
    return sportList;
}

QStringList getShoeStrings()
{
    QStringList shoeList;
    for (int i = 0; i < shoes.size(); ++i)
        shoeList << (shoes.at(i))->getName();
    return shoeList;
}

QStringList getPlaceStrings()
{
    QStringList placeList;
    for (int i = 0; i < places.size(); ++i)
        placeList << (places.at(i))->getName();
    return placeList;
}

QStringList getWeatherStrings()
{
    QStringList weatherList;
    for (int i = 0; i < weathers.size(); ++i)
        weatherList << (weathers.at(i))->getName();
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
