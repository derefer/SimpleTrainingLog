/***************************************************************************
 *   Copyright (C) 2008 by Ferenc Kovács                                   *
 *   derefer@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef SETTINGSDIALOG_H_
#define SETTINGSDIALOG_H_

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

#endif  // SETTINGSDIALOG_H_
