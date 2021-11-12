#include <QApplication>
#include <QCloseEvent>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QTableWidget>
#include <QTextEdit>
#include <QTextStream>
#include <QToolBar>
#include <QtDebug>
//#include <QFtp>

#include "SimpleTrainingLogMainWindow.h"
#include "NewExerciseDialog.h"
#include "ExportDialog.h"
#include "ShoesDialog.h"
#include "SportsDialog.h"
#include "PlacesDialog.h"
#include "WeathersDialog.h"
#include "SettingsDialog.h"

// This is the driver of the parser.  Everything is prefixed with
// exercisedataparser*.  The name of the file is for error reporting.
extern FILE *ExerciseDataParserin;
extern const char *ExerciseDataParserin_name;
extern int ExerciseDataParserparse();
extern int ExerciseDataParserdebug;
extern int ExerciseDataParser_flex_debug;

// Global data from the parser.
extern QList<Shoe*> shoes;
extern QList<Sport*> sports;
extern QList<Exercise*> exercises;
extern QList<Place*> places;
extern QList<Weather*> weathers;

SimpleTrainingLogMainWindow::SimpleTrainingLogMainWindow() : /*m_ftp(NULL),*/ m_dirty(false)
{
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    m_curConfig = DEFAULT_CONFIG;
    m_curLog = DEFAULT_LOG;
    m_curJsonLog = DEFAULT_JSON_LOG;
    m_curHost = DEFAULT_HOST;
    m_curPort = QString("%1").arg(DEFAULT_PORT);
    m_curPath = DEFAULT_PATH;
    m_curUser = DEFAULT_USER;

    readSettings();

    m_exerciseTable = new ExerciseTable();
    m_statisticsHandler = new StatisticsHandler();
    m_mainTabWidget = new QTabWidget;
    m_mainTabWidget->addTab(m_exerciseTable, tr("Summary"));
    m_mainTabWidget->addTab(m_statisticsHandler, tr("Statistics"));
    setCentralWidget(m_mainTabWidget);

    setWindowTitle(QApplication::translate("SimpleTrainingLog", "SimpleTrainingLog", 0));
    resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

    loadDatabase();

    // The SIGNAL and the SLOT must have the same signature. Any modification
    // of the table will trigger setDirty.
    m_exerciseTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_exerciseTable, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(contextMenuForTable(const QPoint &)));
    connect(m_exerciseTable, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
        this, SLOT(editExercise(QTreeWidgetItem *, int)));
}

void SimpleTrainingLogMainWindow::setDirty(int row __attribute__((unused)), int col __attribute__((unused)))
{
    saveAct->setEnabled(true);
    m_dirty = true;
}

void SimpleTrainingLogMainWindow::deleteExercise()
{
    m_exerciseTable->removeCurrentExercise();
    setDirty();
}

void SimpleTrainingLogMainWindow::viewExercise()
{
    NewExerciseDialog dialog(this, "View Exercise");
    dialog.addSportStrings(getSportStrings());
    dialog.addShoeStrings(getShoeStrings());
    dialog.addPlaceStrings(getPlaceStrings());
    dialog.addWeatherStrings(getWeatherStrings());
    dialog.setReadOnly();

    Exercise *e = m_exerciseTable->getCurrentExercise();
    dialog.setDate((e->getDate()).split("-"));
    dialog.setTime(e->getTime());
    dialog.setDistance(QString::number(e->getDistance()));
    dialog.setDuration(e->getDuration());
    dialog.setPulse(e->getMaxPulse(), e->getAvgPulse());
    dialog.setCalories(e->getCal(), e->getFat());
    dialog.setSport(getSportById(e->getSport())->getName());
    QList<Place*> placesById = getPlacesById(e->getPlaces());
    QStringList placesStringList;
    for (int i = 0; i < placesById.size(); ++i)
        placesStringList << (placesById.at(i))->getName();
    dialog.setPlaces(placesStringList);
    dialog.setShoe(getShoeById(e->getShoe())->getName());
    QList<Weather*> weathersById = getWeathersById(e->getWeathers());
    QStringList weathersStringList;
    for (int i = 0; i < weathersById.size(); ++i)
        weathersStringList << (weathersById.at(i))->getName();
    dialog.setWeathers(weathersStringList);
    dialog.setComment(e->getComment());

    if (dialog.exec()) {
        // Do we need this?
    }
}

