#include <QMessageBox>

#include "DataHandler.h"
#include "PlacesDialog.h"
#include "ui_PlacesDialog.h"

PlacesDialog::PlacesDialog(QWidget *parent, DataHandler *dataHandler) :
    QDialog(parent), ui(new Ui::PlacesDialog), m_dirty(false), dataHandler(dataHandler)
{
    ui->setupUi(this);

    QStringList headerLabels("#");
    headerLabels << "Name";
    ui->placesTreeWidget->setHeaderLabels(headerLabels);

    for (int i = 0; i < dataHandler->places.size(); ++i) {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(COL_ID, QString::number(dataHandler->places[i]->getId()));
        item->setText(COL_NAME, dataHandler->places[i]->getName());
        ui->placesTreeWidget->insertTopLevelItem(i, item);
        ui->placesTreeWidget->setColumnWidth(COL_ID, 56);
    }
}

PlacesDialog::~PlacesDialog()
{
    delete ui;
}

void PlacesDialog::slotOk()
{
    accept();
}

void PlacesDialog::slotCancel()
{
    reject();
}

void PlacesDialog::slotAdd()
{
    QString name = ui->nameLineEdit->text();
    if (name.isEmpty()) return;
    for (int i = 0; i < ui->placesTreeWidget->topLevelItemCount(); ++i) {
        if (name == ui->placesTreeWidget->topLevelItem(i)->text(COL_NAME)) {
            ui->nameLineEdit->clear();
            return;
        }
    }
    Place *place = new Place(m_places->size(), name);
    m_places->append(place);
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(COL_ID, QString::number(place->getId()));
    item->setText(COL_NAME, place->getName());
    ui->placesTreeWidget->insertTopLevelItem(ui->placesTreeWidget->topLevelItemCount(), item);
    ui->nameLineEdit->clear();
    m_dirty = true;
}

void PlacesDialog::slotRemove()
{
    // TODO: Place is optional. Don't remove the affected exercises.
    QTreeWidgetItem *item = ui->placesTreeWidget->currentItem();
    if (!item) return;
    int id = (item->text(COL_ID)).toInt();
    QString name = item->text(COL_NAME);
    int count = 0;
    for (const auto& exercise : m_exercises) {
        if ((exercise->getPlaces()).contains(id)) {
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
    for (const auto& exercise : m_exercises) {
        if ((exercise->getPlaces()).contains(id)) {
            m_removedExercises.append(exercise->getId());
        }
    }
    dataHandler->removePlaceById(id);
    delete ui->placesTreeWidget->takeTopLevelItem(ui->placesTreeWidget->indexOfTopLevelItem(item));
    ui->nameLineEdit->clear();
    m_dirty = true;
}

void PlacesDialog::slotSave()
{
    QTreeWidgetItem *item = ui->placesTreeWidget->currentItem();
    if (!item) slotAdd();
    QString name = ui->nameLineEdit->text();
    if (name.isEmpty()) return;
    for (int i = 0; i < ui->placesTreeWidget->topLevelItemCount(); ++i) {
        if (name == ui->placesTreeWidget->topLevelItem(i)->text(COL_NAME)) {
            ui->nameLineEdit->clear();
            return;
        }
    }
    int placeId = -1;
    for (int i = 0; i < m_places->size(); ++i) {
        if (m_places->at(i)->getName() == item->text(COL_NAME)) {
            placeId = m_places->at(i)->getId();
            m_places->at(i)->setName(name);
            break;
        }
    }
    if (placeId >= 0) {
        for (const auto& exercise : m_exercises) {
            if ((exercise->getPlaces()).contains(placeId)) {
                m_modifiedExercises.append(exercise->getId());
            }
        }
    }
    item->setText(COL_NAME, name);
    m_dirty = true;
}

void PlacesDialog::slotSetNameText(QTreeWidgetItem *item, int)
{
    ui->nameLineEdit->setText(item->text(COL_NAME));
}
