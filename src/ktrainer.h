/***************************************************************************
 *   Copyright (C) 2008, 2009 by Ferenc Kovács                             *
 *   derefer@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef KTRAINER_H_
#define KTRAINER_H_

#include <QMainWindow>
#include <QCloseEvent>
// QFtp was remvoved in Qt5.
//#include <QFtp>

#include "exercisetable.h"
#include "statisticshandler.h"

#define DEFAULT_WIDTH 900
#define DEFAULT_HEIGHT 480

// Burned in database.  Read these from configuration file.  Password is
// always requested from the user.
#define DEFAULT_CONFIG "./.SimpleTrainingLog"
#define DEFAULT_LOG "/tmp/log"
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

class KTrainer : public QMainWindow
{
    Q_OBJECT

public:
    KTrainer();
    ~KTrainer();

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
    void loadDatabase(const QString& fileName);
    void setCurrentFile(const QString& fileName);
    QString strippedName(const QString& fullFileName);
    int parseFile(const char *fileName);
    int getSportByName(const QString& sport) const;
    int getShoeByName(const QString& shoe) const;
    int getPlaceByName(const QString& place) const;
    int getWeatherByName(const QString& weather) const;
    void clear();

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

#endif  // KTRAINER_H_
