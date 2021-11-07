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

#include <QComboBox>
#include <QDate>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

#include "statisticshandler.h"
#include "ktrainer.h"

StatisticsHandler::StatisticsHandler(QWidget *parent) : QFrame(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    QGroupBox *filtersGroupBox = new QGroupBox(tr("Filters"));
    QHBoxLayout *filtersLayout = new QHBoxLayout;
    filtersLayout->addWidget(new QLabel("Sport:"));
    m_filterSports = new QComboBox;
    filtersLayout->addWidget(m_filterSports);
    filtersLayout->addWidget(new QLabel("Year:"));
    m_filterYears = new QComboBox;
    filtersLayout->addWidget(m_filterYears);
    filtersLayout->addStretch();
    filtersGroupBox->setLayout(filtersLayout);
    layout->addWidget(filtersGroupBox);

    QHBoxLayout *allNumberOfTrainingsLayout = new QHBoxLayout;
    allNumberOfTrainingsLayout->addWidget(new QLabel(tr("All # of trainings:")));
    allNumberOfTrainingsLayout->addWidget(m_allNumberOfTrainingsLabel = new QLabel);
    allNumberOfTrainingsLayout->addStretch();
    QHBoxLayout *allDistanceLayout = new QHBoxLayout;
    allDistanceLayout->addWidget(new QLabel(tr("All distance:")));
    allDistanceLayout->addWidget(m_allDistanceLabel = new QLabel);
    allDistanceLayout->addStretch();
    QHBoxLayout *allTimeLayout = new QHBoxLayout;
    allTimeLayout->addWidget(new QLabel(tr("All time:")));
    allTimeLayout->addWidget(m_allTimeLabel = new QLabel);
    allTimeLayout->addStretch();
    QHBoxLayout *avgTrainingDistanceLayout = new QHBoxLayout;
    avgTrainingDistanceLayout->addWidget(new QLabel(tr("Average training distance:")));
    avgTrainingDistanceLayout->addWidget(m_avgTrainingDistanceLabel = new QLabel);
    avgTrainingDistanceLayout->addStretch();
    QHBoxLayout *avgTrainingTimeLayout = new QHBoxLayout;
    avgTrainingTimeLayout->addWidget(new QLabel(tr("Average training time:")));
    avgTrainingTimeLayout->addWidget(m_avgTrainingTimeLabel = new QLabel);
    avgTrainingTimeLayout->addStretch();
    QHBoxLayout *avgHRLayout = new QHBoxLayout;
    avgHRLayout->addWidget(new QLabel(tr("Average HR:")));
    avgHRLayout->addWidget(m_avgHRLabel = new QLabel);
    avgHRLayout->addStretch();
    QHBoxLayout *avgCalBurntLayout = new QHBoxLayout;
    avgCalBurntLayout->addWidget(new QLabel(tr("Average calories burnt:")));
    avgCalBurntLayout->addWidget(m_avgCalBurntLabel = new QLabel);
    avgCalBurntLayout->addStretch();
    QHBoxLayout *avgFatBurntLayout = new QHBoxLayout;
    avgFatBurntLayout->addWidget(new QLabel(tr("Average fat % burnt:")));
    avgFatBurntLayout->addWidget(m_avgFatBurntLabel = new QLabel);
    avgFatBurntLayout->addStretch();

    layout->addLayout(allNumberOfTrainingsLayout);
    layout->addLayout(allDistanceLayout);
    layout->addLayout(allTimeLayout);
    layout->addLayout(avgTrainingDistanceLayout);
    layout->addLayout(avgTrainingTimeLayout);
    layout->addLayout(avgHRLayout);
    layout->addLayout(avgCalBurntLayout);
    layout->addLayout(avgFatBurntLayout);

    // The monthly bar-diagram.
    m_diagram = new Diagram();
    layout->addWidget(m_diagram);
    layout->addStretch();

    setLayout(layout);

    connect(m_filterSports, SIGNAL(currentIndexChanged(int)), this,
        SLOT(calculateStatistics(int)));
    connect(m_filterYears, SIGNAL(currentIndexChanged(int)), this,
        SLOT(calculateStatistics(int)));
}

