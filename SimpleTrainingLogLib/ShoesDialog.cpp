#include <QMessageBox>

#include "DataHandler.h"
#include "ShoesDialog.h"
#include "ui_ShoesDialog.h"

ShoesDialog::ShoesDialog(QWidget *parent, DataHandler *dataHandler) :
    QDialog(parent), ui(new Ui::ShoesDialog), m_dirty(false), dataHandler(dataHandler)
{
    ui->setupUi(this);

    QStringList headerLabels("#");
    headerLabels << "Name" << "Buy" << "Running Distance" << "Comment";
    ui->shoesTreeWidget->setHeaderLabels(headerLabels);

    // TODO: Set column width to content and don't depend on sport identifiers
    // (e.g. matching shoe identifier and running).
    for (int i = 0; i < dataHandler->shoes.size(); ++i) {
        Shoe *shoe = dataHandler->shoes.at(i);
        size_t running_distance = 0;
        for (const auto& exercise : dataHandler->exercises) {
            if (exercise->getShoe() == shoe->getId() && exercise->getSport() == 0) {
                running_distance += exercise->getDistance();
            }
        }
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(COL_ID, QString::number(shoe->getId()));
        item->setText(COL_NAME, shoe->getName());
        item->setText(COL_BUY, QString::number(shoe->getBuy()));
        item->setText(COL_RUNNING_DISTANCE, QString::number(running_distance));
        item->setText(COL_COMMENT, shoe->getComment());
        ui->shoesTreeWidget->insertTopLevelItem(i, item);
        ui->shoesTreeWidget->setColumnWidth(COL_ID, 56);
    }
}

ShoesDialog::~ShoesDialog()
{
    delete ui;
}


void ShoesDialog::slotOk()
{
    accept();
}

void ShoesDialog::slotCancel()
{
    reject();
}

void ShoesDialog::slotAdd()
{
    QString name = ui->nameLineEdit->text();
    QString comment = ui->commentTextEdit->toPlainText();
    int buy = (ui->buyLineEdit->text()).toInt();
    if (name.isEmpty()) return;
    for (int i = 0; i < ui->shoesTreeWidget->topLevelItemCount(); ++i) {
        if (name == ui->shoesTreeWidget->topLevelItem(i)->text(COL_NAME)) {
            ui->nameLineEdit->clear();
            ui->buyLineEdit->clear();
            ui->commentTextEdit->clear();
            return;
        }
    }
    Shoe *shoe = new Shoe(m_shoes->size(), name, buy, comment);
    m_shoes->append(shoe);
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(COL_ID, QString::number(shoe->getId()));
    item->setText(COL_NAME, shoe->getName());
    item->setText(COL_BUY, QString::number(shoe->getBuy()));
    item->setText(COL_RUNNING_DISTANCE, "0");
    item->setText(COL_COMMENT, shoe->getComment());
    ui->shoesTreeWidget->insertTopLevelItem(ui->shoesTreeWidget->topLevelItemCount(), item);
    ui->nameLineEdit->clear();
    ui->buyLineEdit->clear();
    ui->commentTextEdit->clear();
    m_dirty = true;
}

void ShoesDialog::slotRemove()
{
    QTreeWidgetItem *item = ui->shoesTreeWidget->currentItem();
    if (!item) return;
    int id = (item->text(COL_ID)).toInt();
    QString name = item->text(COL_NAME);
    int count = 0;
    for (const auto& exercise : m_exercises) {
        if (exercise->getShoe() == id) {
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
        if (exercise->getShoe() == id) {
            m_removedExercises.append(exercise->getId());
        }
    }
    dataHandler->removeShoeById(id);
    delete ui->shoesTreeWidget->takeTopLevelItem(ui->shoesTreeWidget->indexOfTopLevelItem(item));
    ui->nameLineEdit->clear();
    ui->buyLineEdit->clear();
    ui->commentTextEdit->clear();
    m_dirty = true;
}

void ShoesDialog::slotSave()
{
    QTreeWidgetItem *item = ui->shoesTreeWidget->currentItem();
    if (!item) slotAdd();
    QString name = ui->nameLineEdit->text();
    QString comment = ui->commentTextEdit->toPlainText();
    int buy = (ui->buyLineEdit->text()).toInt();
    if (name.isEmpty()) return;
    int shoeId = -1;
    for (int i = 0; i < m_shoes->size(); ++i) {
        if (m_shoes->at(i)->getName() == item->text(COL_NAME)) {
            shoeId = m_shoes->at(i)->getId();
            m_shoes->at(i)->setName(name);
            m_shoes->at(i)->setBuy(buy);
            m_shoes->at(i)->setComment(comment);
            ui->savePushButton->setEnabled(false);
            break;
        }
    }
    if (shoeId >= 0) {
        for (const auto& exercise : m_exercises) {
            if (exercise->getShoe() == shoeId) {
                m_modifiedExercises.append(exercise->getId());
            }
        }
    }
    item->setText(COL_NAME, name);
    item->setText(COL_BUY, QString::number(buy));
    item->setText(COL_COMMENT, comment);
    m_dirty = true;
}

void ShoesDialog::slotSetFieldsText(QTreeWidgetItem *item, int)
{
    ui->nameLineEdit->setText(item->text(COL_NAME));
    ui->nameLineEdit->setCursorPosition(0);
    ui->buyLineEdit->setText(item->text(COL_BUY));
    ui->buyLineEdit->setCursorPosition(0);
    ui->commentTextEdit->setText(item->text(COL_COMMENT));

    slotDisableEnablePushButtons();
}

void ShoesDialog::slotDisableEnablePushButtons()
{
    bool true_to_enable = true;
    if (ui->nameLineEdit->text().length() == 0 &&
        ui->buyLineEdit->text().length() == 0 &&
        ui->commentTextEdit->toPlainText().length() == 0) {
        true_to_enable = false;
    }
    ui->addPushButton->setEnabled(true_to_enable);
    ui->removePushButton->setEnabled(true_to_enable);
    ui->savePushButton->setEnabled(true_to_enable);
}
