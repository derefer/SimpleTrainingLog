#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QColorDialog>

#include "sportsdialog.h"
#include "dataelements.h"

#define LEN_SPORT 49
#define LEN_COLOR 7

SportsDialog::SportsDialog(QWidget *parent, QList<Sport*> *sports,
    QList<Exercise*> *exercises) : QDialog(parent)
{
    m_sports = sports;
    m_exercises = exercises;
    m_dirty = false;

    m_nameLabel = new QLabel(tr("Name:"));
    m_nameLineEdit = new QLineEdit;
    m_nameLineEdit->setMaxLength(LEN_SPORT);
    m_sportsTreeWidget = new QTreeWidget;
    m_colorLabel = new QLabel(tr("Color:"));
    m_colorLineEdit = new QLineEdit;
    m_colorLineEdit->setMaxLength(LEN_COLOR);
    m_colorPushButton = new QPushButton(tr("C&hoose"));

    QStringList headerLabels("#");
    headerLabels << "Name" << "Color";
    m_sportsTreeWidget->setHeaderLabels(headerLabels);

    for (int i = 0; i < sports->size(); ++i) {
        // TODO Set column width to content.
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(COL_ID, QString::number(sports->at(i)->getId()));
        item->setText(COL_NAME, sports->at(i)->getName());
        QColor color = sports->at(i)->getColor();
        item->setText(COL_COLOR, (color.name()));
        for (int j = 0; j < item->columnCount(); ++j)
            item->setBackground(j, QBrush(color));
        m_sportsTreeWidget->insertTopLevelItem(i, item);
        m_sportsTreeWidget->setColumnWidth(COL_ID, 56);
    }

    m_okPushButton = new QPushButton(tr("&OK"));
    m_addPushButton = new QPushButton(tr("&Add"));
    m_removePushButton = new QPushButton(tr("&Remove"));
    m_savePushButton = new QPushButton(tr("&Save"));
    m_cancelPushButton = new QPushButton(tr("&Cancel"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_okPushButton);
    buttonLayout->addWidget(m_addPushButton);
    buttonLayout->addWidget(m_removePushButton);
    buttonLayout->addWidget(m_savePushButton);
    buttonLayout->addWidget(m_cancelPushButton);

    QHBoxLayout *colorFieldsLayout = new QHBoxLayout;
    colorFieldsLayout->addWidget(m_colorLineEdit);
    colorFieldsLayout->addWidget(m_colorPushButton);
    QGridLayout *leftLayout = new QGridLayout;
    leftLayout->addWidget(m_nameLabel, 0, 0);
    leftLayout->addWidget(m_colorLabel, 1, 0);
    leftLayout->addWidget(m_nameLineEdit, 0, 1);
    leftLayout->addLayout(colorFieldsLayout, 1, 1);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(m_sportsTreeWidget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(mainLayout);
    layout->addLayout(buttonLayout);

    setLayout(layout);
    setWindowTitle(tr("Manage Sports"));

    connect(m_colorPushButton, SIGNAL(clicked()), this, SLOT(slotSetColor()));
    connect(m_okPushButton, SIGNAL(clicked()), this, SLOT(slotOk()));
    connect(m_addPushButton, SIGNAL(clicked()), this, SLOT(slotAdd()));
    connect(m_removePushButton, SIGNAL(clicked()), this, SLOT(slotRemove()));
    connect(m_savePushButton, SIGNAL(clicked()), this, SLOT(slotSave()));
    connect(m_cancelPushButton, SIGNAL(clicked()), this, SLOT(slotCancel()));
    connect(m_sportsTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
        this, SLOT(slotSetNameText(QTreeWidgetItem *, int)));
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
    m_colorLineEdit->setText(color.name());
}

// Add a new sport to the list of sports.  The QTextLineEdit is always
// cleared.  TODO: Better error handling.
void SportsDialog::slotAdd()
{
    QString name = m_nameLineEdit->text();
    QColor color = QColor(m_colorLineEdit->text());
    if (name.isEmpty() || !color.isValid()) return;
    for (int i = 0; i < m_sportsTreeWidget->topLevelItemCount(); ++i) {
        if (name == m_sportsTreeWidget->topLevelItem(i)->text(COL_NAME)) {
            m_nameLineEdit->setText("");
            m_colorLineEdit->setText("");
            return;
        }
    }
    Sport *sport = new Sport(m_sports->size(), name, color);
    m_sports->append(sport);
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(COL_ID, QString::number(sport->getId()));
    item->setText(COL_NAME, name);
    item->setText(COL_COLOR, color.name());
    m_sportsTreeWidget->insertTopLevelItem(m_sportsTreeWidget->
        topLevelItemCount(), item);
    m_nameLineEdit->setText("");
    m_colorLineEdit->setText("");
    for (int col = 0; col < item->columnCount(); ++col)
        item->setBackground(col, QBrush(color));
    m_dirty = true;  // Activate the "Save" button.
}

void SportsDialog::slotRemove()
{
    QTreeWidgetItem *item = m_sportsTreeWidget->currentItem();
    if (!item) return;
    QString name = item->text(COL_NAME);
    int count = 0;
    for (int i = 0; i < m_exercises->size(); ++i)
        if (m_exercises->at(i)->getSport() == getSportId(name))
            ++count;
    // Be user friendly and warn.  The QTreeWidget in the background will be
    // cleared after the dialog is closed.  It's not done here.
    if (count > 0) {
        int ret = QMessageBox::warning(this, tr("KTrainer"), tr("%1 is used "
            "by %2 exercises. All of these exercises will be removed. Are "
            "you sure?").arg(name).arg(count), QMessageBox::Yes |
            QMessageBox::Default, QMessageBox::No | QMessageBox::Escape);
        if (ret != QMessageBox::Yes)
            return;
    }
    for (int i = 0; i < m_exercises->size(); ++i)
        if (m_exercises->at(i)->getSport() == getSportId(name))
            m_removedExercises.append(m_exercises->at(i)->getId());
    removeSportById(getSportId(name));
    delete m_sportsTreeWidget->takeTopLevelItem(m_sportsTreeWidget->
        indexOfTopLevelItem(item));
    m_nameLineEdit->setText("");
    m_colorLineEdit->setText("");
    m_dirty = true;
}

void SportsDialog::slotSave()
{
    QTreeWidgetItem *item = m_sportsTreeWidget->currentItem();
    // The default behaviour is "Add".
    if (!item) slotAdd();
    QString name = m_nameLineEdit->text();
    QColor color = QColor(m_colorLineEdit->text());
    if (name.isEmpty()) return;
    for (int i = 0; i < m_sportsTreeWidget->topLevelItemCount(); ++i) {
        if (name == m_sportsTreeWidget->topLevelItem(i)->text(COL_NAME) &&
            m_colorLineEdit->text() == m_sportsTreeWidget->topLevelItem(i)->text(COL_COLOR)) {
            m_nameLineEdit->setText("");
            m_colorLineEdit->setText("");
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
    if (sportId >= 0)
        for (int i = 0; i < m_exercises->size(); ++i)
            if (m_exercises->at(i)->getSport() == sportId)
                m_modifiedExercises.append(m_exercises->at(i)->getId());
    item->setText(COL_NAME, name);
    item->setText(COL_COLOR, color.name());
    for (int col = 0; col < item->columnCount(); ++col)
        item->setBackground(col, QBrush(color));
    m_dirty = true;
}

void SportsDialog::slotSetNameText(QTreeWidgetItem *item,
    int column __attribute__((unused)))
{
    m_nameLineEdit->setText(item->text(COL_NAME));
    m_colorLineEdit->setText(item->text(COL_COLOR));
}
