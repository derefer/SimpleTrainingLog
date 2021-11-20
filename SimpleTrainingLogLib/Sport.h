#pragma once

#include <QColor>
#include <QString>

class Sport
{
public:
    explicit Sport(int id, const QString& name, const QColor& color = Qt::white) : m_id(id), m_name(name), m_color(color) { }

    inline int getId() const { return m_id; }
    inline const QString& getName() const { return m_name; }
    inline const QColor& getColor() const { return m_color; }
    void setName(const QString& name) { m_name = name; }
    void setColor(const QColor& color) { m_color = color; }

private:
    int m_id;
    QString m_name;
    QColor m_color;
};
