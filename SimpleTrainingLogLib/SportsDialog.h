#pragma once

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
