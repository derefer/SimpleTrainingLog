#pragma once

#include <QTreeWidgetItem>

#include "dataelements.h"

class ExerciseItem : public QTreeWidgetItem
{
public:
    ExerciseItem(QTreeWidget *parent) : QTreeWidgetItem(parent) { }

private:
    bool operator<(const QTreeWidgetItem& other) const;
};

// It's a separate class to enable custom sorting of the elements of the
// table and it plays the role of a database handler.
class ExerciseTable : public QTreeWidget
{
    Q_OBJECT
    friend class ExerciseItem;

public:
    explicit ExerciseTable(QWidget *parent = NULL);
    QString encodeHTML() const;
    void appendTable(Exercise *exercise);
    void removeCurrentExercise();
    void removeExercise(int id);
    void updateExercise(int id);
    void fillTable(QList<Shoe*> *shoes, QList<Sport*> *sports,
        QList<Exercise*> *exercises, QList<Place*> *places,
        QList<Weather*> *weathers);
    void addExerciseData(const QStringList& exerciseData, const QColor& color);
    Exercise *getCurrentExercise() const;

private:
    int getIndexForExercise(int id) const;
    enum { COL_ID, COL_DATE, COL_TIME, COL_DISTANCE, COL_DURATION, COL_SPEED,
           COL_PULSE, COL_CALORIES, COL_PLACE, COL_WEATHER, COL_SHOE, COL_COMMENT };

    // Global pointers.
    QList<Sport*> *m_sports;
    QList<Place*> *m_places;
    QList<Shoe*> *m_shoes;
    QList<Exercise*> *m_exercises;
    QList<Weather*> *m_weathers;
};
