#include "ExportDialog.h"
#include "ui_ExportDialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);

    slotUpload(Qt::Unchecked); // By default all fields are frozen.
    m_upload = false;
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::slotOk()
{
    accept();
}

void ExportDialog::slotCancel()
{
    reject();
}

void ExportDialog::setDefaultFile(const QString& file)
{
    // All widgets must exist at this point. Move the cursor to the
    // beginning. This is the field on focus.
    ui->fileLineEdit->setText(QString(file).append(tr(".html")));
    ui->fileLineEdit->setCursorPosition(0);
}

void ExportDialog::setDefaultHost(const QString& host)
{
    ui->hostLineEdit->setText(host);
    ui->hostLineEdit->setCursorPosition(0);
}

void ExportDialog::setDefaultPort(int port)
{
    ui->portLineEdit->setText(QString::number(port));
    ui->portLineEdit->setCursorPosition(0);
}

void ExportDialog::setDefaultUser(const QString& user)
{
    ui->userLineEdit->setText(user);
    ui->userLineEdit->setCursorPosition(0);
}

void ExportDialog::setDefaultPath(const QString& path)
{
    ui->pathLineEdit->setText(path);
    ui->pathLineEdit->setCursorPosition(0);
}

void ExportDialog::slotUpload(int state)
{
    switch (state) {
    case Qt::Unchecked:
        ui->hostLineEdit->setEnabled(false);
        ui->portLineEdit->setEnabled(false);
        ui->userLineEdit->setEnabled(false);
        ui->passwordLineEdit->setEnabled(false);
        ui->pathLineEdit->setEnabled(false);
        m_upload = false;
    case Qt::PartiallyChecked:
        break;
    case Qt::Checked:
        ui->hostLineEdit->setEnabled(true);
        ui->portLineEdit->setEnabled(true);
        ui->userLineEdit->setEnabled(true);
        ui->passwordLineEdit->setEnabled(true);
        ui->pathLineEdit->setEnabled(true);
        m_upload = true;
        break;
    default:
        break;
    }
}

QString ExportDialog::getFile() const { return ui->fileLineEdit->text(); }
QString ExportDialog::getHost() const { return ui->hostLineEdit->text(); }
int ExportDialog::getPort() const { return (ui->portLineEdit->text()).toInt(); }
QString ExportDialog::getUser() const { return ui->userLineEdit->text(); }
QString ExportDialog::getPassword() const { return ui->passwordLineEdit->text(); }
QString ExportDialog::getPath() const { return ui->pathLineEdit->text(); }
