#pragma once

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

    std::vector<int> m_data;

    int maxData() const;
    int getTicks() const;
    int getTickDistance() const;
    int getTickHeight() const;
    void drawBackground();
    void drawBars();
};
