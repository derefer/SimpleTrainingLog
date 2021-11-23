#include <cstdlib>

#include <QApplication>
#include <QPainter>

#include "DataHandler.h"
#include "Diagram.h"
#include "SimpleTrainingLogMainWindow.h"

Diagram::Diagram(QWidget *parent, DataHandler *dataHandler) : QWidget(parent), dataHandler(dataHandler)
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
    painter.drawLine(MARGIN_LEFT, height() - MARGIN_BOTTOM, width(), height() - MARGIN_BOTTOM);
    painter.drawText(0, height() - MARGIN_BOTTOM, "0");
    for (int i = 1; i < getTicks() + 1; ++i) {
        painter.drawLine(MARGIN_LEFT, i * getTickHeight(), width(), i * getTickHeight());
        painter.drawText(0, i * getTickHeight(), QString::number(getTicks() * getTickDistance() - (i - 1) * getTickDistance()));
    }
    for (std::size_t i = 0; i < m_data.size(); i++) {
        painter.drawText(LABEL_INIT_OFFSET + i * LABEL_OFFSET, height() - LABEL_SPACING, dataHandler->months[i]);
    }
}

void Diagram::updateBars(const QList<int>& monthlyDistances)
{
    m_data.clear();
    Q_ASSERT(monthlyDistances.size() == 12);
    for (int i = 0; i < monthlyDistances.size(); ++i) {
        m_data.push_back(monthlyDistances.at(i));
    }
    update();
}

int Diagram::maxData() const
{
    return *std::max_element(m_data.begin(), m_data.end());
}

// Calculate distance per tick ratio to use on the diagram's vertical axis.
int Diagram::getTickDistance() const
{
    const std::int32_t TICKS_LOW = 20;
    const std::int32_t TICKS_MEDIUM = 50;
    const std::int32_t TICKS_HIGH = 100;
    int low_diff = std::abs(10 - maxData() / TICKS_LOW);
    int medium_diff = std::abs(10 - maxData() / TICKS_MEDIUM);
    int high_diff = std::abs(10 - maxData() / TICKS_HIGH);

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
    for (std::size_t i = 0; i < m_data.size(); ++i) {
        QColor color(0x1d, 0x3d, 0x5e);
        painter.fillRect(LABEL_INIT_OFFSET + i * LABEL_OFFSET, height()
            - MARGIN_BOTTOM - (int)(m_data[i] * getTickHeight() /
            getTickDistance()), BAR_WIDTH, (int)m_data[i] *
            getTickHeight() / getTickDistance(), color);
        // Numbers above bars.
        if (m_data[i] == 0) {
            continue;
        }
        QString label = QString::number(m_data[i]);
        QFontMetrics metrics(QApplication::font());
        int x = LABEL_INIT_OFFSET + i * LABEL_OFFSET;
        int x_offset = (BAR_WIDTH - metrics.width(label)) / 2;
        int y = height() - MARGIN_BOTTOM;
        int y_offset = static_cast<int>(m_data[i] * getTickHeight() / getTickDistance());
        painter.drawText(x + x_offset, y - y_offset - LABEL_SPACING, label);
    }
}

// Called by update(). Modifications to exercise data should call update()
// with the new data as a SIGNAL/SLOT.
void Diagram::paintEvent(QPaintEvent *)
{
    drawBackground();
    drawBars();
}
