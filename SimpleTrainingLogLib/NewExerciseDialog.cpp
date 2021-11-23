#include <QCheckBox>
#include <QDate>
#include <QMessageBox>

#include "NewExerciseDialog.h"
#include "SimpleTrainingLogMainWindow.h"
#include "ui_NewExerciseDialog.h"

NewExerciseDialog::NewExerciseDialog(QWidget *parent, const QString& title, DataHandler *dataHandler) :
    QDialog(parent),
    ui(new Ui::NewExerciseDialog),
    dataHandler(dataHandler)
{
    ui->setupUi(this);

    QDate date = QDate::currentDate();
    for (int year = DATE_YEAR_START; year < date.year() + 1; ++year) {
        ui->yearsComboBox->addItem(QString::number(year));
    }
    for (int month = 1; month < 13; ++month) {
        QString monthString = month < 10 ? tr("0%1").arg(month) : QString::number(month);
        ui->monthsComboBox->addItem(dataHandler->months[month - 1], QVariant(monthString));
    }
    for (int day = 1; day < getRightDays(date) + 1; ++day) {
        QString dayString = day < 10 ? tr("0%1").arg(day) : QString::number(day);
        ui->daysComboBox->addItem(dayString);
    }
    // The default value of all date/time related QComboBoxes is coming from
    // QDate::currentDate(). It can be done only through signals emitted
    // directly.
    emit ui->yearsComboBox->setCurrentIndex(date.year() - DATE_YEAR_START);
    emit ui->monthsComboBox->setCurrentIndex(date.month() - 1);
    emit ui->daysComboBox->setCurrentIndex(date.day() - 1);

    setWindowTitle(title);
    ui->timeLineEdit->setFocus();
}

NewExerciseDialog::~NewExerciseDialog()
{
    delete ui;
}


QString NewExerciseDialog::getDate() const
{
    int monthIndex = ui->monthsComboBox->currentIndex();
    // Fetch all data to a QStringList and pass it to the parent widget. Data
    // is set for months only.
    return ui->yearsComboBox->currentText() + QString(DATE_SEPARATOR) +
        (ui->monthsComboBox->itemData(monthIndex)).toString() +
        QString(DATE_SEPARATOR) + ui->daysComboBox->currentText();
}

QStringList NewExerciseDialog::getPlaceList() const
{
    QList<QListWidgetItem*> selectedItems = ui->placeListWidget->selectedItems();
    QStringList placeList;
    for (int i = 0; i < selectedItems.size(); ++i)
        placeList << (selectedItems.at(i))->text();
    return placeList;
}

QString NewExerciseDialog::getShoe() const
{
    return ui->shoeComboBox->currentText();
}

QString NewExerciseDialog::getComment() const
{
    return ui->commentTextEdit->toPlainText();
}

QStringList NewExerciseDialog::getWeatherList() const
{
    QStringList weatherList;
    for (int i = 0; i < m_weathersCheckBoxList.size(); ++i) {
        QCheckBox *checkBox = m_weathersCheckBoxList.at(i);
        if (checkBox->checkState() == Qt::Checked)
            weatherList << checkBox->text();
    }
    return weatherList;
}

