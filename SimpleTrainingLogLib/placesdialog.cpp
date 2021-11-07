#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPainter>

#include "placesdialog.h"
#include "dataelements.h"

PlacesDialog::PlacesDialog(QWidget *parent, QList<Place*> *places,
    QList<Exercise*> *exercises) : QDialog(parent)
{
    m_places = places;
    m_exercises = exercises;
    m_dirty = false;

    m_nameLineEdit = new QLineEdit;
    m_placesTreeWidget = new QTreeWidget;

    QStringList headerLabels("#");
    headerLabels << "Name";
    m_placesTreeWidget->setHeaderLabels(headerLabels);

    for (int i = 0; i < places->size(); ++i) {
        // TODO Set column width to content.
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(COL_ID, QString::number(places->at(i)->getId()));
        item->setText(COL_NAME, places->at(i)->getName());
        m_placesTreeWidget->insertTopLevelItem(i, item);
        m_placesTreeWidget->setColumnWidth(COL_ID, 56);
    }

    QHBoxLayout *dataLayout = new QHBoxLayout;
    dataLayout->addWidget(new QLabel(tr("Name:")));
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
    leftLayout->addWidget(new OSMTileDrawer(this));
    //leftLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(m_placesTreeWidget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(mainLayout);
    layout->addLayout(buttonLayout);

    setLayout(layout);
    setWindowTitle(tr("Manage Places"));

    connect(m_okPushButton, SIGNAL(clicked()), this, SLOT(slotOk()));
    connect(m_addPushButton, SIGNAL(clicked()), this, SLOT(slotAdd()));
    connect(m_removePushButton, SIGNAL(clicked()), this, SLOT(slotRemove()));
    connect(m_savePushButton, SIGNAL(clicked()), this, SLOT(slotSave()));
    connect(m_cancelPushButton, SIGNAL(clicked()), this, SLOT(slotCancel()));
    connect(m_placesTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
        this, SLOT(slotSetNameText(QTreeWidgetItem *, int)));
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
    QString name = m_nameLineEdit->text();
    if (name.isEmpty()) return;
    for (int i = 0; i < m_placesTreeWidget->topLevelItemCount(); ++i) {
        if (name == m_placesTreeWidget->topLevelItem(i)->text(COL_NAME)) {
            m_nameLineEdit->setText("");
            return;
        }
    }
    Place *place = new Place(m_places->size(), name);
    m_places->append(place);
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(COL_ID, QString::number(place->getId()));
    item->setText(COL_NAME, place->getName());
    m_placesTreeWidget->insertTopLevelItem(m_placesTreeWidget->
        topLevelItemCount(), item);
    m_nameLineEdit->setText("");
    m_dirty = true;
}

void PlacesDialog::slotRemove()
{
    // TODO Place is optional.  Don't remove the affected exercises.
    QTreeWidgetItem *item = m_placesTreeWidget->currentItem();
    if (!item) return;
    QString name = item->text(COL_NAME);
    int count = 0;
    for (int i = 0; i < m_exercises->size(); ++i)
        if ((m_exercises->at(i)->getPlaces()).contains(getPlaceId(name)))
            ++count;
    if (count > 0) {
        int ret = QMessageBox::warning(this, tr("KTrainer"), tr("%1 is used "
            "by %2 exercises. All of these exercises will be removed. Are "
            "you sure?").arg(name).arg(count), QMessageBox::Yes |
            QMessageBox::Default, QMessageBox::No | QMessageBox::Escape);
        if (ret != QMessageBox::Yes)
            return;
    }
    for (int i = 0; i < m_exercises->size(); ++i)
        if ((m_exercises->at(i)->getPlaces()).contains(getPlaceId(name)))
            m_removedExercises.append(m_exercises->at(i)->getId());
    removePlaceById(getPlaceId(name));
    delete m_placesTreeWidget->takeTopLevelItem(m_placesTreeWidget->
        indexOfTopLevelItem(item));
    m_nameLineEdit->setText("");
    m_dirty = true;
}

void PlacesDialog::slotSave()
{
    QTreeWidgetItem *item = m_placesTreeWidget->currentItem();
    if (!item) slotAdd();
    QString name = m_nameLineEdit->text();
    if (name.isEmpty()) return;
    for (int i = 0; i < m_placesTreeWidget->topLevelItemCount(); ++i) {
        if (name == m_placesTreeWidget->topLevelItem(i)->text(COL_NAME)) {
            m_nameLineEdit->setText("");
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
        for (int i = 0; i < m_exercises->size(); ++i)
            if ((m_exercises->at(i)->getPlaces()).contains(placeId))
                m_modifiedExercises.append(m_exercises->at(i)->getId());
    }
    item->setText(COL_NAME, name);
    m_dirty = true;
}

void PlacesDialog::slotSetNameText(QTreeWidgetItem *item,
    int column __attribute__((unused)))
{
    m_nameLineEdit->setText(item->text(COL_NAME));
}

void OSMTileDrawer::paintEvent(QPaintEvent *)
{
  QPainter p(this);
  QPixmap pm(":Images/osmdemo.png");
  QImage genimg(256,256,QImage::Format_RGB32);
  //genimg.create(256,256);
  for (int i=0; i<655536; i++ )                     // build color table
      genimg.setColor( i, qRgb(i,0,0) );
  QImage image = pm.toImage();
  qDebug() << image.width() ;
  p.drawImage(0, 0, genimg, 0, 0, 256, 256);
  qDebug() << "Called\n";
}
