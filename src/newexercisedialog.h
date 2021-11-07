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

#ifndef NEWEXERCISEDIALOG_H_
#define NEWEXERCISEDIALOG_H_

#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>

#include "dataelements.h"
#include "ktrainer.h"

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

#endif  // NEWEXERCISEDIALOG_H_
