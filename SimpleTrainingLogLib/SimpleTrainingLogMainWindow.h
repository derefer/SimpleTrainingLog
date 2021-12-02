#pragma once

#include <QMainWindow>
// QFtp was remvoved in Qt5.
//#include <QFtp>

#include "DataHandler.h"
#include "SimpleTrainingLogLibDecl.h"

class QTreeWidgetItem;

class ExerciseTable;
class StatisticsHandler;

namespace Ui {
class SimpleTrainingLogMainWindow;
}

class SIMPLETRAININGLOGLIB_EXPORT SimpleTrainingLogMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimpleTrainingLogMainWindow(QWidget *parent = nullptr);
    ~SimpleTrainingLogMainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newExercise();
    bool save();
    bool exportHtml();
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
    void aboutQt();

public slots:
    void editExercise(QTreeWidgetItem *item = 0, int column = 0);

private:
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadDatabase();
    void loadJsonDatabase();
    void setCurrentFile(const QString& fileName);
    QString strippedName(const QString& fullFileName);
    void clear();

    // Burned in database. Read these from configuration file. Password is always requested from the user.
    const QString DEFAULT_CONFIG = "./.SimpleTrainingLog";
    const QString DEFAULT_LOG = "/tmp/log.json";
    const QString DEFAULT_HOST = "ftp.myftphost.com";
    const std::uint32_t DEFAULT_PORT = 21;
    const QString DEFAULT_PATH = "/tmp/log.html";
    const QString DEFAULT_USER = "myuser";

    Ui::SimpleTrainingLogMainWindow *ui;
    QToolBar *fileToolBar;
    QTabWidget *m_mainTabWidget;
    //QFtp *m_ftp;
    ExerciseTable *m_exerciseTable;
    StatisticsHandler *m_statisticsHandler;
    bool m_dirty; // Data was modified in the table.
    QString m_curConfig;
    QString m_curLog;
    QString m_curHost;
    QString m_curPort;
    QString m_curUser;
    QString m_curPath;
    DataHandler dataHandler;
};
