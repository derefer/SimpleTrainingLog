/***************************************************************************
 *   Copyright (C) 2008, 2009, 2010 by Ferenc Kovács                       *
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

#ifndef SHOESDIALOG_H_
#define SHOESDIALOG_H_

#include <QDialog>

class QLabel;
class QLineEdit;
class QTextEdit;
class QTreeWidget;
class QPushButton;
class QTreeWidgetItem;
class Shoe;
class Exercise;

class ShoesDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ShoesDialog(QWidget *parent, QList<Shoe*> *shoes,
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
  void slotSetFieldsText(QTreeWidgetItem *item, int column);
  void slotDisableEnablePushButtons();

private:
  enum shoes_columns
  {
    COL_ID,
    COL_NAME,
    COL_BUY,
    COL_RUNNING_DISTANCE,
    COL_COMMENT
  };

  QList<Shoe*> *m_shoes;
  QList<Exercise*> *m_exercises;

  bool m_dirty;
  QList<int> m_modifiedExercises;
  QList<int> m_removedExercises;

  QLabel *m_nameLabel;
  QLabel *m_buyLabel;
  QLabel *m_commentLabel;
  QLineEdit *m_nameLineEdit;
  QLineEdit *m_buyLineEdit;
  QTextEdit *m_commentTextEdit;
  QTreeWidget *m_shoesTreeWidget;
  QPushButton *m_okPushButton;
  QPushButton *m_addPushButton;
  QPushButton *m_removePushButton;
  QPushButton *m_savePushButton;
  QPushButton *m_cancelPushButton;
};

#endif  // SHOESDIALOG_H_

