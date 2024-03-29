#include "DataHandler.h"
#include "ExerciseTable.h"
#include "SimpleTrainingLogMainWindow.h"

ExerciseTable::ExerciseTable(QWidget *parent, DataHandler *dataHandler) : QTreeWidget(parent), dataHandler(dataHandler)
{
    setSortingEnabled(true);
    QStringList list;
    list << "#" << "Date" << "Time" << "Distance" << "Duration" << "Speed" << "HR (M/A)"
        << "Calories (C/F)" << "Place" << "Weather" << "Shoe" << "Comment";
    setHeaderLabels(list);

    // It's not the best way to do this, but resizeColumnToContents() neither.
    setColumnWidth(COL_ID, 55);
    setColumnWidth(COL_DATE, 90);
    setColumnWidth(COL_TIME, 70);
    setColumnWidth(COL_DISTANCE, 70);
    setColumnWidth(COL_DURATION, 70);
    setColumnWidth(COL_SPEED, 70);
    setColumnWidth(COL_PULSE, 70);
    setColumnWidth(COL_CALORIES, 100);
    setColumnWidth(COL_PLACE, 100);
    setColumnWidth(COL_WEATHER, 90);
    setColumnWidth(COL_SHOE, 100);
    setColumnWidth(COL_COMMENT, 150);
}

// Add the given element to the table. The other elements of the table should stay untouched.
void ExerciseTable::appendTable(Exercise *exercise)
{
    QStringList exerciseData;
    exerciseData << QString::number(exercise->getId()) << exercise->getDate()
        << exercise->getTime() << QString::number(exercise->getDistance())
        << exercise->getDuration() << QString::number(exercise->getSpeed(), 'f', 2)
        << QString("%1/%2").arg(exercise->getMaxPulse()).arg(exercise->getAvgPulse())
        << QString("%1/%2").arg(exercise->getCal()).arg(exercise->getFat());
    const auto& placeIds = exercise->getPlaces();
    QStringList placeData;
    for (const auto& placeId : placeIds) {
        if (!placeData.empty()) {
            placeData << ", ";
        }
        placeData << dataHandler->getPlaceString(placeId);
    }
    exerciseData << placeData.join("");
    const auto& weatherIds = exercise->getWeathers();
    QStringList weatherData;
    for (const auto& weatherId : weatherIds) {
        if (!weatherData.empty()) {
            weatherData << ", ";
        }
        weatherData << dataHandler->getWeatherString(weatherId);
    }
    exerciseData << weatherData.join("");
    exerciseData << dataHandler->getShoeString(exercise->getShoe());
    exerciseData << exercise->getComment();

    addExerciseData(exerciseData, dataHandler->getSportById(exercise->getSport())->getColor());
}

// Initial fill of the table.
void ExerciseTable::fillTable(QList<Exercise *> *exercises)
{
    m_exercises = exercises;

    // Clean-up first.
    for (int i = 0; i < topLevelItemCount(); ++i)
        delete takeTopLevelItem(i);

    for (int i = 0; i < exercises->size(); ++i)
        appendTable(exercises->at(i));
}

void ExerciseTable::addExerciseData(const QStringList& exerciseData, const QColor& color)
{
    // TODO: The current implementation depends on the order of the columns.
    // Implement better sorting algorithms. For some reason I have problems
    // with the UTF-8 data read from the file. It's sorted by identifiers.
    ExerciseItem *item = new ExerciseItem(this);
    item->setData(COL_ID, Qt::DisplayRole, exerciseData.at(COL_ID).toInt());
    item->setText(COL_DATE, exerciseData.at(COL_DATE));
    item->setText(COL_TIME, exerciseData.at(COL_TIME));
    item->setData(COL_DISTANCE, Qt::DisplayRole,
        exerciseData.at(COL_DISTANCE).toInt());
    item->setText(COL_DURATION, exerciseData.at(COL_DURATION));
    item->setData(COL_SPEED, Qt::DisplayRole,
                  exerciseData.at(COL_SPEED).split(" ")[0].toDouble());
    item->setText(COL_PULSE, exerciseData.at(COL_PULSE));
    item->setText(COL_CALORIES, exerciseData.at(COL_CALORIES));
    item->setText(COL_PLACE, exerciseData.at(COL_PLACE));
    item->setText(COL_WEATHER, exerciseData.at(COL_WEATHER));
    item->setText(COL_SHOE, exerciseData.at(COL_SHOE));
    item->setText(COL_COMMENT, exerciseData.at(COL_COMMENT));

    for (int col = 0; col < item->columnCount(); ++col) {
        item->setBackground(col, QBrush(color));
    }
}

void ExerciseTable::removeCurrentExercise()
{
    int index = indexOfTopLevelItem(currentItem());
    m_exercises->removeAt(getIndexForExercise((topLevelItem(index)->
        data(COL_ID, Qt::DisplayRole)).toInt()));
    delete takeTopLevelItem(index);
}

