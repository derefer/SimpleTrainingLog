#pragma once

#include <QDialog>

#define COL_ID 0
#define COL_NAME 1

class QLabel;
class QLineEdit;
class QTreeWidget;
class QPushButton;
class QTreeWidgetItem;
class Weather;
class Exercise;

class WeathersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WeathersDialog(QWidget *parent, QList<Weather*> *weathers, QList<Exercise*> *exercises);

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

private:
    QList<Weather*> *m_weathers;
    QList<Exercise*> *m_exercises;

    bool m_dirty;
    QList<int> m_modifiedExercises;
    QList<int> m_removedExercises;

    QLabel *m_nameLabel;
    QLineEdit *m_nameLineEdit;
    QTreeWidget *m_weathersTreeWidget;
    QPushButton *m_okPushButton;
    QPushButton *m_addPushButton;
    QPushButton *m_removePushButton;
    QPushButton *m_savePushButton;
    QPushButton *m_cancelPushButton;
};
