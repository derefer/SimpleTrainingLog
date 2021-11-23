#include <QMessageBox>

#include "DataHandler.h"
#include "WeathersDialog.h"
#include "ui_WeathersDialog.h"

WeathersDialog::WeathersDialog(QWidget *parent, DataHandler *dataHandler) :
    QDialog(parent), ui(new Ui::WeathersDialog), m_dirty(false), dataHandler(dataHandler)
{
    ui->setupUi(this);

    QStringList headerLabels("#");
    headerLabels << "Name";
    ui->weathersTreeWidget->setHeaderLabels(headerLabels);

    for (int i = 0; i < dataHandler->weathers.size(); ++i) {
        // TODO: Set column width to content.
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(COL_ID, QString::number(dataHandler->weathers[i]->getId()));
        item->setText(COL_NAME, dataHandler->weathers[i]->getName());
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
            ui->nameLineEdit->clear();
            return;
        }
    }
    Weather *weather = new Weather(dataHandler->weathers.size(), name);
    dataHandler->weathers.append(weather);
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(COL_ID, QString::number(weather->getId()));
    item->setText(COL_NAME, weather->getName());
    ui->weathersTreeWidget->insertTopLevelItem(ui->weathersTreeWidget->topLevelItemCount(), item);
    ui->nameLineEdit->clear();
    m_dirty = true; // Activate the "Save" button.
}

void WeathersDialog::slotRemove()
{
    // TODO: Weather is optional. Don't remove the affected exercises.
    QTreeWidgetItem *item = ui->weathersTreeWidget->currentItem();
    if (!item) return;
    int id = (item->text(COL_NAME)).toInt();
    QString name = item->text(COL_NAME);
    int count = 0;
    for (const auto& exercise : dataHandler->exercises) {
        if ((exercise->getWeathers()).contains(id)) {
            ++count;
        }
    }
    if (count > 0) {
        int ret = QMessageBox::warning(this, tr("SimpleTrainingLog"), tr("%1 is used "
            "by %2 exercises. All of these exercises will be removed. Are "
            "you sure?").arg(name).arg(count), QMessageBox::Yes |
            QMessageBox::Default, QMessageBox::No | QMessageBox::Escape);
        if (ret != QMessageBox::Yes) {
            return;
        }
    }
    for (const auto& exercise : dataHandler->exercises) {
        if ((exercise->getWeathers()).contains(id)) {
            m_removedExercises.append(exercise->getId());
        }
    }
    dataHandler->removeWeatherById(id);
    delete ui->weathersTreeWidget->takeTopLevelItem(ui->weathersTreeWidget->indexOfTopLevelItem(item));
    ui->nameLineEdit->clear();
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
            ui->nameLineEdit->clear();
            return;
        }
    }
    int weatherId = -1;
    for (int i = 0; i < dataHandler->weathers.size(); ++i) {
        if (dataHandler->weathers[i]->getName() == item->text(COL_NAME)) {
            weatherId = dataHandler->weathers[i]->getId();
            dataHandler->weathers[i]->setName(name);
            break;
        }
    }
    if (weatherId >= 0) {
        for (const auto& exercise: dataHandler->exercises) {
            if ((exercise->getWeathers()).contains(weatherId)) {
                m_modifiedExercises.append(exercise->getId());
            }
        }
    }
    item->setText(COL_NAME, name);
    m_dirty = true;
}

void WeathersDialog::slotSetNameText(QTreeWidgetItem *item, int)
{
    ui->nameLineEdit->setText(item->text(COL_NAME));
}