void ExerciseTable::removeExercise(int id)
{
    for (int i = 0; i < topLevelItemCount(); ++i) {
        if (id == topLevelItem(i)->data(COL_ID, Qt::DisplayRole).toInt()) {
            delete takeTopLevelItem(i);
        }
    }
}

void ExerciseTable::updateExercise(int id)
{
    QTreeWidgetItem *item = nullptr;
    for (int i = 0; i < topLevelItemCount(); ++i) {
        if ((topLevelItem(i)->data(COL_ID, Qt::DisplayRole)).toInt() == id) {
            item = topLevelItem(i);
        }
    }
    if (!item) {
        return;
    }
    Exercise *e = dataHandler->getExerciseById(id);
    item->setData(COL_ID, Qt::DisplayRole, e->getId());
    item->setText(COL_DATE, e->getDate());
    item->setText(COL_TIME, e->getTime());
    item->setData(COL_DISTANCE, Qt::DisplayRole, e->getDistance());
    item->setText(COL_DURATION, e->getDuration());
    item->setData(COL_SPEED, Qt::DisplayRole, e->getSpeed());
    item->setText(COL_PULSE, QString("%1/%2").arg(e->
        getMaxPulse()).arg(e->getAvgPulse()));
    item->setText(COL_CALORIES, QString("%1/%2").arg(e->
        getCal()).arg(e->getFat()));
    QStringList placeStrings;
    const auto& placeIds = e->getPlaces();
    for (const auto& placeId : placeIds) {
        placeStrings << dataHandler->getPlaceString(placeId);
    }
    item->setText(COL_PLACE, placeStrings.join(", "));
    QStringList weatherStrings;
    const auto& weatherIds = e->getWeathers();
    for (const auto& weatherId : weatherIds) {
        weatherStrings << dataHandler->getWeatherString(weatherId);
    }
    item->setText(COL_WEATHER, weatherStrings.join(", "));
    QString shoeString = dataHandler->getShoeString(e->getShoe());
    item->setText(COL_SHOE, shoeString);
    item->setText(COL_COMMENT, e->getComment());
}

Exercise *ExerciseTable::getCurrentExercise() const
{
    int index = indexOfTopLevelItem(currentItem());
    return m_exercises->at(getIndexForExercise((topLevelItem(index)->data(COL_ID, Qt::DisplayRole)).toInt()));
}

int ExerciseTable::getIndexForExercise(int id) const
{
    for (int i = 0; i < m_exercises->size(); ++i)
        if (m_exercises->at(i)->getId() == id) return i;
    return -1;
}

bool ExerciseItem::operator<(const QTreeWidgetItem& other) const
{
    int column = treeWidget()->sortColumn();
    switch (column) {
    case ExerciseTable::COL_ID:
    case ExerciseTable::COL_DISTANCE:
        return data(column, Qt::DisplayRole).toInt() < other.data(column, Qt::DisplayRole).toInt();
    case ExerciseTable::COL_SPEED:
        return data(column, Qt::DisplayRole).toDouble() < other.data(column, Qt::DisplayRole).toDouble();
    case ExerciseTable::COL_DATE: {
        // E.g. "2008-01-01". It is assumed to be correct.
        QStringList dateList = text(column).split(DATE_SEPARATOR);
        QStringList dateListOther = other.text(column).split(DATE_SEPARATOR);
        int year = dateList[0].toInt();
        int yearOther = dateListOther[0].toInt();
        int month = dateList[1].toInt();
        int monthOther = dateListOther[1].toInt();
        int day = dateList[2].toInt();
        int dayOther = dateListOther[2].toInt();
        if (year < yearOther) {
            return true;
        } else if (year > yearOther) {
            return false;
        } else {
            if (month < monthOther) {
                return true;
            } else if (month > monthOther) {
                return false;
            } else {
                return day < dayOther;
            }
        }
        break; }
    case ExerciseTable::COL_TIME: // E.g. "6:00".
    case ExerciseTable::COL_DURATION: {
        QStringList timeList = text(column).split(TIME_SEPARATOR);
        QStringList timeListOther = other.text(column).split(TIME_SEPARATOR);
        int hour = timeList[0].toInt();
        int hourOther = timeListOther[0].toInt();
        int minute = timeList[1].toInt();
        int minuteOther = timeListOther[1].toInt();
        if (hour < hourOther) {
            return true;
        } else if (hour > hourOther) {
            return false;
        } else {
            return minute < minuteOther;
        }
        break; }
    default: // Sport/Place/Shoe/Comment/Weather. Alphabetical order.
        return data(column, Qt::DisplayRole).toString() < other.data(column, Qt::DisplayRole).toString();
    }
}
