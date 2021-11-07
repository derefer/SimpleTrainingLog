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

#ifndef DIAGRAM_H_
#define DIAGRAM_H_

#include <QWidget>

class Diagram : public QWidget
{
    Q_OBJECT

public:
    Diagram(QWidget *parent = NULL);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void updateBars(const QList<int>& monthlyDistances);

protected:
    void paintEvent(QPaintEvent *event);

private:
    enum {
      DIAGRAM_WIDTH = 360,
      DIAGRAM_HEIGHT = 300,
      BAR_WIDTH = 40,
      MARGIN_LEFT = 30,
      MARGIN_BOTTOM = 20,
      LABEL_INIT_OFFSET = 45,  // For month labels and bars.
      LABEL_OFFSET = 64,
      LABEL_SPACING = 5
    };

    QList<int> m_data;

    int maxData() const;
    int getTicks() const;
    int getTickDistance() const;
    int getTickHeight() const;
    void drawBackground();
    void drawBars();
};

#endif  // DIAGRAM_H_
