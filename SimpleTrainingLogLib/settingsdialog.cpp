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

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "settingsdialog.h"

// On an error like this:
// settingsdialog.o: In function `SettingsDialog::SettingsDialog(QWidget*)':
// settingsdialog.cpp:(.text+0x1e): undefined reference to `vtable for SettingsDialog'
// settingsdialog.cpp:(.text+0x26): undefined reference to `vtable for SettingsDialog'
// settingsdialog.cpp:(.text+0x32): undefined reference to `SettingsDialog::staticMetaObject'
//
// Regenerate all of your makefiles.  http://lists.trolltech.com/qt-interest/
// 2007-06/thread01171-0.html
SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent)
{
    m_logLabel = new QLabel(tr("Log:"));
    m_hostLabel = new QLabel(tr("Host:"));
    m_portLabel = new QLabel(tr("Port:"));
    m_pathLabel = new QLabel(tr("Path:"));
    m_userLabel = new QLabel(tr("User:"));
    m_logLineEdit = new QLineEdit;
    m_hostLineEdit = new QLineEdit;
    m_portLineEdit = new QLineEdit;
    m_pathLineEdit = new QLineEdit;
    m_userLineEdit = new QLineEdit;

    QVBoxLayout *labelLayout = new QVBoxLayout;
    labelLayout->addWidget(m_logLabel);
    labelLayout->addWidget(m_hostLabel);
    labelLayout->addWidget(m_portLabel);
    labelLayout->addWidget(m_pathLabel);
    labelLayout->addWidget(m_userLabel);

    QVBoxLayout *lineEditLayout = new QVBoxLayout;
    lineEditLayout->addWidget(m_logLineEdit);
    lineEditLayout->addWidget(m_hostLineEdit);
    lineEditLayout->addWidget(m_portLineEdit);
    lineEditLayout->addWidget(m_pathLineEdit);
    lineEditLayout->addWidget(m_userLineEdit);

    QHBoxLayout *dataLayout = new QHBoxLayout;
    dataLayout->addLayout(labelLayout);
    dataLayout->addLayout(lineEditLayout);

    m_okPushButton = new QPushButton(tr("OK"));
    m_cancelPushButton = new QPushButton(tr("Cancel"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_okPushButton);
    buttonLayout->addWidget(m_cancelPushButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(dataLayout);
    layout->addLayout(buttonLayout);

    setLayout(layout);

    setWindowTitle(tr("Settings"));

    connect(m_okPushButton, SIGNAL(clicked()), this, SLOT(slotOk()));
    connect(m_cancelPushButton, SIGNAL(clicked()), this, SLOT(slotCancel()));
}

void SettingsDialog::slotOk()
{
    accept();
}

void SettingsDialog::slotCancel()
{
    reject();
}
