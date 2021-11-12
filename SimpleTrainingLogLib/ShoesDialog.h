#pragma once

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
  explicit ShoesDialog(QWidget *parent, QList<Shoe*> *shoes, QList<Exercise*> *exercises);

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
