#pragma once

#include <QMainWindow>
#include <QCloseEvent>
// QFtp was remvoved in Qt5.
//#include <QFtp>

#include "ExerciseTable.h"
#include "SimpleTrainingLogLibDecl.h"
#include "StatisticsHandler.h"

// Burned in database.  Read these from configuration file.  Password is
// always requested from the user.
#define DEFAULT_CONFIG "./.SimpleTrainingLog"
#define DEFAULT_LOG "/tmp/log.json"
#define DEFAULT_HOST "ftp.myftphost.com"
#define DEFAULT_PORT 21
#define DEFAULT_PATH "/tmp/log.html"
#define DEFAULT_USER "myuser"

#define DATE_YEAR_START 2002
#define DATE_SEPARATOR ("-")
#define TIME_SEPARATOR (":")
#define PULSE_SEPARATOR ("/")

extern Shoe *getShoeById(int id);
extern Sport *getSportById(int id);
extern QList<Place*> getPlacesById(QList<int> ids);
extern QList<Weather*> getWeathersById(QList<int> ids);

class QAction;
class QMenu;
class QTextEdit;

class SIMPLETRAININGLOGLIB_EXPORT SimpleTrainingLogMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SimpleTrainingLogMainWindow();
    ~SimpleTrainingLogMainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newExercise();
    bool save();
    bool exportHTML();
    bool saveAs();
    void about();
    void documentWasModified();
    void setDirty(int row = -1, int column = -1);
    void ftpStateChanged(int state);
    void contextMenuForTable(const QPoint& pos);
    void deleteExercise();
    void viewExercise();
    void manageShoes();
    void manageSports();
    void managePlaces();
    void manageWeathers();
    void settings();

public slots:
    void editExercise(QTreeWidgetItem *item = 0, int column = 0);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadDatabase();
    void loadJsonDatabase();
    void setCurrentFile(const QString& fileName);
    QString strippedName(const QString& fullFileName);
    int getSportByName(const QString& sport) const;
    int getShoeByName(const QString& shoe) const;
    int getPlaceByName(const QString& place) const;
    int getWeatherByName(const QString& weather) const;
    void clear();

    const std::uint32_t DEFAULT_WIDTH = 900;
    const std::uint32_t DEFAULT_HEIGHT = 480;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *exportHTMLAct;
    QAction *m_shoesAct;
    QAction *m_sportsAct;
    QAction *m_placesAct;
    QAction *m_weathersAct;
    QAction *m_settingsAct;
    QTabWidget *m_mainTabWidget;

    //QFtp *m_ftp;
    ExerciseTable *m_exerciseTable;
    StatisticsHandler *m_statisticsHandler;
    bool m_dirty;  // Data was modified in the table.
    QString m_curConfig;
    QString m_curLog;
    QString m_curHost;
    QString m_curPort;
    QString m_curUser;
    QString m_curPath;
};
