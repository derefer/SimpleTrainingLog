#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QToolBar>

#include "ExerciseTable.h"
#include "ExportDialog.h"
#include "NewExerciseDialog.h"
#include "PlacesDialog.h"
#include "SettingsDialog.h"
#include "ShoesDialog.h"
#include "SimpleTrainingLogMainWindow.h"
#include "SportsDialog.h"
#include "StatisticsHandler.h"
#include "ui_SimpleTrainingLogMainWindow.h"
#include "WeathersDialog.h"

SimpleTrainingLogMainWindow::SimpleTrainingLogMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimpleTrainingLogMainWindow),
    //m_ftp(nullptr),
    m_dirty(false)
{
    ui->setupUi(this);

    createToolBars();
    createStatusBar();

    m_curConfig = DEFAULT_CONFIG;
    m_curLog = DEFAULT_LOG;
    m_curHost = DEFAULT_HOST;
    m_curPort = QString("%1").arg(DEFAULT_PORT);
    m_curPath = DEFAULT_PATH;
    m_curUser = DEFAULT_USER;

    readSettings();

    m_exerciseTable = new ExerciseTable(this, &dataHandler);
    m_statisticsHandler = new StatisticsHandler(this, &dataHandler);
    m_mainTabWidget = new QTabWidget();
    m_mainTabWidget->addTab(m_exerciseTable, tr("Summary"));
    m_mainTabWidget->addTab(m_statisticsHandler, tr("Statistics"));
    setCentralWidget(m_mainTabWidget);

    loadDatabase();

    // The SIGNAL and the SLOT must have the same signature. Any modification
    // of the table will trigger setDirty.
    m_exerciseTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_exerciseTable, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(contextMenuForTable(const QPoint &)));
    connect(m_exerciseTable, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(editExercise(QTreeWidgetItem *, int)));
}

SimpleTrainingLogMainWindow::~SimpleTrainingLogMainWindow()
{
    /*if (m_ftp)
        delete m_ftp;*/

    clear();

    delete ui;
}

void SimpleTrainingLogMainWindow::setDirty(int, int)
{
    ui->action_Save->setEnabled(true);
    m_dirty = true;
}

void SimpleTrainingLogMainWindow::deleteExercise()
{
    m_exerciseTable->removeCurrentExercise();
    setDirty();
}

void SimpleTrainingLogMainWindow::viewExercise()
{
    NewExerciseDialog dialog(this, "View Exercise", &dataHandler);
    dialog.addSportStrings(dataHandler.getSportStrings());
    dialog.addShoeStrings(dataHandler.getShoeStrings());
    dialog.addPlaceStrings(dataHandler.getPlaceStrings());
    dialog.addWeatherStrings(dataHandler.getWeatherStrings());
    dialog.setReadOnly();

    Exercise *e = m_exerciseTable->getCurrentExercise();
    dialog.setDate((e->getDate()).split("-"));
    dialog.setTime(e->getTime());
    dialog.setDistance(QString::number(e->getDistance()));
    dialog.setDuration(e->getDuration());
    dialog.setPulse(e->getMaxPulse(), e->getAvgPulse());
    dialog.setCalories(e->getCal(), e->getFat());
    dialog.setSport(dataHandler.getSportString(e->getSport()));
    const auto& placesById = dataHandler.getPlacesById(e->getPlaces());
    QStringList placesStringList;
    for (const auto place : placesById) {
        placesStringList << place->getName();
    }
    dialog.setPlaces(placesStringList);
    dialog.setShoe(dataHandler.getShoeString(e->getShoe()));
    const auto& weathersById = dataHandler.getWeathersById(e->getWeathers());
    QStringList weathersStringList;
    for (const auto weather : weathersById) {
        weathersStringList << weather->getName();
    }
    dialog.setWeathers(weathersStringList);
    dialog.setComment(e->getComment());

    dialog.exec();
}

