/***************************************************************************
 *   Copyright (C) 2008, 2009 by Ferenc Kovács                             *
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

#ifndef EXPORTDIALOG_H_
#define EXPORTDIALOG_H_

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

class ExportDialog: public QDialog
{
public:
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = NULL);
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

#endif  // EXPORTDIALOG_H_
