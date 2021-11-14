#include <QMessageBox>

#include "DataElements.h"
#include "ShoesDialog.h"
#include "ui_ShoesDialog.h"

ShoesDialog::ShoesDialog(QWidget *parent, QList<Shoe*> *shoes, QList<Exercise*> *exercises) :
    QDialog(parent), ui(new Ui::ShoesDialog), m_shoes(shoes), m_exercises(exercises), m_dirty(false)
{
    ui->setupUi(this);

    QStringList headerLabels("#");
    headerLabels << "Name" << "Buy" << "Running Distance" << "Comment";
    ui->shoesTreeWidget->setHeaderLabels(headerLabels);

    // TODO Set column width to content and don't depend on sport identifiers
    // (e.g. matching shoe identifier and running).
    for (size_t i = 0; i < static_cast<size_t>(shoes->size()); ++i) {
        Shoe *shoe = shoes->at(i);
        size_t running_distance = 0;
        for (int j = 0; j < exercises->size(); ++j) {
            Exercise *exercise = exercises->at(j);
            if (exercise->getShoe() == shoe->getId() && exercise->getSport() == 0)
                running_distance += exercise->getDistance();
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
            ui->nameLineEdit->setText("");
            ui->buyLineEdit->setText("");
            ui->commentTextEdit->setText("");
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
    ui->nameLineEdit->setText("");
    ui->buyLineEdit->setText("");
    ui->commentTextEdit->setText("");
    m_dirty = true;
}

void ShoesDialog::slotRemove()
{
    QTreeWidgetItem *item = ui->shoesTreeWidget->currentItem();
    if (!item) return;
    QString name = item->text(COL_NAME);
    int count = 0;
    for (int i = 0; i < m_exercises->size(); ++i)
        if (m_exercises->at(i)->getShoe() == getShoeId(name)) ++count;
    if (count > 0) {
        int ret = QMessageBox::warning(this, tr("SimpleTrainingLog"), tr("%1 is used "
            "by %2 exercises. All of these exercises will be removed. Are "
            "you sure?").arg(name).arg(count), QMessageBox::Yes |
            QMessageBox::Default, QMessageBox::No | QMessageBox::Escape);
        if (ret != QMessageBox::Yes)
            return;
    }
    for (int i = 0; i < m_exercises->size(); ++i)
        if (m_exercises->at(i)->getShoe() == getShoeId(name))
            m_removedExercises.append(m_exercises->at(i)->getId());
    removeShoeById(getShoeId(name));
    delete ui->shoesTreeWidget->takeTopLevelItem(ui->shoesTreeWidget->indexOfTopLevelItem(item));
    ui->nameLineEdit->setText("");
    ui->buyLineEdit->setText("");
    ui->commentTextEdit->setText("");
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
        for (int i = 0; i < m_exercises->size(); ++i)
            if (m_exercises->at(i)->getShoe() == shoeId)
                m_modifiedExercises.append(m_exercises->at(i)->getId());
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
