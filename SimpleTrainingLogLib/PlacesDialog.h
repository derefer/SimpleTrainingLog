#pragma once

#include <QDialog>

class QTreeWidgetItem;

class Exercise;
class Place;

namespace Ui {
class PlacesDialog;
}

class PlacesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlacesDialog(QWidget *parent, QList<Place*> *places, QList<Exercise*> *exercises);
    ~PlacesDialog();

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

private:
    const std::uint32_t COL_ID = 0;
    const std::uint32_t COL_NAME = 1;

    Ui::PlacesDialog *ui;

    QList<Place*> *m_places;
    QList<Exercise*> *m_exercises;
    bool m_dirty;
    QList<int> m_modifiedExercises;
    QList<int> m_removedExercises;
};