void SimpleTrainingLogMainWindow::editExercise(QTreeWidgetItem *item __attribute__((unused)), int column __attribute__((unused)))
{
    NewExerciseDialog dialog(this, "Edit Exercise");
    dialog.addSportStrings(getSportStrings());
    dialog.addShoeStrings(getShoeStrings());
    dialog.addPlaceStrings(getPlaceStrings());
    dialog.addWeatherStrings(getWeatherStrings());

    Exercise *e = m_exerciseTable->getCurrentExercise();
    dialog.setDate((e->getDate()).split("-"));
    dialog.setTime(e->getTime());
    dialog.setDistance(QString::number(e->getDistance()));
    dialog.setDuration(e->getDuration());
    dialog.setPulse(e->getMaxPulse(), e->getAvgPulse());
    dialog.setCalories(e->getCal(), e->getFat());
    dialog.setSport(getSportById(e->getSport())->getName());
    QList<Place*> placesById = getPlacesById(e->getPlaces());
    QStringList placesStringList;
    for (int i = 0; i < placesById.size(); ++i)
        placesStringList << (placesById.at(i))->getName();
    dialog.setPlaces(placesStringList);
    dialog.setShoe(getShoeById(e->getShoe())->getName());
    QList<Weather*> weathersById = getWeathersById(e->getWeathers());
    QStringList weathersStringList;
    for (int i = 0; i < weathersById.size(); ++i)
        weathersStringList << (weathersById.at(i))->getName();
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
        e->setSport(getSportByName(dialog.getSport()));
        e->clearPlaces();
        QStringList placeList = dialog.getPlaceList();
        for (int i = 0; i < placeList.size(); ++i)
            e->addPlace(getPlaceByName(placeList.at(i)));
        e->setShoe(getShoeByName(dialog.getShoe()));
        e->setComment(dialog.getComment());
        e->clearWeathers();
        QStringList weatherList = dialog.getWeatherList();
        for (int i = 0; i < weatherList.size(); ++i)
            e->addWeather(getWeatherByName(weatherList.at(i)));
        m_exerciseTable->updateExercise(e->getId());
        m_statisticsHandler->calculateStatistics();
        setDirty();
    }
}

