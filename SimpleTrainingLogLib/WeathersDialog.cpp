#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include "WeathersDialog.h"
#include "DataElements.h"

WeathersDialog::WeathersDialog(QWidget *parent, QList<Weather*> *weathers, QList<Exercise*> *exercises) :
    QDialog(parent), m_weathers(weathers), m_exercises(exercises), m_dirty(false)
{
    m_nameLabel = new QLabel(tr("Name:"));
    m_nameLineEdit = new QLineEdit;
    m_weathersTreeWidget = new QTreeWidget;

    QStringList headerLabels("#");
    headerLabels << "Name";
    m_weathersTreeWidget->setHeaderLabels(headerLabels);

    for (int i = 0; i < weathers->size(); ++i) {
        // TODO Set column width to content.
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(COL_ID, QString::number(weathers->at(i)->getId()));
        item->setText(COL_NAME, weathers->at(i)->getName());
        m_weathersTreeWidget->insertTopLevelItem(i, item);
        m_weathersTreeWidget->setColumnWidth(COL_ID, 56);
    }

    QHBoxLayout *dataLayout = new QHBoxLayout;
    dataLayout->addWidget(m_nameLabel);
    dataLayout->addWidget(m_nameLineEdit);

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

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(dataLayout);
    leftLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(m_weathersTreeWidget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(mainLayout);
    layout->addLayout(buttonLayout);

    setLayout(layout);
    setWindowTitle(tr("Manage Weathers"));

    connect(m_okPushButton, SIGNAL(clicked()), this, SLOT(slotOk()));
    connect(m_addPushButton, SIGNAL(clicked()), this, SLOT(slotAdd()));
    connect(m_removePushButton, SIGNAL(clicked()), this, SLOT(slotRemove()));
    connect(m_savePushButton, SIGNAL(clicked()), this, SLOT(slotSave()));
    connect(m_cancelPushButton, SIGNAL(clicked()), this, SLOT(slotCancel()));
    connect(m_weathersTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
        this, SLOT(slotSetNameText(QTreeWidgetItem *, int)));
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
    QString name = m_nameLineEdit->text();
    if (name.isEmpty()) return;
    for (int i = 0; i < m_weathersTreeWidget->topLevelItemCount(); ++i) {
        if (name == m_weathersTreeWidget->topLevelItem(i)->text(COL_NAME)) {
            m_nameLineEdit->setText("");
            return;
        }
    }
    Weather *weather = new Weather(m_weathers->size(), name);
    m_weathers->append(weather);
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(COL_ID, QString::number(weather->getId()));
    item->setText(COL_NAME, weather->getName());
    m_weathersTreeWidget->insertTopLevelItem(m_weathersTreeWidget->
        topLevelItemCount(), item);
    m_nameLineEdit->setText("");
    m_dirty = true;  // Activate the "Save" button.
}

void WeathersDialog::slotRemove()
{
    // TODO Weather is optional.  Don't remove the affected exercises.
    QTreeWidgetItem *item = m_weathersTreeWidget->currentItem();
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
    delete m_weathersTreeWidget->takeTopLevelItem(m_weathersTreeWidget->
        indexOfTopLevelItem(item));
    m_nameLineEdit->setText("");
    m_dirty = true;
}

void WeathersDialog::slotSave()
{
    QTreeWidgetItem *item = m_weathersTreeWidget->currentItem();
    if (!item) slotAdd();
    QString name = m_nameLineEdit->text();
    if (name.isEmpty()) return;
    for (int i = 0; i < m_weathersTreeWidget->topLevelItemCount(); ++i) {
        if (name == m_weathersTreeWidget->topLevelItem(i)->text(COL_NAME)) {
            m_nameLineEdit->setText("");
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
    m_nameLineEdit->setText(item->text(COL_NAME));
}
