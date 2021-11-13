#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, const QString& log, const QString& host, const QString& port, const QString& path, const QString& user) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->logLineEdit->setText(log);
    ui->hostLineEdit->setText(host);
    ui->portLineEdit->setText(port);
    ui->pathLineEdit->setText(path);
    ui->userLineEdit->setText(user);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

QString SettingsDialog::getLog() const { return ui->logLineEdit->displayText(); }
QString SettingsDialog::getHost() const { return ui->hostLineEdit->displayText(); }
QString SettingsDialog::getPort() const { return ui->portLineEdit->displayText(); }
QString SettingsDialog::getPath() const { return ui->pathLineEdit->displayText(); }
QString SettingsDialog::getUser() const { return ui->userLineEdit->displayText(); }

void SettingsDialog::slotOk()
{
    accept();
}

void SettingsDialog::slotCancel()
{
    reject();
}