// It seems to be better to do it here instead of using mousePressEvent() in
// QTreeWidget.
void SimpleTrainingLogMainWindow::contextMenuForTable(const QPoint& pos)
{
    QTreeWidgetItem *item = NULL;
    item = m_exerciseTable->itemAt(pos);
    if (NULL != item) {
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
    } else {
        return;
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
    if (!(sports.size() > 0 && shoes.size() > 0)) return;

    NewExerciseDialog dialog(this, "New Exercise");
    dialog.addSportStrings(getSportStrings());
    dialog.addShoeStrings(getShoeStrings());
    dialog.addPlaceStrings(getPlaceStrings());
    dialog.addWeatherStrings(getWeatherStrings());

    if (dialog.exec()) {
        Exercise *e = new Exercise(exercises.size());
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
        e->setSport(getSportByName(dialog.getSport()));
        QStringList placeList = dialog.getPlaceList();
        for (int i = 0; i < placeList.size(); ++i)
            e->addPlace(getPlaceByName(placeList.at(i)));
        e->setShoe(getShoeByName(dialog.getShoe()));
        e->setComment(dialog.getComment());
        QStringList weatherList = dialog.getWeatherList();
        for (int i = 0; i < weatherList.size(); ++i)
            e->addWeather(getWeatherByName(weatherList.at(i)));
        exercises.append(e);
        m_exerciseTable->appendTable(e);
        m_statisticsHandler->calculateStatistics();
        setDirty();
    }
}

bool SimpleTrainingLogMainWindow::save()
{
    // TODO Ask the user to create a database.  Optionally.  Write the changed
    // exercises only.  All changes on the table will be present in these
    // data structures as well.
    QFile file(m_curLog);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SimpleTrainingLog"), tr("Cannot write "
            "file %1: %2.").arg(DEFAULT_LOG).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QStringList outData("shoes {\n");
    for (int i = 0; i < shoes.size(); ++i) {
        outData << QString("{ id=%1").arg(i) <<
            QString(", name=\"%1\"").arg(shoes.at(i)->getName()) <<
            QString(", buy=%1").arg(shoes.at(i)->getBuy()) <<
            QString(", comment=\"%1\" }\n").arg(shoes.at(i)->getComment());
    }
    outData << "}\nsports {\n";
    for (int i = 0; i < sports.size(); ++i) {
        outData << QString("{ id=%1").arg(i);
        outData << QString(", name=\"%1\"").arg(sports.at(i)->getName());
        outData << QString(", color=\"%1\" }\n").arg((sports.at(i)->getColor()).name());
    }
    outData << "}\nplaces {\n";
    for (int i = 0; i < places.size(); ++i) {
        outData << QString("{ id=%1").arg(i) <<
            QString(", name=\"%1\" }\n").arg(places.at(i)->getName());
    }
    outData << "}\nweathers {\n";
    for (int i = 0; i < weathers.size(); ++i) {
        outData << QString("{ id=%1").arg(i) <<
            QString(", name=\"%1\" }\n").arg(weathers.at(i)->getName());
    }
    outData << "}\nexercises {\n";
    for (int i = 0; i < exercises.size(); ++i) {
        Exercise *e = exercises.at(i);
        outData << QString("{ id=%1").arg(i);
        outData << QString(", date=\"%1\"").arg(e->getDate());
        outData << QString(", time=\"%1\"").arg(e->getTime());
        outData << QString(", distance=%1").arg(e->getDistance());
        outData << QString(", duration=\"%1\"").arg(e->getDuration());
        outData << QString(", sport=%1").arg(e->getSport());
        // Need for improvement.
        QList<int> placeList = e->getPlaces();
        QStringList placeNumberList;
        for (int j = 0; j < placeList.size(); ++j)
            placeNumberList << QString::number(placeList.at(j));
        outData << QString(", place=(%1)").arg(placeNumberList.join(", "));
        outData << QString(", shoe=%1").arg(e->getShoe());
        outData << QString(", comment=\"%1\"").arg(encodeComment(e->getComment()));
        QList<int> weatherList = e->getWeathers();
        QStringList weatherNumberList;
        for (int j = 0; j < weatherList.size(); ++j)
            weatherNumberList << QString::number(weatherList.at(j));
        outData << QString(", weather=(%1)").arg(weatherNumberList.join(", "));
        outData << QString(", pulse=%1/%2").arg(e->getMaxPulse()).arg(e->getAvgPulse());
        outData << QString(", calories=%1/%2 }\n").arg(e->getCal()).arg(e->getFat());
    }
    outData << "}\n";
    out << outData.join("");
    QApplication::restoreOverrideCursor();
    file.close();

    saveAct->setEnabled(false);
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

void SimpleTrainingLogMainWindow::documentWasModified()
{
    setWindowModified(true);
}

void SimpleTrainingLogMainWindow::createActions()
{
    newAct = new QAction(QIcon(":Images/new.png"), tr("&New Exercise"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("Add a new exercise"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newExercise()));

    saveAct = new QAction(QIcon(":Images/save.png"), tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the changes to database"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    exportHTMLAct = new QAction(QIcon(":Images/export.png"),
        tr("&Export && Publishing"), this);
    exportHTMLAct->setShortcut(tr("Ctrl+E"));
    exportHTMLAct->setStatusTip(tr("HTML export & FTP upload"));
    connect(exportHTMLAct, SIGNAL(triggered()), this, SLOT(exportHTML()));

    exitAct = new QAction(QIcon(":Images/exit.png"), tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    m_shoesAct = new QAction(tr("S&hoes"), this);
    m_shoesAct->setStatusTip(tr("Manage shoes"));
    connect(m_shoesAct, SIGNAL(triggered()), this, SLOT(manageShoes()));

    m_sportsAct = new QAction(tr("Sp&orts"), this);
    m_sportsAct->setStatusTip(tr("Manage sports"));
    connect(m_sportsAct, SIGNAL(triggered()), this, SLOT(manageSports()));

    m_placesAct = new QAction(tr("&Places"), this);
    m_placesAct->setStatusTip(tr("Manage places"));
    connect(m_placesAct, SIGNAL(triggered()), this, SLOT(managePlaces()));

    m_weathersAct = new QAction(tr("&Weathers"), this);
    m_weathersAct->setStatusTip(tr("Manage weathers"));
    connect(m_weathersAct, SIGNAL(triggered()), this, SLOT(manageWeathers()));

    m_settingsAct = new QAction(QIcon(":Images/configure.png"),
        tr("&Settings"), this);
    m_settingsAct->setStatusTip(tr("Settings"));
    connect(m_settingsAct, SIGNAL(triggered()), this, SLOT(settings()));

    aboutAct = new QAction(QIcon(":Images/about.png"), tr("&About"), this);
    aboutAct->setStatusTip(tr("About SimpleTrainingLog"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("About Qt"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    saveAct->setEnabled(false);
    // connect(textEdit, SIGNAL(copyAvailable(bool)), cutAct,
    // SLOT(setEnabled(bool)));
}

void SimpleTrainingLogMainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exportHTMLAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(m_shoesAct);
    editMenu->addAction(m_sportsAct);
    editMenu->addAction(m_placesAct);
    editMenu->addAction(m_weathersAct);
    editMenu->addSeparator();
    editMenu->addAction(m_settingsAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void SimpleTrainingLogMainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(exportHTMLAct);
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
    int ret = QMessageBox::warning(this, tr("SimpleTrainingLog"), tr("Exercise data "
        "has been modified. Do you want to save your changes?"),
        QMessageBox::Yes | QMessageBox::Default, QMessageBox::No,
        QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Yes) return save();
    else if (ret == QMessageBox::Cancel) return false;
    return true;
}

void SimpleTrainingLogMainWindow::loadDatabase()
{
    loadJsonDatabase();

    m_exerciseTable->fillTable(&exercises);
    m_statisticsHandler->fillHandler(&sports, &exercises);

    setCurrentFile(m_curLog);
    statusBar()->showMessage(tr("Database loaded"), 2000);
}

void SimpleTrainingLogMainWindow::loadJsonDatabase()
{
    QString curJsonLogAsString;
    QFile file(m_curJsonLog);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot read file" << m_curJsonLog << ":" << file.errorString();
        return;
    }
    curJsonLogAsString = file.readAll();
    file.close();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(curJsonLogAsString.toUtf8());
    if (jsonDocument.isNull()) {
        qWarning() << "Invalid JSON document:" << m_curJsonLog;
        return;
    }

    QJsonObject jsonObject = jsonDocument.object();
    QJsonArray shoesFromJsonAsArray = jsonObject.value("shoes").toArray();
    QJsonArray sportsFromJsonAsArray = jsonObject.value("sports").toArray();
    QJsonArray placesFromJsonAsArray = jsonObject.value("places").toArray();
    QJsonArray weathersFromJsonAsArray = jsonObject.value("weathers").toArray();
    QJsonArray exercisesFromJsonAsArray = jsonObject.value("exercises").toArray();

    for (int i = 0; i < shoesFromJsonAsArray.size(); i++) {
        const auto& shoe = shoesFromJsonAsArray[i].toObject();
        const auto& shoeId = shoe["id"].toInt();
        if (shoeId < 0) {
            qWarning() << "Invalid shoe identifier:" << shoeId;
            continue;
        }
        const auto& shoeName = shoe["name"].toString();
        const auto& shoeBuy = shoe["buy"].toInt();
        const auto& shoeComment = shoe["comment"].toString();
        shoes.append(new Shoe(shoeId, shoeName, shoeBuy, shoeComment));
    }

    for (int i = 0; i < sportsFromJsonAsArray.size(); i++) {
        const auto& sport = sportsFromJsonAsArray[i].toObject();
        const auto& sportId = sport["id"].toInt();
        if (sportId < 0) {
            qWarning() << "Invalid sport identifier:" << sportId;
            continue;
        }
        const auto& sportName = sport["name"].toString();
        const auto& sportColor = sport["color"].toString();
        sports.append(new Sport(sportId, sportName, sportColor));
    }

    for (int i = 0; i < placesFromJsonAsArray.size(); i++) {
        const auto& place = placesFromJsonAsArray[i].toObject();
        const auto& placeId = place["id"].toInt();
        if (placeId < 0) {
            qWarning() << "Invalid place identifier:" << placeId;
            continue;
        }
        const auto& placeName = place["name"].toString();
        places.append(new Place(placeId, placeName));
    }

    for (int i = 0; i < weathersFromJsonAsArray.size(); i++) {
        const auto& weather = weathersFromJsonAsArray[i].toObject();
        const auto& weatherId = weather["id"].toInt();
        if (weatherId < 0) {
            qWarning() << "Invalid weather identifier:" << weatherId;
            continue;
        }
        const auto& weatherName = weather["name"].toString();
        weathers.append(new Weather(weatherId, weatherName));
    }

    for (int i = 0; i < exercisesFromJsonAsArray.size(); i++) {
        const auto& exercise = exercisesFromJsonAsArray[i].toObject();
        const auto& exerciseId = exercise["id"].toInt();
        if (exerciseId < 0) {
            qWarning() << "Invalid exercise identifier:" << exerciseId;
            continue;
        }
        const auto& exerciseDate = exercise["date"].toString();
        const auto& exerciseTime = exercise["time"].toString();
        const auto& exerciseDistance = exercise["distance"].toInt();
        const auto& exerciseDuration = exercise["duration"].toString();
        const auto& exerciseSport = exercise["sport"].toInt();
        const auto& exercisePlace = exercise["place"].toString().split(", ");
        const auto& exerciseShoe = exercise["shoe"].toInt();
        const auto& exerciseComment = exercise["comment"].toString();
        const auto& exerciseWeather = exercise["weather"].toString().split(", ");
        const auto& exercisePulse = exercise["pulse"].toString().split("/"); // MaxBpm/AvgBpm
        const auto& exerciseCalories = exercise["calories"].toString().split("/"); // Calories/Fat%
        Exercise *newExercise = new Exercise(exerciseId);
        newExercise->setDate(exerciseDate);
        newExercise->setTime(exerciseTime);
        newExercise->setDistance(exerciseDistance);
        newExercise->setDuration(exerciseDuration);
        newExercise->setSport(exerciseSport);
        QList<int> places;
        for (const auto& place : exercisePlace) { places.append(place.toInt()); }
        newExercise->setPlaces(places);
        newExercise->setShoe(exerciseShoe);
        newExercise->setComment(exerciseComment);
        QList<int> weathers;
        for (const auto& weather : exerciseWeather) { weathers.append(weather.toInt()); }
        newExercise->setWeathers(weathers);
        newExercise->setMaxPulse(exercisePulse[0].toInt());
        newExercise->setAvgPulse(exercisePulse[1].toInt());
        newExercise->setCal(exerciseCalories[0].toInt());
        newExercise->setFat(exerciseCalories[1].toInt());
        exercises.append(newExercise);
    }
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

bool SimpleTrainingLogMainWindow::exportHTML()
{
    // Only HTML export is available.
    ExportDialog dialog(this);
    dialog.setDefaultFile(m_curLog);
    dialog.setDefaultHost(m_curHost);
    dialog.setDefaultPort(m_curPort.toInt());
    dialog.setDefaultPath(m_curPath);
    dialog.setDefaultUser(m_curUser);
    if (dialog.exec()) {
        QString fileName(QString(m_curLog).append(".html"));
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("SimpleTrainingLog"), tr("Cannot write "
                "file %1: %2.").arg(fileName).arg(file.errorString()));
            return false;
        }
        QTextStream out(&file);
        QStringList outList;
        QApplication::setOverrideCursor(Qt::WaitCursor);
        // TODO Create a template.  As a QStringList we can reuse the HTML
        // data during the uploading process.
        outList << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<html>\n";
        outList << "<head>\n";
        outList << "<meta http-equiv=\"content-type\" content=\"text/html; "
            "charset=utf-8\"/>\n<title>Exercises</title>\n";
        outList << "<script type=\"text/javascript\">\n"
            "function display_year(year) {\n"
            "  var table = document.getElementById(\"exercise_table\");\n"
            "  for (var i = 0; i < table.tBodies.length; i++) {\n"
            "    for (var j = 1; j < table.tBodies[i].rows.length; j++) {\n"
            "      row_year = parseInt(table.tBodies[i].rows[j].cells[1].innerHTML.split(\"-\")[0]);\n"
            "      if (year == row_year) table.tBodies[i].rows[j].style.display = \"\";\n"
            "      else table.tBodies[i].rows[j].style.display = \"none\";\n"
            "    }\n"
            "  }\n"
            "}\n"
            "</script>\n";
        outList << "<style type=\"text/css\">\n"
            "h1 {\n"
            "  font-family: verdana, arial, sans-serif;\n"
            "  font-size: 14px;\n"
            "  font-weight: bold;\n"
            "}\n"
            "a:link, a:visited a:hover, a:active {\n"
            "  font-family: verdana, arial, sans-serif;\n"
            "  font-size: 10px;\n"
            "  text-decoration: none;\n"
            "}\n"
            "a:hover {\n"
            "  text-decoration: underline;\n"
            "}\n"
            "p.normal {\n"
            "  font-family: verdana, arial, sans-serif;\n"
            "  font-size: 10px;\n"
            "}\n"
            "th {\n"
            "  font-family: verdana, arial, sans-serif;\n"
            "  font-size: 10px;\n"
            "  background: #1b63ac;\n"
            "  color: white;\n"
            "}\n"
            "td {\n"
            "  vertical-align: top;\n"
            "  padding: 0.5em;\n"
            "}\n"
            "td.normal {\n"
            "  font-family: verdana, arial, sans-serif;\n"
            "  font-size: 10px;\n"
            "}\n";
        outList << "</style>\n";
        outList << "</head>\n";
        outList << "<body>\n";
        outList << "<h1>EXERCISES</h1>\n";
        outList << "<p class=\"normal\">\nFILTER BY YEAR: ";
        QSet<QString> years;
        for (int i = 0; i < exercises.size(); ++i)
            years.insert(exercises.at(i)->getDate().split("-")[0]);
        foreach (QString year, years)  // Qt's construct.
            outList << "<a href=\"javascript:display_year(" << year << ")\"><b>"
                    << year << "</b></a>\n";
        outList << "</p>\n";
        outList << "<table cellspacing=\"2\" border=\"1\">\n";
        outList << "<tr>\n";
        for (int i = 0; i < sports.size(); ++i)
            outList << "<td bgcolor=\"" << sports.at(i)->getColor().name()
                    << "\" class=\"normal\"><b>" << sports.at(i)->getName()
                    << "</b></td>\n";
        outList << "</tr>\n";
        outList << "</table>\n";
        outList << m_exerciseTable->encodeHTML();
        outList << "</body>\n";
        outList << "</html>\n";
        out << outList.join("");  // Use the QStringList.
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
        }*/
        statusBar()->showMessage(tr("HTML export successful"), 2000);
    }
    return true;
}

QString SimpleTrainingLogMainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void SimpleTrainingLogMainWindow::clear()
{
    for (int i = 0; i < shoes.size(); ++i) delete shoes.at(i);
    for (int i = 0; i < exercises.size(); ++i) delete exercises.at(i);
    for (int i = 0; i < places.size(); ++i) delete places.at(i);
    for (int i = 0; i < sports.size(); ++i) delete sports.at(i);
    m_exerciseTable->clear();
    m_statisticsHandler->clear();
}

SimpleTrainingLogMainWindow::~SimpleTrainingLogMainWindow()
{
    /*if (m_ftp)
        delete m_ftp;*/

    clear();
}

int SimpleTrainingLogMainWindow::parseFile(const char *fileName)
{
    // exercisedataparserdebug = 1;
    ExerciseDataParser_flex_debug = 0;
    ExerciseDataParserin_name = fileName;
    ExerciseDataParserin = fopen(fileName, "r");
    if (ExerciseDataParserin == 0) {
        return 1;
    }
    int ret = ExerciseDataParserparse();
    fclose(ExerciseDataParserin);
    return ret;
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

int SimpleTrainingLogMainWindow::getSportByName(const QString& sport) const
{
    for (int i = 0; i < sports.size(); ++i)
        if ((sports.at(i))->getName() == sport)
            return i;
    return -1;
}

int SimpleTrainingLogMainWindow::getPlaceByName(const QString& place) const
{
    for (int i = 0; i < places.size(); ++i)
        if ((places.at(i))->getName() == place)
            return i;
    return -1;
}

int SimpleTrainingLogMainWindow::getShoeByName(const QString& shoe) const
{
    for (int i = 0; i < shoes.size(); ++i)
        if ((shoes.at(i))->getName() == shoe)
            return i;
    return -1;
}

int SimpleTrainingLogMainWindow::getWeatherByName(const QString& weather) const
{
    for (int i = 0; i < weathers.size(); ++i)
        if ((weathers.at(i))->getName() == weather)
            return i;
    return -1;
}

void SimpleTrainingLogMainWindow::manageShoes()
{
    ShoesDialog dialog(this, &shoes, &exercises);
    if (dialog.exec()) {
        if (dialog.isDirty())
            setDirty();
        const QList<int>& modified = dialog.getModifiedExercises();
        const QList<int>& removed = dialog.getRemovedExercises();
        for (int i = 0; i < modified.size(); ++i)
            m_exerciseTable->updateExercise(modified.at(i));
        for (int i = 0; i < removed.size(); ++i) {
            m_exerciseTable->removeExercise(removed.at(i));
            removeExerciseById(removed.at(i));
        }
    }
}

void SimpleTrainingLogMainWindow::manageSports()
{
    SportsDialog dialog(this, &sports, &exercises);
    if (dialog.exec()) {
        if (dialog.isDirty())
            setDirty();
        const QList<int>& modified = dialog.getModifiedExercises();
        const QList<int>& removed = dialog.getRemovedExercises();
        for (int i = 0; i < modified.size(); ++i)
            m_exerciseTable->updateExercise(modified.at(i));
        for (int i = 0; i < removed.size(); ++i) {
            m_exerciseTable->removeExercise(removed.at(i));
            removeExerciseById(removed.at(i));
        }
    }
}

void SimpleTrainingLogMainWindow::managePlaces()
{
    PlacesDialog dialog(this, &places, &exercises);
    if (dialog.exec()) {
        if (dialog.isDirty())
            setDirty();
        const QList<int>& modified = dialog.getModifiedExercises();
        const QList<int>& removed = dialog.getRemovedExercises();
        for (int i = 0; i < modified.size(); ++i)
            m_exerciseTable->updateExercise(modified.at(i));
        for (int i = 0; i < removed.size(); ++i) {
            m_exerciseTable->removeExercise(removed.at(i));
            removeExerciseById(removed.at(i));
        }
    }
}

void SimpleTrainingLogMainWindow::manageWeathers()
{
    WeathersDialog dialog(this, &weathers, &exercises);
    if (dialog.exec()) {
        if (dialog.isDirty())
            setDirty();
        const QList<int>& modified = dialog.getModifiedExercises();
        const QList<int>& removed = dialog.getRemovedExercises();
        for (int i = 0; i < modified.size(); ++i)
            m_exerciseTable->updateExercise(modified.at(i));
        for (int i = 0; i < removed.size(); ++i) {
            m_exerciseTable->removeExercise(removed.at(i));
            removeExerciseById(removed.at(i));
        }
    }
}

void SimpleTrainingLogMainWindow::settings()
{
    SettingsDialog dialog(this);
    dialog.setLog(m_curLog);
    dialog.setHost(m_curHost);
    dialog.setPort(m_curPort);
    dialog.setPath(m_curPath);
    dialog.setUser(m_curUser);
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
