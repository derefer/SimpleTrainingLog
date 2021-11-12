#pragma once

#include <QFrame>

#include "DataElements.h"
#include "Diagram.h"

class QComboBox;
class QLabel;

class StatisticsHandler : public QFrame
{
    Q_OBJECT
public:
    explicit StatisticsHandler(QWidget *parent = nullptr);
    void fillHandler(QList<Sport*> *sports, QList<Exercise*> *exercises);
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
