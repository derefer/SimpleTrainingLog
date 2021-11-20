#pragma once

#include <QString>

class Shoe
{
public:
    explicit Shoe(int id, const QString& name, int buy, const QString& comment) : m_id(id), m_name(name), m_buy(buy), m_comment(comment) { }

    inline int getId() const { return m_id; }
    inline const QString& getName() const { return m_name; }
    inline int getBuy() const { return m_buy; }
    inline const QString& getComment() const { return m_comment; }
    void setName(const QString& name) { m_name = name; }
    void setBuy(int buy) { m_buy = buy; }
    void setComment(const QString& comment) { m_comment = comment; }

private:
    int m_id;
    QString m_name;
    int m_buy; // Year only.
    QString m_comment;
};
