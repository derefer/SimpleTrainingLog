#pragma once

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent, const QString& log, const QString& host, const QString& port, const QString& path, const QString& user);
    ~SettingsDialog();

    QString getLog() const;
    QString getHost() const;
    QString getPort() const;
    QString getPath() const;
    QString getUser() const;

private slots:
    void slotOk();
    void slotCancel();

private:
    Ui::SettingsDialog *ui;
};