void SimpleTrainingLogMainWindow::editExercise(QTreeWidgetItem *, int)
{
    NewExerciseDialog dialog(this, "Edit Exercise", &dataHandler);
    dialog.addSportStrings(dataHandler.getSportStrings());
    dialog.addShoeStrings(dataHandler.getShoeStrings());
    dialog.addPlaceStrings(dataHandler.getPlaceStrings());
    dialog.addWeatherStrings(dataHandler.getWeatherStrings());

    Exercise *e = m_exerciseTable->getCurrentExercise();
    dialog.setDate((e->getDate()).split("-"));
    dialog.setTime(e->getTime());
    dialog.setDistance(QString::number(e->getDistance()));
    dialog.setDuration(e->getDuration());
    dialog.setPulse(e->getMaxPulse(), e->getAvgPulse());
    dialog.setCalories(e->getCal(), e->getFat());
    dialog.setSport(dataHandler.getSportString(e->getSport()));
    const auto& placesById = dataHandler.getPlacesById(e->getPlaces());
    QStringList placesStringList;
    for (const auto place : placesById) {
        placesStringList << place->getName();
    }
    dialog.setPlaces(placesStringList);
    dialog.setShoe(dataHandler.getShoeString(e->getShoe()));
    const auto& weathersById = dataHandler.getWeathersById(e->getWeathers());
    QStringList weathersStringList;
    for (const auto weather : weathersById) {
        weathersStringList << weather->getName();
    }
    dialog.setWeathers(weathersStringList);
    dialog.setComment(e->getComment());

    if (dialog.exec()) {
        e->setDate(dialog.getDate());
        e->setTime(dialog.getTime());
        e->setDistance(dialog.getDistance().toInt());
        e->setDuration(dialog.getDuration());
        QStringList pulseList = dialog.getPulse().split(PULSE_SEPARATOR);
        e->setMaxPulse(pulseList[0].toInt());
        e->setAvgPulse(pulseList[1].toInt());
        QStringList caloriesList = dialog.getCalories().split(PULSE_SEPARATOR);
        e->setCal(caloriesList[0].toInt());
        e->setFat(caloriesList[1].toInt());
        e->setSport(dataHandler.getSportId(dialog.getSport()));
        e->clearPlaces();
        QStringList placeList = dialog.getPlaceList();
        for (int i = 0; i < placeList.size(); ++i)
            e->addPlace(dataHandler.getPlaceId(placeList.at(i)));
        e->setShoe(dataHandler.getShoeId(dialog.getShoe()));
        e->setComment(dialog.getComment());
        e->clearWeathers();
        QStringList weatherList = dialog.getWeatherList();
        for (int i = 0; i < weatherList.size(); ++i)
            e->addWeather(dataHandler.getWeatherId(weatherList.at(i)));
        m_exerciseTable->updateExercise(e->getId());
        m_statisticsHandler->calculateStatistics();
        setDirty();
    }
}

// It seems to be better to do it here instead of using mousePressEvent() in
// QTreeWidget.
void SimpleTrainingLogMainWindow::contextMenuForTable(const QPoint& pos)
{
    QTreeWidgetItem *item = nullptr;
    item = m_exerciseTable->itemAt(pos);
    if (item != nullptr) {
        QMenu menu(m_exerciseTable);
        QAction *viewAct = new QAction(tr("&View"), m_exerciseTable);
        viewAct->setStatusTip(tr("View the selected exercise"));
        connect(viewAct, SIGNAL(triggered()), this, SLOT(viewExercise()));
        QAction *editAct = new QAction(tr("&Edit"), m_exerciseTable);
        editAct->setStatusTip(tr("Edit the selected exercise"));
        connect(editAct, SIGNAL(triggered()), this, SLOT(editExercise()));
        QAction *deleteAct = new QAction(tr("&Delete"), m_exerciseTable);
        deleteAct->setStatusTip(tr("Delete the selected exercise"));
        connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteExercise()));
        menu.addAction(viewAct);
        menu.addSeparator();
        menu.addAction(editAct);
        menu.addAction(deleteAct);
        menu.exec(m_exerciseTable->mapToGlobal(pos));
    }
}

void SimpleTrainingLogMainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void SimpleTrainingLogMainWindow::newExercise()
{
    if (dataHandler.sports.empty()) {
        // TODO: Initially, when there is no database, create a preset instead.
        QMessageBox::warning(this, tr("SimpleTrainingLog"), tr("Please define at least one Sport to create an exercise."));
        return;
    }

    NewExerciseDialog dialog(this, "New Exercise", &dataHandler);
    dialog.addSportStrings(dataHandler.getSportStrings());
    dialog.addShoeStrings(dataHandler.getShoeStrings());
    dialog.addPlaceStrings(dataHandler.getPlaceStrings());
    dialog.addWeatherStrings(dataHandler.getWeatherStrings());

    if (dialog.exec()) {
        Exercise *e = new Exercise(dataHandler.exercises.size());
        e->setDate(dialog.getDate());
        e->setTime(dialog.getTime());
        e->setDistance(dialog.getDistance().toInt());
        e->setDuration(dialog.getDuration());
        QStringList pulseList = dialog.getPulse().split(PULSE_SEPARATOR);
        e->setMaxPulse(pulseList[0].toInt());
        e->setAvgPulse(pulseList[1].toInt());
        QStringList caloriesList = dialog.getCalories().split(PULSE_SEPARATOR);
        e->setCal(caloriesList[0].toInt());
        e->setFat(caloriesList[1].toInt());
        e->setSport(dataHandler.getSportId(dialog.getSport()));
        QStringList placeList = dialog.getPlaceList();
        for (int i = 0; i < placeList.size(); ++i)
            e->addPlace(dataHandler.getPlaceId(placeList.at(i)));
        e->setShoe(dataHandler.getShoeId(dialog.getShoe()));
        e->setComment(dialog.getComment());
        QStringList weatherList = dialog.getWeatherList();
        for (int i = 0; i < weatherList.size(); ++i)
            e->addWeather(dataHandler.getWeatherId(weatherList.at(i)));
        dataHandler.exercises.append(e);
        m_exerciseTable->appendTable(e);
        m_statisticsHandler->calculateStatistics();
        setDirty();
    }
}

bool SimpleTrainingLogMainWindow::save()
{
    // TODO: Ask the user to create a database. Optionally. Write the changed
    // exercises only. All changes on the table will be present in these
    // data structures as well.
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (!dataHandler.saveDatabase(m_curLog)) {
        QMessageBox::warning(this, tr("SimpleTrainingLog"), tr("Cannot write file: %1").arg(m_curLog));
        return false;
    }
    QApplication::restoreOverrideCursor();
    ui->action_Save->setEnabled(false);
    m_dirty = false;
    statusBar()->showMessage(tr("Data saved"), 2000);
    return true;
}

bool SimpleTrainingLogMainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return true;
}

void SimpleTrainingLogMainWindow::about()
{
    QMessageBox::about(this, tr("SimpleTrainingLog"),
        tr("<p><b>SimpleTrainingLog</b> is a simple GUI application to organize "
           "exercise data. <b>SimpleTrainingLog</b> uses a plain text file as its "
           "database to make the data easy to archive. Brief summary of "
           "features:<ul><li>Managing basic exercise data.</li><li>Limited "
           "sorting capabilities of exercises.</li><li>Exporting to HTML. "
           "</li><li>Exercise/place/shoe/sport/weather database.</li><li>Basic "
           "exercise editing features.</li><li>Uploading exercise data to a "
           "custom FTP site.</li><li>Managing shoe/sport/place/weather data."
           "</li><li>Basic statistics with diagrams.</li></ul></p><p>If you "
           "have any idea how to improve <b>SimpleTrainingLog</b> please contact me "
           "at the address below. Bug reports or improvement ideas are "
           "welcome. Thank you.</p><p>Ferenc Kovács "
           "&lt;derefer@gmail.com&gt;</p>"));
}

void SimpleTrainingLogMainWindow::aboutQt()
{
    QApplication::aboutQt();
}

void SimpleTrainingLogMainWindow::documentWasModified()
{
    setWindowModified(true);
}

void SimpleTrainingLogMainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(ui->action_New_Exercise);
    fileToolBar->addAction(ui->action_Save);
    fileToolBar->addSeparator();
    fileToolBar->addAction(ui->action_Export_Publishing);
}

void SimpleTrainingLogMainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void SimpleTrainingLogMainWindow::readSettings()
{
    QFile file(m_curConfig);
    if (!file.exists()) {
        writeSettings();
        return;
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    QString line;
    do {
        line = stream.readLine();
        int index = line.indexOf("=") + 1;
        if (!index)
            continue;
        if (line.contains("log=")) m_curLog = line.mid(index);
        else if (line.contains("host=")) m_curHost = line.mid(index);
        else if (line.contains("port=")) m_curPort = line.mid(index);
        else if (line.contains("path=")) m_curPath = line.mid(index);
        else if (line.contains("user=")) m_curUser = line.mid(index);
    } while (!line.isNull());
    file.close();
}

void SimpleTrainingLogMainWindow::writeSettings()
{
    QFile file(m_curConfig);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(&file);
    stream << QString("log=%1\n").arg(m_curLog);
    stream << QString("host=%1\n").arg(m_curHost);
    stream << QString("port=%1\n").arg(m_curPort);
    stream << QString("path=%1\n").arg(m_curPath);
    stream << QString("user=%1\n").arg(m_curUser);
    file.close();
}

bool SimpleTrainingLogMainWindow::maybeSave()
{
    if (!m_dirty)
        return true;
    // Ask for confirmation.
    int ret = QMessageBox::warning(this, tr("SimpleTrainingLog"), tr("Exercise data has been modified. Do you want to save your changes?"),
        QMessageBox::Yes | QMessageBox::Default, QMessageBox::No,
        QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Yes) return save();
    else if (ret == QMessageBox::Cancel) return false;
    return true;
}

void SimpleTrainingLogMainWindow::loadDatabase()
{
    dataHandler.loadDatabase(m_curLog);

    m_exerciseTable->fillTable(&dataHandler.exercises);
    m_statisticsHandler->fillHandler(&dataHandler.sports, &dataHandler.exercises);

    setCurrentFile(m_curLog);
    statusBar()->showMessage(tr("Database loaded"), 2000);
}

// Set the exercise database.
void SimpleTrainingLogMainWindow::setCurrentFile(const QString& fileName)
{
    m_curLog = fileName;
    // textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (m_curLog.isEmpty()) shownName = "untitled";
    else shownName = strippedName(m_curLog);
    setWindowTitle(tr("%1 - %2[*]").arg(tr("SimpleTrainingLog")).arg(shownName));
}

bool SimpleTrainingLogMainWindow::exportHtml()
{
    // Only HTML export is available.
    ExportDialog dialog(this);
    dialog.setDefaultFile(m_curLog);
    dialog.setDefaultHost(m_curHost);
    dialog.setDefaultPort(m_curPort.toInt());
    dialog.setDefaultPath(m_curPath);
    dialog.setDefaultUser(m_curUser);
    if (dialog.exec()) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QString htmlFileName(QString(m_curLog).append(".html"));
        if (!dataHandler.exportDatabaseAsHtml(htmlFileName)) {
            QMessageBox::warning(this, tr("SimpleTrainingLog"), tr("Cannot write file: %1").arg(htmlFileName));
            return false;
        }
        QApplication::restoreOverrideCursor();
        /*
        if (dialog.isUpload()) {
            // Don't use QFtp as a local variable.  The callbacks will need
            // the object.  Don't forget to free it.
            m_ftp = new QFtp;
            connect(m_ftp, SIGNAL(stateChanged(int)), this,
                SLOT(ftpStateChanged(int)));
            m_ftp->connectToHost(dialog.getHost(), dialog.getPort());
            m_ftp->login(dialog.getUser(), dialog.getPassword());
            m_ftp->put(outList.join("").toUtf8(), dialog.getPath());
        }
        */
        statusBar()->showMessage(tr("HTML export successful"), 5000);
    }
    return true;
}

QString SimpleTrainingLogMainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void SimpleTrainingLogMainWindow::clear()
{
    for (int i = 0; i < dataHandler.shoes.size(); ++i) delete dataHandler.shoes.at(i);
    for (int i = 0; i < dataHandler.exercises.size(); ++i) delete dataHandler.exercises.at(i);
    for (int i = 0; i < dataHandler.places.size(); ++i) delete dataHandler.places.at(i);
    for (int i = 0; i < dataHandler.sports.size(); ++i) delete dataHandler.sports.at(i);
    m_exerciseTable->clear();
    m_statisticsHandler->clear();
}

