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

#ifndef STATISTICSHANDLER_H_
#define STATISTICSHANDLER_H_

#include <QFrame>

#include "dataelements.h"
#include "diagram.h"

class QComboBox;
class QLabel;

class StatisticsHandler : public QFrame
{
    Q_OBJECT
public:
    explicit StatisticsHandler(QWidget *parent = NULL);
    void fillHandler(QList<Shoe*> *shoes, QList<Sport*> *sports,
        QList<Exercise*> *exercises, QList<Place*> *places,
        QList<Weather*> *weathers);
    void clear();

public slots:
    void calculateStatistics(int index = -1);

private:
    QComboBox *m_filterSports;
    QComboBox *m_filterYears;
    QLabel *m_allDistanceLabel;
    QLabel *m_allTimeLabel;
    QLabel *m_allNumberOfTrainingsLabel;
    QLabel *m_avgTrainingDistanceLabel;
    QLabel *m_avgTrainingTimeLabel;
    QLabel *m_avgHRLabel;
    QLabel *m_avgCalBurntLabel;
    QLabel *m_avgFatBurntLabel;

    QList<Shoe*> *m_shoes;
    QList<Sport*> *m_sports;
    QList<Exercise*> *m_exercises;
    QList<Place*> *m_places;
    QList<Weather*> *m_weathers;

    Diagram *m_diagram;
};

#endif  // STATISTICSHANDLER_H_
