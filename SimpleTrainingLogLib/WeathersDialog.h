#pragma once

#include <QDialog>

class QTreeWidgetItem;

class DataHandler;
class Exercise;
class Weather;

namespace Ui {
class WeathersDialog;
}

class WeathersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WeathersDialog(QWidget *parent, DataHandler *dataHandler);
    ~WeathersDialog();

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

    Ui::WeathersDialog *ui;
    bool m_dirty;
    QList<int> m_modifiedExercises;
    QList<int> m_removedExercises;
    DataHandler *dataHandler;
};
