#include <QMessageBox>

#include "DataElements.h"
#include "WeathersDialog.h"
#include "ui_WeathersDialog.h"

WeathersDialog::WeathersDialog(QWidget *parent, QList<Weather*> *weathers, QList<Exercise*> *exercises) :
    QDialog(parent), ui(new Ui::WeathersDialog), m_weathers(weathers), m_exercises(exercises), m_dirty(false)
{
    ui->setupUi(this);

    QStringList headerLabels("#");
    headerLabels << "Name";
    ui->weathersTreeWidget->setHeaderLabels(headerLabels);

    for (int i = 0; i < weathers->size(); ++i) {
        // TODO Set column width to content.
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(COL_ID, QString::number(weathers->at(i)->getId()));
        item->setText(COL_NAME, weathers->at(i)->getName());
        ui->weathersTreeWidget->insertTopLevelItem(i, item);
        ui->weathersTreeWidget->setColumnWidth(COL_ID, 56);
    }
}

WeathersDialog::~WeathersDialog()
{
    delete ui;
}

void WeathersDialog::slotOk()
{
    accept();
}

void WeathersDialog::slotCancel()
{
    reject();
}

void WeathersDialog::slotAdd()
{
    QString name = ui->nameLineEdit->text();
    if (name.isEmpty()) return;
    for (int i = 0; i < ui->weathersTreeWidget->topLevelItemCount(); ++i) {
        if (name == ui->weathersTreeWidget->topLevelItem(i)->text(COL_NAME)) {
            ui->nameLineEdit->setText("");
            return;
        }
    }
    Weather *weather = new Weather(m_weathers->size(), name);
    m_weathers->append(weather);
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(COL_ID, QString::number(weather->getId()));
    item->setText(COL_NAME, weather->getName());
    ui->weathersTreeWidget->insertTopLevelItem(ui->weathersTreeWidget->topLevelItemCount(), item);
    ui->nameLineEdit->setText("");
    m_dirty = true;  // Activate the "Save" button.
}

void WeathersDialog::slotRemove()
{
    // TODO Weather is optional.  Don't remove the affected exercises.
    QTreeWidgetItem *item = ui->weathersTreeWidget->currentItem();
    if (!item) return;
    QString name = item->text(COL_NAME);
    int count = 0;
    for (int i = 0; i < m_exercises->size(); ++i)
        if ((m_exercises->at(i)->getWeathers()).contains(getWeatherId(name)))
            ++count;
    if (count > 0) {
        int ret = QMessageBox::warning(this, tr("SimpleTrainingLog"), tr("%1 is used "
            "by %2 exercises. All of these exercises will be removed. Are "
            "you sure?").arg(name).arg(count), QMessageBox::Yes |
            QMessageBox::Default, QMessageBox::No | QMessageBox::Escape);
        if (ret != QMessageBox::Yes)
            return;
    }
    for (int i = 0; i < m_exercises->size(); ++i)
        if ((m_exercises->at(i)->getWeathers()).contains(getWeatherId(name)))
            m_removedExercises.append(m_exercises->at(i)->getId());
    removeWeatherById(getWeatherId(name));
    delete ui->weathersTreeWidget->takeTopLevelItem(ui->weathersTreeWidget->indexOfTopLevelItem(item));
    ui->nameLineEdit->setText("");
    m_dirty = true;
}

void WeathersDialog::slotSave()
{
    QTreeWidgetItem *item = ui->weathersTreeWidget->currentItem();
    if (!item) slotAdd();
    QString name = ui->nameLineEdit->text();
    if (name.isEmpty()) return;
    for (int i = 0; i < ui->weathersTreeWidget->topLevelItemCount(); ++i) {
        if (name == ui->weathersTreeWidget->topLevelItem(i)->text(COL_NAME)) {
            ui->nameLineEdit->setText("");
            return;
        }
    }
    int weatherId = -1;
    for (int i = 0; i < m_weathers->size(); ++i) {
        if (m_weathers->at(i)->getName() == item->text(COL_NAME)) {
            weatherId = m_weathers->at(i)->getId();
            m_weathers->at(i)->setName(name);
            break;
        }
    }
    if (weatherId >= 0) {
        for (int i = 0; i < m_exercises->size(); ++i)
            if ((m_exercises->at(i)->getWeathers()).contains(weatherId))
                m_modifiedExercises.append(m_exercises->at(i)->getId());
    }
    item->setText(COL_NAME, name);
    m_dirty = true;
}

void WeathersDialog::slotSetNameText(QTreeWidgetItem *item, int)
{
    ui->nameLineEdit->setText(item->text(COL_NAME));
}
