#include <QMessageBox>
#include <QColorDialog>

#include "DataHandler.h"
#include "SportsDialog.h"
#include "ui_SportsDialog.h"

SportsDialog::SportsDialog(QWidget *parent, DataHandler *dataHandler) :
    QDialog(parent), ui(new Ui::SportsDialog), m_dirty(false), dataHandler(dataHandler)
{
    ui->setupUi(this);

    QStringList headerLabels("#");
    headerLabels << "Name" << "Color";
    ui->sportsTreeWidget->setHeaderLabels(headerLabels);

    for (int i = 0; i < dataHandler->sports.size(); ++i) {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(COL_ID, QString::number(dataHandler->sports[i]->getId()));
        item->setText(COL_NAME, dataHandler->sports[i]->getName());
        QColor color = dataHandler->sports[i]->getColor();
        item->setText(COL_COLOR, (color.name()));
        for (int j = 0; j < item->columnCount(); ++j)
            item->setBackground(j, QBrush(color));
        ui->sportsTreeWidget->insertTopLevelItem(i, item);
        ui->sportsTreeWidget->setColumnWidth(COL_ID, 56);
    }
}

SportsDialog::~SportsDialog()
{
    delete ui;
}

void SportsDialog::slotOk()
{
    accept();
}

void SportsDialog::slotCancel()
{
    reject();
}

void SportsDialog::slotSetColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    ui->colorLineEdit->setText(color.name());
}

// Add a new sport to the list of sports. The QTextLineEdit is always cleared.
// TODO: Better error handling.
void SportsDialog::slotAdd()
{
    QString name = ui->nameLineEdit->text();
    QColor color = QColor(ui->colorLineEdit->text());
    if (name.isEmpty() || !color.isValid()) return;
    for (int i = 0; i < ui->sportsTreeWidget->topLevelItemCount(); ++i) {
        if (name == ui->sportsTreeWidget->topLevelItem(i)->text(COL_NAME)) {
            ui->nameLineEdit->clear();
            ui->colorLineEdit->clear();
            return;
        }
    }
    Sport *sport = new Sport(m_sports->size(), name, color);
    m_sports->append(sport);
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(COL_ID, QString::number(sport->getId()));
    item->setText(COL_NAME, name);
    item->setText(COL_COLOR, color.name());
    ui->sportsTreeWidget->insertTopLevelItem(ui->sportsTreeWidget->topLevelItemCount(), item);
    ui->nameLineEdit->clear();
    ui->colorLineEdit->clear();
    for (int col = 0; col < item->columnCount(); ++col)
        item->setBackground(col, QBrush(color));
    m_dirty = true; // Activate the "Save" button.
}

void SportsDialog::slotRemove()
{
    QTreeWidgetItem *item = ui->sportsTreeWidget->currentItem();
    if (!item) return;
    int id = (item->text(COL_ID)).toInt();
    QString name = item->text(COL_NAME);
    int count = 0;
    for (const auto& exercise : m_exercises) {
        if (exercise->getSport() == id) {
            ++count;
        }
    }
    // Be user friendly and warn. The QTreeWidget in the background will be
    // cleared after the dialog is closed. It's not done here.
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
        if (exercise->getSport() == id) {
            m_removedExercises.append(exercise->getId());
        }
    }
    dataHandler->removeSportById(id);
    delete ui->sportsTreeWidget->takeTopLevelItem(ui->sportsTreeWidget->indexOfTopLevelItem(item));
    ui->nameLineEdit->clear();
    ui->colorLineEdit->clear();
    m_dirty = true;
}

void SportsDialog::slotSave()
{
    QTreeWidgetItem *item = ui->sportsTreeWidget->currentItem();
    // The default behaviour is "Add".
    if (!item) slotAdd();
    QString name = ui->nameLineEdit->text();
    QColor color = QColor(ui->colorLineEdit->text());
    if (name.isEmpty()) return;
    for (int i = 0; i < ui->sportsTreeWidget->topLevelItemCount(); ++i) {
        if (name == ui->sportsTreeWidget->topLevelItem(i)->text(COL_NAME) &&
            ui->colorLineEdit->text() == ui->sportsTreeWidget->topLevelItem(i)->text(COL_COLOR)) {
            ui->nameLineEdit->clear();
            ui->colorLineEdit->clear();
            return;
        }
    }
    int sportId = -1;
    for (int i = 0; i < m_sports->size(); ++i) {
        if (m_sports->at(i)->getName() == item->text(COL_NAME)) {
            sportId = m_sports->at(i)->getId();
            m_sports->at(i)->setName(name);
            m_sports->at(i)->setColor(color);
            break;
        }
    }
    if (sportId >= 0) {
        for (const auto& exercise : m_exercises) {
            if (exercise->getSport() == sportId) {
                m_modifiedExercises.append(exercise->getId());
            }
        }
    }
    item->setText(COL_NAME, name);
    item->setText(COL_COLOR, color.name());
    for (int col = 0; col < item->columnCount(); ++col)
        item->setBackground(col, QBrush(color));
    m_dirty = true;
}

void SportsDialog::slotSetNameText(QTreeWidgetItem *item, int)
{
    ui->nameLineEdit->setText(item->text(COL_NAME));
    ui->colorLineEdit->setText(item->text(COL_COLOR));
}
