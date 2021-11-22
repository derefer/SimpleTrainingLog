#pragma once

#include <QDialog>

class QTreeWidgetItem;
class Shoe;
class Exercise;

namespace Ui {
class ShoesDialog;
}

class ShoesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShoesDialog(QWidget *parent, QList<Shoe*> *shoes, const QList<Exercise *>& exercises);
    ~ShoesDialog();

    inline bool isDirty() const { return m_dirty; }
    inline const QList<int>& getModifiedExercises() const { return m_modifiedExercises; }
    inline const QList<int>& getRemovedExercises() const { return m_removedExercises; }

  private slots:
    void slotOk();
    void slotAdd();
    void slotRemove();
    void slotSave();
    void slotCancel();
    void slotSetFieldsText(QTreeWidgetItem *item, int column);
    void slotDisableEnablePushButtons();

private:
    enum shoes_columns {
      COL_ID,
      COL_NAME,
      COL_BUY,
      COL_RUNNING_DISTANCE,
      COL_COMMENT
    };
    Ui::ShoesDialog *ui;
    QList<Shoe*> *m_shoes;
    QList<Exercise *> m_exercises;
    bool m_dirty;
    QList<int> m_modifiedExercises;
    QList<int> m_removedExercises;
};
