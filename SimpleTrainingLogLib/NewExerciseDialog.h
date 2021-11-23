#pragma once

#include <QDialog>

class QCheckBox;

class DataHandler;

namespace Ui {
class NewExerciseDialog;
}

class NewExerciseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewExerciseDialog(QWidget *parent, const QString& title, DataHandler *dataHandler);
    ~NewExerciseDialog();
    void addSportStrings(const QStringList& sportStrings);
    void addShoeStrings(const QStringList& shoeStrings);
    void addPlaceStrings(const QStringList& placeStrings);
    void addWeatherStrings(const QStringList& weatherStrings);
    QString getDate() const;
    QString getTime() const;
    QString getDistance() const;
    QString getDuration() const;
    QString getPulse() const;
    QString getCalories() const;
    QString getSport() const;
    QStringList getPlaceList() const;
    QString getShoe() const;
    QString getComment() const;
    QStringList getWeatherList() const;
    void setDate(const QStringList& date);
    void setTime(const QString& time);
    void setDistance(const QString& distance);
    void setDuration(const QString& duration);
    void setPulse(int maxPulse, int avgPulse);
    void setCalories(int calories, int fat);
    void setSport(const QString& sport);
    void setPlaces(const QStringList& places);
    void setShoe(const QString& shoe);
    void setWeathers(const QStringList& weathers);
    void setComment(const QString& comment);
    void setReadOnly();
    int getRightDays(const QDate& date) const;

private slots:
    void slotOk();
    void slotCancel();
    void slotDaysForYear(int year);
    void slotDaysForMonth(int month);

private:
    Ui::NewExerciseDialog *ui;
    QList<QCheckBox*> m_weathersCheckBoxList;
    DataHandler *dataHandler;
};
