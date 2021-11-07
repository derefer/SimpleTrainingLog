#pragma once

#include <QDialog>
#include <QLineEdit>

class QLabel;
class QPushButton;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent);

    inline QString getLog() const { return m_logLineEdit->displayText(); }
    inline QString getHost() const { return m_hostLineEdit->displayText(); }
    inline QString getPort() const { return m_portLineEdit->displayText(); }
    inline QString getPath() const { return m_pathLineEdit->displayText(); }
    inline QString getUser() const { return m_userLineEdit->displayText(); }

    // All members should be initialized at this point.
    void setLog(const QString& log) { m_logLineEdit->setText(log); }
    void setHost(const QString& host) { m_hostLineEdit->setText(host); }
    void setPort(const QString& port) { m_portLineEdit->setText(port); }
    void setPath(const QString& path) { m_pathLineEdit->setText(path); }
    void setUser(const QString& user) { m_userLineEdit->setText(user); }

private slots:
    void slotOk();
    void slotCancel();

private:
    QLabel *m_logLabel;
    QLabel *m_hostLabel;
    QLabel *m_portLabel;
    QLabel *m_pathLabel;
    QLabel *m_userLabel;
    QLineEdit *m_logLineEdit;
    QLineEdit *m_hostLineEdit;
    QLineEdit *m_portLineEdit;
    QLineEdit *m_pathLineEdit;
    QLineEdit *m_userLineEdit;
    QPushButton *m_okPushButton;
    QPushButton *m_cancelPushButton;
};