void NewExerciseDialog::slotOk()
{
    if ((ui->pulseLineEdit->text()).isEmpty()) ui->pulseLineEdit->setText("0/0");
    if ((ui->caloriesLineEdit->text()).isEmpty())
        ui->caloriesLineEdit->setText("0/0");
    QStringList pulseList = (ui->pulseLineEdit->text()).split(PULSE_SEPARATOR);
    QStringList caloriesList = (ui->caloriesLineEdit->
        text()).split(PULSE_SEPARATOR);
    if (pulseList.size() != 2 || caloriesList.size() != 2) {
        QMessageBox::critical(this, tr("Invalid Data"),
                              tr("The format of the \"HR (M/A)\" and "
                                 "\"Calories (C/F)\" fields should be: "
                                 "<number>/<number>. The numbers must be "
                                 "separated with a '/' character."),
                              QMessageBox::Ok | QMessageBox::Default);
        if (pulseList.size() != 2) {
            ui->pulseLineEdit->setFocus();
            ui->pulseLineEdit->home(false);
        } else {
            ui->caloriesLineEdit->setFocus();
            ui->caloriesLineEdit->home(false);
        }
        return;
    } else {
        // TODO: Think it over or move the whole thing to a separate function.
        bool ok_hr_max, ok_hr_avg, ok_cal_cal, ok_cal_fat;
        ok_hr_max = ok_hr_avg = ok_cal_cal = ok_cal_fat = false;
        int hr_max = pulseList.at(0).toInt(&ok_hr_max);
        int hr_avg = pulseList.at(1).toInt(&ok_hr_avg);
        int cal_cal = caloriesList.at(0).toInt(&ok_cal_cal);
        int cal_fat = caloriesList.at(1).toInt(&ok_cal_fat);
        if (!ok_hr_max || !ok_hr_avg || !ok_cal_cal || !ok_cal_fat ||
            hr_max < 0 || hr_avg < 0 || cal_cal < 0 || cal_fat < 0) {
            QMessageBox::critical(this, tr("Invalid Data"),
                                  tr("The format of the \"HR (M/A)\" and "
                                     "\"Calories (C/F)\" fields should be: "
                                     "<number>/<number>. The numbers must "
                                     "be non-negative integers."),
                                  QMessageBox::Ok | QMessageBox::Default);
            if (!ok_hr_max || !ok_hr_avg || hr_max < 0 || hr_avg < 0) {
                ui->pulseLineEdit->setFocus();
                ui->pulseLineEdit->home(false);
            } else {
                ui->caloriesLineEdit->setFocus();
                ui->caloriesLineEdit->home(false);
            }
            return;
        }
    }
    QStringList timeList = (ui->timeLineEdit->text()).split(TIME_SEPARATOR);
    QStringList durationList = (ui->durationLineEdit->
        text()).split(TIME_SEPARATOR);
    if (timeList.size() != 2 || durationList.size() != 2) {
        QMessageBox::critical(this, tr("Invalid Data"),
                              tr("The format of the \"Time\" and "
                                 "\"Duration\" fields should be: "
                                 "<number>:<number>. The numbers must be "
                                 "separated with a '/' character."),
                              QMessageBox::Ok | QMessageBox::Default);
        if (timeList.size() != 2) {
            ui->timeLineEdit->setFocus();
            ui->timeLineEdit->home(false);
        } else {
            ui->durationLineEdit->setFocus();
            ui->durationLineEdit->home(false);
        }
        return;
    } else {
        bool ok_time_h, ok_time_m, ok_dur_h, ok_dur_m;
        ok_time_h = ok_time_m = ok_dur_h = ok_dur_m = false;
        int time_h = timeList.at(0).toInt(&ok_time_h);
        int time_m = timeList.at(1).toInt(&ok_time_m);
        int dur_h = durationList.at(0).toInt(&ok_dur_h);
        int dur_m = durationList.at(1).toInt(&ok_dur_m);
        if (!ok_time_h || !ok_time_m || !ok_dur_h || !ok_dur_m ||
            time_h < 0 || time_m < 0 || dur_h < 0 || dur_m < 0) {
            QMessageBox::critical(this, tr("Invalid Data"),
                                  tr("The format of the \"Time\" and "
                                     "\"Duration\" fields should be: "
                                     "<number>/<number>. The numbers must "
                                     "be non-negative integers."),
                                  QMessageBox::Ok | QMessageBox::Default);
            if (!ok_time_h || !ok_time_m || time_h < 0 || time_m < 0) {
                ui->timeLineEdit->setFocus();
                ui->timeLineEdit->home(false);
            } else {
                ui->durationLineEdit->setFocus();
                ui->durationLineEdit->home(false);
            }
            return;
        }
    }
    bool ok_dist;
    int dist = (ui->distanceLineEdit->text()).toInt(&ok_dist);
    if (!ok_dist || dist < 0) {
        QMessageBox::critical(this, tr("Invalid Data"),
                              tr("The \"Distance\" field should contain only "
                                 "a non-negative integer value."),
                              QMessageBox::Ok | QMessageBox::Default);
        ui->distanceLineEdit->setFocus();
        ui->distanceLineEdit->home(false);
        return;
    }
    accept();
}

void NewExerciseDialog::slotCancel()
{
    reject();
}

void NewExerciseDialog::slotDaysForYear(int year)
{
    QDate date(year + DATE_YEAR_START, ui->monthsComboBox->currentIndex() + 1, 1);
    int old_day = ui->daysComboBox->currentIndex();
    ui->daysComboBox->clear();
    for (int day = 1; day < getRightDays(date) + 1; ++day)
        ui->daysComboBox->addItem(day < 10 ? tr("0%1").arg(day) : QString::number(day));
    if (old_day < ui->daysComboBox->count())
        ui->daysComboBox->setCurrentIndex(old_day);
}

void NewExerciseDialog::slotDaysForMonth(int month)
{
    QDate date(ui->yearsComboBox->currentIndex() + DATE_YEAR_START, month + 1, 1);
    int old_day = ui->daysComboBox->currentIndex();
    ui->daysComboBox->clear();
    for (int day = 1; day < getRightDays(date) + 1; ++day)
        ui->daysComboBox->addItem(day < 10 ? tr("0%1").arg(day) : QString::number(day));
    if (old_day < ui->daysComboBox->count())
        ui->daysComboBox->setCurrentIndex(old_day);
}

void NewExerciseDialog::addSportStrings(const QStringList& sportStrings)
{
    for (int i = 0; i < sportStrings.size(); ++i)
        ui->sportComboBox->addItem(sportStrings.at(i));
}

void NewExerciseDialog::addShoeStrings(const QStringList& shoeStrings)
{
    for (int i = 0; i < shoeStrings.size(); ++i)
        ui->shoeComboBox->addItem(shoeStrings.at(i));
}

void NewExerciseDialog::addPlaceStrings(const QStringList& placeStrings)
{
    for (int i = 0; i < placeStrings.size(); ++i)
        ui->placeListWidget->addItem(placeStrings.at(i));
}

