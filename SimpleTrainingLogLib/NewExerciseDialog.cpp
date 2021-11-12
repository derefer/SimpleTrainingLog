#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>

#include "NewExerciseDialog.h"

NewExerciseDialog::NewExerciseDialog(QWidget *parent, const QString& title) : QDialog(parent)
{
    dateLabel = new QLabel(tr("Date:"));
    timeLabel = new QLabel(tr("Time:"));
    distanceLabel = new QLabel(tr("Distance:"));
    durationLabel = new QLabel(tr("Duration:"));
    pulseLabel = new QLabel(tr("HR (M/A):"));
    caloriesLabel = new QLabel(tr("Calories (C/F):"));
    sportLabel = new QLabel(tr("Sport:"));
    placeLabel = new QLabel(tr("Place:"));
    shoeLabel = new QLabel(tr("Shoe:"));
    commentLabel = new QLabel(tr("Comment:"));
    weatherLabel = new QLabel(tr("Weather:"));
    timeLineEdit = new QLineEdit;
    // `HH:MM:SS'.  TODO Syntax check.
    timeLineEdit->setMaxLength(8);
    distanceLineEdit = new QLineEdit;
    durationLineEdit = new QLineEdit;
    pulseLineEdit = new QLineEdit;
    caloriesLineEdit = new QLineEdit;
    commentTextEdit = new QTextEdit;
    okButton = new QPushButton(tr("OK"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(slotOK()));
    cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(slotCancel()));

    placesListWidget = new QListWidget;
    placesListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    yearsComboBox = new QComboBox;
    QDate date = QDate::currentDate();
    for (int year = DATE_YEAR_START; year < date.year() + 1; ++year)
        yearsComboBox->addItem(QString::number(year));
    monthsComboBox = new QComboBox;
    for (int month = 1; month < 13; ++month) {
        QString monthString = month < 10 ? tr("0%1").arg(month) :
            QString::number(month);
        monthsComboBox->addItem(months[month - 1], QVariant(monthString));
    }
    daysComboBox = new QComboBox;
    for (int day = 1; day < getRightDays(date) + 1; ++day) {
        QString dayString = day < 10 ? tr("0%1").arg(day) :
            QString::number(day);
        daysComboBox->addItem(dayString);
    }
    connect(yearsComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(slotDaysForYear(int)));
    connect(monthsComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(slotDaysForMonth(int)));
    // The default value of all date/time related QComboBoxes is coming from
    // QDate::currentDate(). It can be done only through signals emitted
    // directly.
    emit yearsComboBox->setCurrentIndex(date.year() - DATE_YEAR_START);
    emit monthsComboBox->setCurrentIndex(date.month() - 1);
    emit daysComboBox->setCurrentIndex(date.day() - 1);

    m_weatherLayout = new QGridLayout;
    // Set from outside.
    sportsComboBox = new QComboBox;
    shoesComboBox = new QComboBox;

    // Freely adjustable fields?
    QHBoxLayout *dateLayout = new QHBoxLayout;
    dateLayout->addWidget(yearsComboBox);
    dateLayout->addWidget(monthsComboBox);
    dateLayout->addWidget(daysComboBox);

    QGridLayout *dataLayout = new QGridLayout;
    dataLayout->setColumnMinimumWidth(0, 72);
    dataLayout->addWidget(dateLabel, 0, 0);
    dataLayout->addWidget(timeLabel, 1, 0);
    dataLayout->addWidget(distanceLabel, 2, 0);
    dataLayout->addWidget(durationLabel, 3, 0);
    dataLayout->addWidget(pulseLabel, 4, 0);
    dataLayout->addWidget(caloriesLabel, 5, 0);
    dataLayout->addWidget(sportLabel, 6, 0);
    dataLayout->addWidget(placeLabel, 7, 0);
    dataLayout->addWidget(shoeLabel, 8, 0);
    dataLayout->addWidget(weatherLabel, 9, 0);
    dataLayout->addWidget(commentLabel, 10, 0);
    dataLayout->addLayout(dateLayout, 0, 1);
    dataLayout->addWidget(timeLineEdit, 1, 1);
    dataLayout->addWidget(distanceLineEdit, 2, 1);
    dataLayout->addWidget(durationLineEdit, 3, 1);
    dataLayout->addWidget(pulseLineEdit, 4, 1);
    dataLayout->addWidget(caloriesLineEdit, 5, 1);
    dataLayout->addWidget(sportsComboBox, 6, 1);
    dataLayout->addWidget(placesListWidget, 7, 1);
    dataLayout->addWidget(shoesComboBox, 8, 1);
    dataLayout->addLayout(m_weatherLayout, 9, 1);
    dataLayout->addWidget(commentTextEdit, 10, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(dataLayout);
    layout->addLayout(buttonLayout);
    setLayout(layout);

    setWindowTitle(title);
    timeLineEdit->setFocus();
}

QString NewExerciseDialog::getDate() const
{
    int monthIndex = monthsComboBox->currentIndex();
    // Fetch all data to a QStringList and pass it to the parent widget. Data
    // is set for months only.
    return yearsComboBox->currentText() + QString(DATE_SEPARATOR) +
        (monthsComboBox->itemData(monthIndex)).toString() +
        QString(DATE_SEPARATOR) + daysComboBox->currentText();
}

QStringList NewExerciseDialog::getPlaceList() const
{
    QList<QListWidgetItem*> selectedItems = placesListWidget->selectedItems();
    QStringList placeList;
    for (int i = 0; i < selectedItems.size(); ++i)
        placeList << (selectedItems.at(i))->text();
    return placeList;
}

QString NewExerciseDialog::getShoe() const
{
    return shoesComboBox->currentText();
}

QString NewExerciseDialog::getComment() const
{
    return commentTextEdit->toPlainText();
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

void NewExerciseDialog::slotOK()
{
    if ((pulseLineEdit->text()).isEmpty()) pulseLineEdit->setText("0/0");
    if ((caloriesLineEdit->text()).isEmpty())
        caloriesLineEdit->setText("0/0");
    QStringList pulseList = (pulseLineEdit->text()).split(PULSE_SEPARATOR);
    QStringList caloriesList = (caloriesLineEdit->
        text()).split(PULSE_SEPARATOR);
    if (pulseList.size() != 2 || caloriesList.size() != 2) {
        QMessageBox::critical(this, tr("Invalid Data"), 
                              tr("The format of the \"HR (M/A)\" and "
                                 "\"Calories (C/F)\" fields should be: "
                                 "<number>/<number>. The numbers must be "
                                 "separated with a '/' character."),
                              QMessageBox::Ok | QMessageBox::Default);
        if (pulseList.size() != 2) {
            pulseLineEdit->setFocus();
            pulseLineEdit->home(false);
        } else {
            caloriesLineEdit->setFocus();
            caloriesLineEdit->home(false);
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
                pulseLineEdit->setFocus();
                pulseLineEdit->home(false);
            } else {
                caloriesLineEdit->setFocus();
                caloriesLineEdit->home(false);
            }
            return;
        }
    }
    QStringList timeList = (timeLineEdit->text()).split(TIME_SEPARATOR);
    QStringList durationList = (durationLineEdit->
        text()).split(TIME_SEPARATOR);
    if (timeList.size() != 2 || durationList.size() != 2) {
        QMessageBox::critical(this, tr("Invalid Data"), 
                              tr("The format of the \"Time\" and "
                                 "\"Duration\" fields should be: "
                                 "<number>:<number>. The numbers must be "
                                 "separated with a '/' character."),
                              QMessageBox::Ok | QMessageBox::Default);
        if (timeList.size() != 2) {
            timeLineEdit->setFocus();
            timeLineEdit->home(false);
        } else {
            durationLineEdit->setFocus();
            durationLineEdit->home(false);
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
                timeLineEdit->setFocus();
                timeLineEdit->home(false);
            } else {
                durationLineEdit->setFocus();
                durationLineEdit->home(false);
            }
            return;
        }
    }
    bool ok_dist;
    int dist = (distanceLineEdit->text()).toInt(&ok_dist);
    if (!ok_dist || dist < 0) {
        QMessageBox::critical(this, tr("Invalid Data"),
                              tr("The \"Distance\" field should contain only "
                                 "a non-negative integer value."),
                              QMessageBox::Ok | QMessageBox::Default);
        distanceLineEdit->setFocus();
        distanceLineEdit->home(false);
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
    QDate date(year + DATE_YEAR_START, monthsComboBox->currentIndex() + 1, 1);
    int old_day = daysComboBox->currentIndex();
    daysComboBox->clear();
    for (int day = 1; day < getRightDays(date) + 1; ++day)
        daysComboBox->addItem(day < 10 ? tr("0%1").arg(day)
            : QString::number(day));
    if (old_day < daysComboBox->count())
        daysComboBox->setCurrentIndex(old_day);        
}

void NewExerciseDialog::slotDaysForMonth(int month)
{
    QDate date(yearsComboBox->currentIndex() + DATE_YEAR_START, month + 1, 1);
    int old_day = daysComboBox->currentIndex();
    daysComboBox->clear();
    for (int day = 1; day < getRightDays(date) + 1; ++day)
        daysComboBox->addItem(day < 10 ? tr("0%1").arg(day)
            : QString::number(day));
    if (old_day < daysComboBox->count())
        daysComboBox->setCurrentIndex(old_day);
}

void NewExerciseDialog::addSportStrings(const QStringList& sportStrings)
{
    if (!sportsComboBox) return;
    for (int i = 0; i < sportStrings.size(); ++i)
        sportsComboBox->addItem(sportStrings.at(i));
}

void NewExerciseDialog::addShoeStrings(const QStringList& shoeStrings)
{
    if (!shoesComboBox) return;
    for (int i = 0; i < shoeStrings.size(); ++i)
        shoesComboBox->addItem(shoeStrings.at(i));
}

void NewExerciseDialog::addPlaceStrings(const QStringList& placeStrings)
{
    if (!placesListWidget) return;
    for (int i = 0; i < placeStrings.size(); ++i)
        placesListWidget->addItem(placeStrings.at(i));
}

void NewExerciseDialog::addWeatherStrings(const QStringList& weatherStrings)
{
    if (!m_weatherLayout) return;
    for (int i = 0; i < weatherStrings.size(); ++i) {
        // It's a three column checkbox list.  For some reason, if one of
        // QGridLayout's column is empty, there will be a gap between the
        // rows.
        QCheckBox *weatherCheckBox = new QCheckBox(weatherStrings.at(i));
        m_weathersCheckBoxList.append(weatherCheckBox);
        m_weatherLayout->addWidget(weatherCheckBox, i / 3, i % 3);
    }
}

void NewExerciseDialog::setReadOnly()
{
    yearsComboBox->setEnabled(false);
    monthsComboBox->setEnabled(false);
    daysComboBox->setEnabled(false);
    timeLineEdit->setReadOnly(true);
    distanceLineEdit->setReadOnly(true);
    durationLineEdit->setReadOnly(true);
    pulseLineEdit->setReadOnly(true);
    caloriesLineEdit->setReadOnly(true);
    sportsComboBox->setEnabled(false);
    placesListWidget->setEnabled(false);
    shoesComboBox->setEnabled(false);
    for (int i = 0; i < m_weathersCheckBoxList.size(); ++i)
        (m_weathersCheckBoxList.at(i))->setEnabled(false);
    commentTextEdit->setReadOnly(true);
}

void NewExerciseDialog::setDate(const QStringList& date)
{
    for (int i = 0; i < yearsComboBox->count(); ++i)
        if (date.at(0) == yearsComboBox->itemText(i)) {
            emit yearsComboBox->setCurrentIndex(i);
            break;
        }

    for (int i = 0; i < monthsComboBox->count(); ++i)
        if (months[date.at(1).toInt() - 1] == monthsComboBox->itemText(i)) {
            emit monthsComboBox->setCurrentIndex(i);
            break;
        }

    for (int i = 0; i < daysComboBox->count(); ++i)
        if (date.at(2) == daysComboBox->itemText(i)) {
            emit daysComboBox->setCurrentIndex(i);
            break;
        }
}

void NewExerciseDialog::setPulse(int maxPulse, int avgPulse)
{
    // It's better to do the presentation here.  We get only the data.  We are
    // responsible for the correct presentation of it.
    pulseLineEdit->setText(QString("%1/%2").arg(maxPulse).arg(avgPulse));
}

void NewExerciseDialog::setCalories(int calories, int fat)
{
    caloriesLineEdit->setText(QString("%1/%2").arg(calories).arg(fat));
}

void NewExerciseDialog::setSport(const QString& sport)
{
    for (int i = 0; i < sportsComboBox->count(); ++i)
        if (sport == sportsComboBox->itemText(i)) {
            emit sportsComboBox->setCurrentIndex(i);
            break;
        }
}

void NewExerciseDialog::setShoe(const QString& shoe)
{
    for (int i = 0; i < shoesComboBox->count(); ++i)
        if (shoe == shoesComboBox->itemText(i)) {
            emit shoesComboBox->setCurrentIndex(i);
            break;
        }
}

void NewExerciseDialog::setPlaces(const QStringList& places)
{
    for (int i = 0; i < placesListWidget->count(); ++i)
        for (int j = 0; j < places.size(); ++j)
            if (places.at(j) == placesListWidget->item(i)->text())
                placesListWidget->item(i)->setSelected(true);
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
