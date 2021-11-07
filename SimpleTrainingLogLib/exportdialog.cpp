#include "exportdialog.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>

ExportDialog::ExportDialog(QWidget *parent) : QDialog(parent)
{
    m_fileLabel = new QLabel(tr("File:"));
    m_fileLineEdit = new QLineEdit;
    QHBoxLayout *fileLayout = new QHBoxLayout;
    fileLayout->addWidget(m_fileLabel);
    fileLayout->addWidget(m_fileLineEdit);

    m_okButton = new QPushButton(tr("OK"));
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(slotOK()));
    m_cancelButton = new QPushButton(tr("Cancel"));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(slotCancel()));
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);

    m_uploadCheckBox = new QCheckBox(tr("Upload to FTP site"));
    connect(m_uploadCheckBox, SIGNAL(stateChanged(int)), this,
        SLOT(slotUpload(int)));
    m_hostLabel = new QLabel(tr("Host:"));
    m_hostLineEdit = new QLineEdit;
    m_portLabel = new QLabel(tr("Port:"));
    m_portLineEdit = new QLineEdit;
    m_userLabel = new QLabel(tr("User:"));
    m_userLineEdit = new QLineEdit;
    m_passwordLabel = new QLabel(tr("Password:"));
    m_passwordLineEdit = new QLineEdit;
    m_passwordLineEdit->setEchoMode(QLineEdit::Password);
    m_pathLabel = new QLabel("Path:");
    m_pathLineEdit = new QLineEdit;
    QGridLayout *uploadLayout = new QGridLayout;
    uploadLayout->addWidget(m_hostLabel, 0, 0);
    uploadLayout->addWidget(m_hostLineEdit, 0, 1);
    uploadLayout->addWidget(m_portLabel, 1, 0);
    uploadLayout->addWidget(m_portLineEdit, 1, 1);
    uploadLayout->addWidget(m_pathLabel, 3, 0);
    uploadLayout->addWidget(m_pathLineEdit, 3, 1);
    uploadLayout->addWidget(m_userLabel, 4, 0);
    uploadLayout->addWidget(m_userLineEdit, 4, 1);
    uploadLayout->addWidget(m_passwordLabel, 5, 0);
    uploadLayout->addWidget(m_passwordLineEdit, 5, 1);
    QGroupBox *uploadGroupBox = new QGroupBox(tr("FTP Options"));
    uploadGroupBox->setLayout(uploadLayout);
    slotUpload(Qt::Unchecked);  // By default all fields are frozen.

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(fileLayout);
    layout->addWidget(m_uploadCheckBox);
    layout->addWidget(uploadGroupBox);
    layout->addLayout(buttonLayout);
    m_upload = false;
    setLayout(layout);

    setWindowTitle(tr("Export & Publishing"));
}

void ExportDialog::slotOK()
{
    accept();
}

void ExportDialog::slotCancel()
{
    reject();
}

void ExportDialog::setDefaultFile(const QString& file)
{
    // All widgets must exist at this point.  Move the cursor to the
    // beginning.  This is the field on focus.
    m_fileLineEdit->setText(QString(file).append(tr(".html")));
    m_fileLineEdit->setCursorPosition(0);
}

void ExportDialog::setDefaultHost(const QString& host)
{
    m_hostLineEdit->setText(host);
    m_hostLineEdit->setCursorPosition(0);
}

void ExportDialog::setDefaultPort(int port)
{
    m_portLineEdit->setText(QString::number(port));
    m_portLineEdit->setCursorPosition(0);
}

void ExportDialog::setDefaultUser(const QString& user)
{
    m_userLineEdit->setText(user);
    m_userLineEdit->setCursorPosition(0);
}

void ExportDialog::setDefaultPath(const QString& path)
{
    m_pathLineEdit->setText(path);
    m_pathLineEdit->setCursorPosition(0);
}

void ExportDialog::slotUpload(int state)
{
    switch (state) {
    case Qt::Unchecked:
        m_hostLineEdit->setEnabled(false);
        m_portLineEdit->setEnabled(false);
        m_userLineEdit->setEnabled(false);
        m_passwordLineEdit->setEnabled(false);
        m_pathLineEdit->setEnabled(false);
        m_upload = false;
    case Qt::PartiallyChecked:
        break;
    case Qt::Checked:
        m_hostLineEdit->setEnabled(true);
        m_portLineEdit->setEnabled(true);
        m_userLineEdit->setEnabled(true);
        m_passwordLineEdit->setEnabled(true);
        m_pathLineEdit->setEnabled(true);
        m_upload = true;
        break;
    default:
        break;
    }
}
