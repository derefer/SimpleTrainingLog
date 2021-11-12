#pragma once

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

class ExportDialog: public QDialog
{
    Q_OBJECT
public:
    explicit ExportDialog(QWidget *parent = nullptr);
    void setDefaultFile(const QString& file);
    void setDefaultHost(const QString& host);
    void setDefaultPort(int port);
    void setDefaultUser(const QString& user);
    void setDefaultPath(const QString& path);
    inline bool isUpload() const { return m_upload; }
    // Can't return a reference since QLineEdit returns a local copy.
    inline QString getFile() const { return m_fileLineEdit->text(); }
    inline QString getHost() const { return m_hostLineEdit->text(); }
    inline int getPort() const { return (m_portLineEdit->text()).toInt(); }
    inline QString getUser() const { return m_userLineEdit->text(); }
    inline QString getPassword() const { return m_passwordLineEdit->text(); }
    inline QString getPath() const { return m_pathLineEdit->text(); }

private slots:
    void slotOK();
    void slotCancel();
    void slotUpload(int state);

private:
    bool m_upload;  // Set if FTP upload is enabled.

    QLabel *m_fileLabel;
    QLabel *m_pathLabel;
    QLabel *m_hostLabel;
    QLabel *m_userLabel;
    QLabel *m_passwordLabel;
    QLabel *m_portLabel;
    QLineEdit *m_fileLineEdit;
    QLineEdit *m_pathLineEdit;
    QLineEdit *m_hostLineEdit;
    QLineEdit *m_portLineEdit;
    QLineEdit *m_userLineEdit;
    QLineEdit *m_passwordLineEdit;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
    QCheckBox *m_uploadCheckBox;
};
