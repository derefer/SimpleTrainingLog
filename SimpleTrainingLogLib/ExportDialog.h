#pragma once

#include <QDialog>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = nullptr);
    ~ExportDialog();

    void setDefaultFile(const QString& file);
    void setDefaultHost(const QString& host);
    void setDefaultPort(int port);
    void setDefaultUser(const QString& user);
    void setDefaultPath(const QString& path);
    inline bool isUpload() const { return m_upload; }
    // Can't return a reference since QLineEdit returns a local copy.
    QString getFile() const;
    QString getHost() const;
    int getPort() const;
    QString getUser() const;
    QString getPassword() const;
    QString getPath() const;

private slots:
    void slotOk();
    void slotCancel();
    void slotUpload(int state);

private:
    Ui::ExportDialog *ui;
    bool m_upload; // Set if FTP upload is enabled.
};
