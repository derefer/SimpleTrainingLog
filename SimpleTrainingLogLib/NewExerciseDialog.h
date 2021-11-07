#pragma once

#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>

#include "DataElements.h"
#include "SimpleTrainingLogMainWindow.h"

class QLabel;
class QLineEdit;
class QComboBox;
class QListWidget;
class QTextEdit;
class QPushButton;
class QErrorMessage;
class QCheckBox;
class QGridLayout;

class NewExerciseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewExerciseDialog(QWidget *parent, const QString& title);
    void addSportStrings(const QStringList& sportStrings);
    void addShoeStrings(const QStringList& shoeStrings);
    void addPlaceStrings(const QStringList& placeStrings);
    void addWeatherStrings(const QStringList& weatherStrings);
    QString getDate() const;
    inline QString getTime() const { return timeLineEdit->text(); }
    inline QString getDistance() const { return distanceLineEdit->text(); }
    inline QString getDuration() const { return durationLineEdit->text(); }
    inline QString getPulse() const { return pulseLineEdit->text(); }
    inline QString getCalories() const { return caloriesLineEdit->text(); }
    inline QString getSport() const { return sportsComboBox->currentText(); }
    QStringList getPlaceList() const;
    QString getShoe() const;
    QString getComment() const;
    QStringList getWeatherList() const;
    void setDate(const QStringList& date);
    void setTime(const QString& time) { timeLineEdit->setText(time); }
    void setDistance(const QString& distance)
        { distanceLineEdit->setText(distance); }
    void setDuration(const QString& duration)
        { durationLineEdit->setText(duration); }
    void setPulse(int maxPulse, int avgPulse);
    void setCalories(int calories, int fat);
    void setSport(const QString& sport);
    void setPlaces(const QStringList& places);
    void setShoe(const QString& shoe);
    void setWeathers(const QStringList& weathers);
    void setComment(const QString& comment)
        { commentTextEdit->setText(comment); }
    void setReadOnly();
    int getRightDays(const QDate& date) const;

private slots:
    void slotOK();
    void slotCancel();
    void slotDaysForYear(int year);
    void slotDaysForMonth(int month);

private:
    QLabel *dateLabel;
    QLabel *timeLabel;
    QLabel *distanceLabel;
    QLabel *durationLabel;
    QLabel *pulseLabel;
    QLabel *caloriesLabel;
    QLabel *sportLabel;
    QLabel *placeLabel;
    QLabel *shoeLabel;
    QLabel *weatherLabel;
    QLabel *commentLabel;
    QLineEdit *timeLineEdit;
    QLineEdit *distanceLineEdit;
    QLineEdit *durationLineEdit;
    QLineEdit *pulseLineEdit;
    QLineEdit *caloriesLineEdit;
    QTextEdit *commentTextEdit;
    QGridLayout *m_weatherLayout;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QErrorMessage *errorMessageDialog;
    QComboBox *yearsComboBox;  // For easier selection.
    QComboBox *monthsComboBox;
    QComboBox *daysComboBox;
    QComboBox *sportsComboBox;
    QComboBox *shoesComboBox;
    QListWidget *placesListWidget;
    QList<QCheckBox*> m_weathersCheckBoxList;
};
