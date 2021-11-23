#pragma once

#include <QDialog>

class QTreeWidgetItem;

class DataHandler;
class Exercise;
class Sport;

namespace Ui {
class SportsDialog;
}

class SportsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SportsDialog(QWidget *parent, DataHandler *dataHandler);
    ~SportsDialog();

    inline bool isDirty() const { return m_dirty; }
    inline const QList<int>& getModifiedExercises() const { return m_modifiedExercises; }
    inline const QList<int>& getRemovedExercises() const { return m_removedExercises; }

private slots:
    void slotOk();
    void slotAdd();
    void slotRemove();
    void slotSave();
    void slotCancel();
    void slotSetNameText(QTreeWidgetItem *item, int column);
    void slotSetColor();

private:
    const std::uint32_t COL_ID = 0;
    const std::uint32_t COL_NAME = 1;
    const std::uint32_t COL_COLOR = 2;

    Ui::SportsDialog *ui;
    QList<Sport*> *m_sports;
    QList<Exercise *> m_exercises;
    bool m_dirty;
    QList<int> m_modifiedExercises;
    QList<int> m_removedExercises;
    DataHandler *dataHandler;
};