void StatisticsHandler::fillHandler(QList<Shoe*> *shoes, QList<Sport*> *sports,
    QList<Exercise*> *exercises, QList<Place*> *places,
    QList<Weather*> *weathers)
{
    m_shoes = shoes;
    m_sports = sports;
    m_exercises = exercises;
    m_places = places;
    m_weathers = weathers;

    clear();

    for (int i = 0; i < m_sports->size(); ++i)
        m_filterSports->addItem(m_sports->at(i)->getName());
    m_filterSports->setFocus();
    QDate date = QDate::currentDate();
    for (int i = DATE_YEAR_START; i < date.year() + 1; ++i)
        m_filterYears->addItem(QString::number(i));
    emit m_filterYears->setCurrentIndex(date.year() - DATE_YEAR_START);
}

// To use it with setCurrentIndex(int) we need a parameter.  The filters are
// always set here.
void StatisticsHandler::calculateStatistics(int index __attribute__((unused)))
{
    int allDistance = 0;
    int allNumberOfTrainings = 0;
    int allTimeMinutes = 0;
    int hrMeasured = 0;
    double avgHR = 0.0;
    double avgCalBurnt = 0.0;
    double avgFatBurnt = 0.0;
    double avgTrainingDistance = 0.0;
    QString allTime = "0:00";
    QString avgTrainingTime = "0:00";
    int monthlyDistances[12];

    for (int i = 0; i < 12; ++i) monthlyDistances[i] = 0;

    int yearSelected = (m_filterYears->currentText()).toInt();
    int sportSelected = getSportId(m_filterSports->currentText());
    for (int i = 0; i < m_exercises->size(); ++i) {
        Exercise *e = m_exercises->at(i);
        if (e->getSport() != sportSelected ||
            (e->getDate()).split(DATE_SEPARATOR)[0].toInt() != yearSelected)
                continue;
        monthlyDistances[(e->getDate()).split(DATE_SEPARATOR)[1].toInt() - 1]
            += e->getDistance();
        allNumberOfTrainings++;
        allDistance += e->getDistance();
        QStringList timeList = (e->getDuration()).split(TIME_SEPARATOR);
        allTimeMinutes += timeList[0].toInt() * 60 + timeList[1].toInt();
        if (e->getAvgPulse() != 0) {
            avgHR += e->getAvgPulse();
            avgCalBurnt += e->getCal();
            avgFatBurnt += e->getFat();
            ++hrMeasured;
        }
    }
    if (allNumberOfTrainings > 0) {
        avgTrainingDistance = (double)allDistance / (double)allNumberOfTrainings;
        int avgMinutes = allTimeMinutes / allNumberOfTrainings;
        int avgHours = avgMinutes / 60;
        avgMinutes = avgMinutes % 60;
        QString formatString = avgMinutes < 10 ? QString("%1:0%2") :
            QString("%1:%2");
        avgTrainingTime = formatString.arg(avgHours).arg(avgMinutes);
    }

    if (hrMeasured > 0) {  // It's optional.
        avgHR /= (double)hrMeasured;
        avgCalBurnt /= (double)hrMeasured;
        avgFatBurnt /= (double)hrMeasured;
    }

    int hours = allTimeMinutes / 60;
    int minutes = allTimeMinutes = allTimeMinutes % 60;
    QString formatString = minutes < 10 ? QString("%1:0%2") : QString("%1:%2");
    allTime = formatString.arg(hours).arg(minutes);

    m_allDistanceLabel->setText(QString::number(allDistance));
    m_allNumberOfTrainingsLabel->setText(QString::number(allNumberOfTrainings));
    m_allTimeLabel->setText(allTime);
    m_avgTrainingDistanceLabel->setText(QString::number(avgTrainingDistance));
    m_avgTrainingTimeLabel->setText(avgTrainingTime);
    m_avgHRLabel->setText(QString::number(avgHR));
    m_avgCalBurntLabel->setText(QString::number(avgCalBurnt));
    m_avgFatBurntLabel->setText(QString::number(avgFatBurnt));

    QList<int> monthlyDistancesList;
    for (int i = 0; i < 12; ++i)
        monthlyDistancesList.append(monthlyDistances[i]);
    m_diagram->updateBars(monthlyDistancesList);
}

void StatisticsHandler::clear()
{
    m_filterSports->clear();
    m_filterYears->clear();
}

