#pragma once

#include <QDialog>

class QLabel;
class QLineEdit;
class QTreeWidget;
class QPushButton;
class QTreeWidgetItem;
class QPainter;
class Place;
class Exercise;

class PlacesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlacesDialog(QWidget *parent, QList<Place*> *places,
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

private:
    enum places_columns { COL_ID, COL_NAME };

    QList<Place*> *m_places;
    QList<Exercise*> *m_exercises;

    bool m_dirty;
    QList<int> m_modifiedExercises;
    QList<int> m_removedExercises;

    QLineEdit *m_nameLineEdit;
    QTreeWidget *m_placesTreeWidget;
    QPushButton *m_okPushButton;
    QPushButton *m_addPushButton;
    QPushButton *m_removePushButton;
    QPushButton *m_savePushButton;
    QPushButton *m_cancelPushButton;
};