void SimpleTrainingLogMainWindow::ftpStateChanged(int /*state*/)
{/*
    switch (state) {
    case QFtp::Unconnected:
        // statusBar()->showMessage(tr("Not connected"), 2000);
        qWarning("Not connected");
        break;
    case QFtp::HostLookup:
        // statusBar()->showMessage(tr("Looking up host..."), 2000);
        qWarning("Looking up host...");
        break;
    case QFtp::Connecting:
        // statusBar()->showMessage(tr("Connecting..."), 2000);
        qWarning("Connecting...");
        break;
    case QFtp::Connected:
        // statusBar()->showMessage(tr("Connected"), 2000);
        qWarning("Connected");
        break;
    case QFtp::LoggedIn:
        // statusBar()->showMessage(tr("Login succesful"), 2000);
        qWarning("Login successful");
        break;
    case QFtp::Closing:
        // statusBar()->showMessage(tr("Closing..."), 2000);
        qWarning("Closing...");
        break;
    }*/
}

void SimpleTrainingLogMainWindow::manageShoes()
{
    ShoesDialog dialog(this, &dataHandler);
    if (dialog.exec()) {
        if (dialog.isDirty())
            setDirty();
        const QList<int>& modified = dialog.getModifiedExercises();
        const QList<int>& removed = dialog.getRemovedExercises();
        for (int i = 0; i < modified.size(); ++i)
            m_exerciseTable->updateExercise(modified.at(i));
        for (int i = 0; i < removed.size(); ++i) {
            m_exerciseTable->removeExercise(removed.at(i));
            dataHandler.removeExerciseById(removed.at(i));
        }
    }
}

void SimpleTrainingLogMainWindow::manageSports()
{
    SportsDialog dialog(this, &dataHandler);
    if (dialog.exec()) {
        if (dialog.isDirty())
            setDirty();
        const QList<int>& modified = dialog.getModifiedExercises();
        const QList<int>& removed = dialog.getRemovedExercises();
        for (int i = 0; i < modified.size(); ++i)
            m_exerciseTable->updateExercise(modified.at(i));
        for (int i = 0; i < removed.size(); ++i) {
            m_exerciseTable->removeExercise(removed.at(i));
            dataHandler.removeExerciseById(removed.at(i));
        }
    }
}

void SimpleTrainingLogMainWindow::managePlaces()
{
    PlacesDialog dialog(this, &dataHandler);
    if (dialog.exec()) {
        if (dialog.isDirty())
            setDirty();
        const QList<int>& modified = dialog.getModifiedExercises();
        const QList<int>& removed = dialog.getRemovedExercises();
        for (int i = 0; i < modified.size(); ++i)
            m_exerciseTable->updateExercise(modified.at(i));
        for (int i = 0; i < removed.size(); ++i) {
            m_exerciseTable->removeExercise(removed.at(i));
            dataHandler.removeExerciseById(removed.at(i));
        }
    }
}

void SimpleTrainingLogMainWindow::manageWeathers()
{
    WeathersDialog dialog(this, &dataHandler);
    if (dialog.exec()) {
        if (dialog.isDirty())
            setDirty();
        const QList<int>& modified = dialog.getModifiedExercises();
        const QList<int>& removed = dialog.getRemovedExercises();
        for (int i = 0; i < modified.size(); ++i)
            m_exerciseTable->updateExercise(modified.at(i));
        for (int i = 0; i < removed.size(); ++i) {
            m_exerciseTable->removeExercise(removed.at(i));
            dataHandler.removeExerciseById(removed.at(i));
        }
    }
}

void SimpleTrainingLogMainWindow::settings()
{
    SettingsDialog dialog(this, m_curLog, m_curHost, m_curPort, m_curPath, m_curUser);
    if (dialog.exec()) {
        QString dLog = dialog.getLog();
        QString dHost = dialog.getHost();
        QString dPort = dialog.getPort();
        QString dPath = dialog.getPath();
        QString dUser = dialog.getUser();
        if (dLog != m_curLog) {
            clear();
            m_curLog = dLog;
            loadDatabase();
        }
        if (dHost != m_curHost) m_curHost = dHost;
        if (dPort != m_curPort) m_curPort = dPort;
        if (dPath != m_curPath) m_curPath = dPath;
        if (dUser != m_curUser) m_curUser = dUser;
    }
}
