/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Ferenc Kovács                                   *
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

#ifndef SPORTSDIALOG_H_
#define SPORTSDIALOG_H_

#include <QDialog>

class QLabel;
class QLineEdit;
class QTreeWidget;
class QPushButton;
class QTreeWidgetItem;
class Sport;
class Exercise;

class SportsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SportsDialog(QWidget *parent, QList<Sport*> *sports,
        QList<Exercise*> *exercises);

    inline bool isDirty() const { return m_dirty; }
    inline const QList<int>& getModifiedExercises() const
        { return m_modifiedExercises; }
    inline const QList<int>& getRemovedExercises() const
        { return m_removedExercises; }

private slots:
    void slotOk();
    void slotAdd();
    void slotRemove();
    void slotSave();
    void slotCancel();
    void slotSetNameText(QTreeWidgetItem *item, int column);
    void slotSetColor();

private:
    enum sports_columns { COL_ID, COL_NAME, COL_COLOR };

    QList<Sport*> *m_sports;
    QList<Exercise*> *m_exercises;

    bool m_dirty;
    QList<int> m_modifiedExercises;
    QList<int> m_removedExercises;

    QLabel *m_nameLabel;
    QLabel *m_colorLabel;
    QLineEdit *m_nameLineEdit;
    QLineEdit *m_colorLineEdit;
    QTreeWidget *m_sportsTreeWidget;
    QPushButton *m_okPushButton;
    QPushButton *m_addPushButton;
    QPushButton *m_removePushButton;
    QPushButton *m_savePushButton;
    QPushButton *m_cancelPushButton;
    QPushButton *m_colorPushButton;
};

#endif  // SPORTSDIALOG_H_
