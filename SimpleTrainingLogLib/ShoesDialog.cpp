#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include "ShoesDialog.h"
#include "DataElements.h"

ShoesDialog::ShoesDialog(QWidget *parent, QList<Shoe*> *shoes,
  QList<Exercise*> *exercises) : QDialog(parent)
{
  m_shoes = shoes;
  m_exercises = exercises;
  m_dirty = false;

  m_nameLabel = new QLabel(tr("Name:"));
  m_buyLabel = new QLabel(tr("Buy:"));
  m_commentLabel = new QLabel(tr("Comment:"));
  m_nameLineEdit = new QLineEdit;
  m_buyLineEdit = new QLineEdit;
  m_commentTextEdit = new QTextEdit;
  m_shoesTreeWidget = new QTreeWidget;

  QStringList headerLabels("#");
  headerLabels << "Name" << "Buy" << "Running Distance" << "Comment";
  m_shoesTreeWidget->setHeaderLabels(headerLabels);

  // TODO Set column width to content and don't depend on sport identifiers
  // (e.g. matching shoe identifier and running).
  for (size_t i = 0; i < static_cast<size_t>(shoes->size()); ++i) {
    Shoe *shoe = shoes->at(i);
    size_t running_distance = 0;
    for (int j = 0; j < exercises->size(); ++j) {
      Exercise *exercise = exercises->at(j);
      if (exercise->getShoe() == shoe->getId() &&
          exercise->getSport() == 0)
        running_distance += exercise->getDistance();
    }
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(COL_ID, QString::number(shoe->getId()));
    item->setText(COL_NAME, shoe->getName());
    item->setText(COL_BUY, QString::number(shoe->getBuy()));
    item->setText(COL_RUNNING_DISTANCE, QString::number(running_distance));
    item->setText(COL_COMMENT, shoe->getComment());
    m_shoesTreeWidget->insertTopLevelItem(i, item);
    m_shoesTreeWidget->setColumnWidth(COL_ID, 56);
  }

  QGridLayout *dataLayout = new QGridLayout();
  dataLayout->addWidget(m_nameLabel, 0, 0);
  dataLayout->addWidget(m_nameLineEdit, 0, 1);
  dataLayout->addWidget(m_buyLabel, 1, 0);
  dataLayout->addWidget(m_buyLineEdit, 1, 1);
  dataLayout->addWidget(m_commentLabel, 2, 0);
  dataLayout->addWidget(m_commentTextEdit, 2, 1);

  m_okPushButton = new QPushButton(tr("&OK"));
  m_addPushButton = new QPushButton(tr("&Add"));
  m_addPushButton->setEnabled(false);
  m_removePushButton = new QPushButton(tr("&Remove"));
  m_removePushButton->setEnabled(false);
  m_savePushButton = new QPushButton(tr("&Save"));
  m_savePushButton->setEnabled(false);
  m_cancelPushButton = new QPushButton(tr("&Cancel"));

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(m_okPushButton);
  buttonLayout->addWidget(m_addPushButton);
  buttonLayout->addWidget(m_removePushButton);
  buttonLayout->addWidget(m_savePushButton);
  buttonLayout->addWidget(m_cancelPushButton);

  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->addLayout(dataLayout);
  mainLayout->addWidget(m_shoesTreeWidget);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(mainLayout);
  layout->addLayout(buttonLayout);

  setLayout(layout);
  setWindowTitle(tr("Manage Shoes"));

  connect(m_okPushButton, SIGNAL(clicked()), this, SLOT(slotOk()));
  connect(m_addPushButton, SIGNAL(clicked()), this, SLOT(slotAdd()));
  connect(m_removePushButton, SIGNAL(clicked()), this, SLOT(slotRemove()));
  connect(m_savePushButton, SIGNAL(clicked()), this, SLOT(slotSave()));
  connect(m_cancelPushButton, SIGNAL(clicked()), this, SLOT(slotCancel()));
  connect(m_shoesTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
          this, SLOT(slotSetFieldsText(QTreeWidgetItem *, int)));
  // All actions are possible.
  connect(m_nameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(slotDisableEnablePushButtons()));
  connect(m_buyLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(slotDisableEnablePushButtons()));
  connect(m_commentTextEdit, SIGNAL(textChanged()), this, SLOT(slotDisableEnablePushButtons()));
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
  QString name = m_nameLineEdit->text();
  QString comment = m_commentTextEdit->toPlainText();
  int buy = (m_buyLineEdit->text()).toInt();
  if (name.isEmpty()) return;
  for (int i = 0; i < m_shoesTreeWidget->topLevelItemCount(); ++i) {
    if (name == m_shoesTreeWidget->topLevelItem(i)->text(COL_NAME)) {
      m_nameLineEdit->setText("");
      m_buyLineEdit->setText("");
      m_commentTextEdit->setText("");
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
  m_shoesTreeWidget->insertTopLevelItem(m_shoesTreeWidget->
    topLevelItemCount(), item);
  m_nameLineEdit->setText("");
  m_buyLineEdit->setText("");
  m_commentTextEdit->setText("");
  m_dirty = true;
}

void ShoesDialog::slotRemove()
{
  QTreeWidgetItem *item = m_shoesTreeWidget->currentItem();
  if (!item) return;
  QString name = item->text(COL_NAME);
  int count = 0;
  for (int i = 0; i < m_exercises->size(); ++i)
    if (m_exercises->at(i)->getShoe() == getShoeId(name))
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
    if (m_exercises->at(i)->getShoe() == getShoeId(name))
      m_removedExercises.append(m_exercises->at(i)->getId());
  removeShoeById(getShoeId(name));
  delete m_shoesTreeWidget->takeTopLevelItem(m_shoesTreeWidget->
    indexOfTopLevelItem(item));
  m_nameLineEdit->setText("");
  m_buyLineEdit->setText("");
  m_commentTextEdit->setText("");
  m_dirty = true;
}

void ShoesDialog::slotSave()
{
  QTreeWidgetItem *item = m_shoesTreeWidget->currentItem();
  if (!item) slotAdd();
  QString name = m_nameLineEdit->text();
  QString comment = m_commentTextEdit->toPlainText();
  int buy = (m_buyLineEdit->text()).toInt();
  if (name.isEmpty()) return;
  int shoeId = -1;
  for (int i = 0; i < m_shoes->size(); ++i) {
    if (m_shoes->at(i)->getName() == item->text(COL_NAME)) {
      shoeId = m_shoes->at(i)->getId();
      m_shoes->at(i)->setName(name);
      m_shoes->at(i)->setBuy(buy);
      m_shoes->at(i)->setComment(comment);
      m_savePushButton->setEnabled(false);
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

void ShoesDialog::slotSetFieldsText(QTreeWidgetItem *item, int /*column*/)
{
  m_nameLineEdit->setText(item->text(COL_NAME));
  m_nameLineEdit->setCursorPosition(0);
  m_buyLineEdit->setText(item->text(COL_BUY));
  m_buyLineEdit->setCursorPosition(0);
  m_commentTextEdit->setText(item->text(COL_COMMENT));

  slotDisableEnablePushButtons();
}

void ShoesDialog::slotDisableEnablePushButtons()
{
  bool true_to_enable = true;
  if (m_nameLineEdit->text().length() == 0 &&
      m_buyLineEdit->text().length() == 0 &&
      m_commentTextEdit->toPlainText().length() == 0) {
    true_to_enable = false;
  }
  m_addPushButton->setEnabled(true_to_enable);
  m_removePushButton->setEnabled(true_to_enable);
  m_savePushButton->setEnabled(true_to_enable);
}