void NewExerciseDialog::addWeatherStrings(const QStringList& weatherStrings)
{
    for (int i = 0; i < weatherStrings.size(); ++i) {
        // It's a three column checkbox list.  For some reason, if one of
        // QGridLayout's column is empty, there will be a gap between the
        // rows.
        QCheckBox *weatherCheckBox = new QCheckBox(weatherStrings.at(i));
        m_weathersCheckBoxList.append(weatherCheckBox);
        ui->weatherLayout->addWidget(weatherCheckBox, i / 3, i % 3);
    }
}

void NewExerciseDialog::setReadOnly()
{
    ui->yearsComboBox->setEnabled(false);
    ui->monthsComboBox->setEnabled(false);
    ui->daysComboBox->setEnabled(false);
    ui->timeLineEdit->setReadOnly(true);
    ui->distanceLineEdit->setReadOnly(true);
    ui->durationLineEdit->setReadOnly(true);
    ui->pulseLineEdit->setReadOnly(true);
    ui->caloriesLineEdit->setReadOnly(true);
    ui->sportComboBox->setEnabled(false);
    ui->placeListWidget->setEnabled(false);
    ui->shoeComboBox->setEnabled(false);
    for (int i = 0; i < m_weathersCheckBoxList.size(); ++i)
        (m_weathersCheckBoxList.at(i))->setEnabled(false);
    ui->commentTextEdit->setReadOnly(true);
}

void NewExerciseDialog::setDate(const QStringList& date)
{
    for (int i = 0; i < ui->yearsComboBox->count(); ++i)
        if (date.at(0) == ui->yearsComboBox->itemText(i)) {
            emit ui->yearsComboBox->setCurrentIndex(i);
            break;
        }

    for (int i = 0; i < ui->monthsComboBox->count(); ++i)
        if (dataHandler->months[date.at(1).toInt() - 1] == ui->monthsComboBox->itemText(i)) {
            emit ui->monthsComboBox->setCurrentIndex(i);
            break;
        }

    for (int i = 0; i < ui->daysComboBox->count(); ++i)
        if (date.at(2) == ui->daysComboBox->itemText(i)) {
            emit ui->daysComboBox->setCurrentIndex(i);
            break;
        }
}

void NewExerciseDialog::setPulse(int maxPulse, int avgPulse)
{
    // It's better to do the presentation here.  We get only the data.  We are
    // responsible for the correct presentation of it.
    ui->pulseLineEdit->setText(QString("%1/%2").arg(maxPulse).arg(avgPulse));
}

void NewExerciseDialog::setCalories(int calories, int fat)
{
    ui->caloriesLineEdit->setText(QString("%1/%2").arg(calories).arg(fat));
}

void NewExerciseDialog::setSport(const QString& sport)
{
    for (int i = 0; i < ui->sportComboBox->count(); ++i)
        if (sport == ui->sportComboBox->itemText(i)) {
            emit ui->sportComboBox->setCurrentIndex(i);
            break;
        }
}

void NewExerciseDialog::setShoe(const QString& shoe)
{
    for (int i = 0; i < ui->shoeComboBox->count(); ++i) {
        if (shoe == ui->shoeComboBox->itemText(i)) {
            emit ui->shoeComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void NewExerciseDialog::setPlaces(const QStringList& places)
{
    for (int i = 0; i < ui->placeListWidget->count(); ++i)
        for (int j = 0; j < places.size(); ++j)
            if (places.at(j) == ui->placeListWidget->item(i)->text())
                ui->placeListWidget->item(i)->setSelected(true);
}

void NewExerciseDialog::setWeathers(const QStringList& weathers)
{
    for (int i = 0; i < m_weathersCheckBoxList.size(); ++i)
        for (int j = 0; j < weathers.size(); ++j)
            if (weathers.at(j) == (m_weathersCheckBoxList.at(i))->text())
                (m_weathersCheckBoxList.at(i))->setCheckState(Qt::Checked);
}

int NewExerciseDialog::getRightDays(const QDate& date) const
{
    if (QDate::isLeapYear(date.year()) && date.month() == 2) return 29;
    switch (date.month()) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 2:
            return 28;
        default:
            return 30;
    }
}

QString NewExerciseDialog::getTime() const { return ui->timeLineEdit->text(); }
QString NewExerciseDialog::getDistance() const { return ui->distanceLineEdit->text(); }
QString NewExerciseDialog::getDuration() const { return ui->durationLineEdit->text(); }
QString NewExerciseDialog::getPulse() const { return ui->pulseLineEdit->text(); }
QString NewExerciseDialog::getCalories() const { return ui->caloriesLineEdit->text(); }
QString NewExerciseDialog::getSport() const { return ui->sportComboBox->currentText(); }
void NewExerciseDialog::setTime(const QString& time) { ui->timeLineEdit->setText(time); }
void NewExerciseDialog::setDistance(const QString& distance) { ui->distanceLineEdit->setText(distance); }
void NewExerciseDialog::setDuration(const QString& duration) { ui->durationLineEdit->setText(duration); }
void NewExerciseDialog::setComment(const QString& comment) { ui->commentTextEdit->setText(comment); }
