#include <stdlib.h>

#include <QApplication>
#include <QPainter>

#include "SimpleTrainingLogMainWindow.h"
#include "Diagram.h"

extern const char *months[];

Diagram::Diagram(QWidget *parent) : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

QSize Diagram::minimumSizeHint() const
{
    return QSize(DIAGRAM_WIDTH, DIAGRAM_HEIGHT);
}

QSize Diagram::sizeHint() const
{
    return QSize(DIAGRAM_WIDTH, DIAGRAM_HEIGHT);
}

void Diagram::drawBackground()
{
    QPainter painter(this);
    painter.setPen(Qt::gray);
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(MARGIN_LEFT, 0, MARGIN_LEFT, height() - MARGIN_BOTTOM);
    painter.drawLine(MARGIN_LEFT, height() - MARGIN_BOTTOM, width(),
                     height() - MARGIN_BOTTOM);
    painter.drawText(0, height() - MARGIN_BOTTOM, "0");
    for (int i = 1; i < getTicks() + 1; ++i) {
        painter.drawLine(MARGIN_LEFT, i * getTickHeight(), width(),
                         i * getTickHeight());
        painter.drawText(0, i * getTickHeight(),
            QString::number(getTicks() * getTickDistance() -
                            (i - 1) * getTickDistance()));
    }
    for (int i = 0; i < m_data.size(); ++i)
        painter.drawText(LABEL_INIT_OFFSET + i * LABEL_OFFSET, height()
            - LABEL_SPACING, months[i]);
}

void Diagram::updateBars(const QList<int>& monthlyDistances)
{
    m_data.clear();
    Q_ASSERT(monthlyDistances.size() == 12);
    for (int i = 0; i < monthlyDistances.size(); ++i)
        m_data.append(monthlyDistances.at(i));
    update();
}

int Diagram::maxData() const
{
    int max = -1;
    for (int i = 0; i < m_data.size(); ++i)
        if (m_data.at(i) > max)
            max = m_data.at(i);
    return max;
}

// Calculate distance per tick ratio to use on the diagram's vertical axis.
#define TICKS_LOW    20
#define TICKS_MEDIUM 50
#define TICKS_HIGH   100
int Diagram::getTickDistance() const
{
    int low_diff = abs(10 - maxData() / TICKS_LOW);
    int medium_diff = abs(10 - maxData() / TICKS_MEDIUM);
    int high_diff = abs(10 - maxData() / TICKS_HIGH);

    if (low_diff < medium_diff) {
        if (low_diff < high_diff) {
            return TICKS_LOW;
        } else {
            return TICKS_HIGH;
        }
    } else {
        if (medium_diff < high_diff) {
            return TICKS_MEDIUM;
        } else {
            return TICKS_HIGH;
        }
    }
}
#undef TICKS_LOW
#undef TICKS_MEDIUM
#undef TICKS_HIGH

int Diagram::getTicks() const
{
    // Make sure (+ 1) that we're higher than the highest bar of all.
    if (maxData() > 0) {
        return (maxData() / getTickDistance()) + 1;
    }
    return 0;
}

int Diagram::getTickHeight() const
{
    const double net_height = height() - MARGIN_BOTTOM;
    return (int)((double)net_height / ((double)getTicks() + 1.0) + 0.5);
}

void Diagram::drawBars()
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    for (int i = 0; i < m_data.size(); ++i) {
        QColor color(0x1d, 0x3d, 0x5e);
        painter.fillRect(LABEL_INIT_OFFSET + i * LABEL_OFFSET, height()
            - MARGIN_BOTTOM - (int)(m_data.at(i) * getTickHeight() /
            getTickDistance()), BAR_WIDTH, (int)m_data.at(i) *
            getTickHeight() / getTickDistance(), color);
        // Numbers above bars.
        if (!m_data.at(i))
            continue;
        QString label = QString::number(m_data.at(i));
        QFontMetrics metrics(QApplication::font());
        int x = LABEL_INIT_OFFSET + i * LABEL_OFFSET;
        int x_offset = (BAR_WIDTH - metrics.width(label)) / 2;
        int y = height() - MARGIN_BOTTOM;
        int y_offset = static_cast<int>(m_data.at(i) * getTickHeight() /
                                        getTickDistance());
        painter.drawText(x + x_offset, y - y_offset - LABEL_SPACING, label);
    }
}

// Called by update().  Modifications to exercise data should call update()
// with the new data as a SIGNAL/SLOT.
void Diagram::paintEvent(QPaintEvent *event __attribute__((unused)))
{
    drawBackground();
    drawBars();
}