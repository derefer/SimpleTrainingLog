#pragma once

#include <QSet>
#include <QString>

class Exercise
{
public:
    explicit Exercise(int id) : m_id(id) { }

    inline int getId() const { return m_id; }
    inline const QString& getDate() const { return m_date; }
    inline const QString& getTime() const { return m_time; }
    inline int getDistance() const { return m_distance; }
    inline const QString& getDuration() const { return m_duration; }
    inline int getSport() const { return m_sport; }
    inline const QSet<int>& getPlaces() const { return m_places; }
    inline int getShoe() const { return m_shoe; }
    inline const QString& getComment() const { return m_comment; }
    inline const QSet<int>& getWeathers() const { return m_weathers; }
    inline int getMaxPulse() const { return m_maxPulse; }
    inline int getAvgPulse() const { return m_avgPulse; }
    inline int getCal() const { return m_cal; }
    inline int getFat() const { return m_fat; }
    inline double getDurationHours() const { return !m_duration.isEmpty() ? (m_duration.split(":")[0].toDouble() * 60.0 + m_duration.split(":")[1].toDouble()) / 60.0 : 0.0; }
    inline double getSpeed() const { return static_cast<double>(m_distance) / getDurationHours(); }
    void setId(int id) { m_id = id; }
    void setDate(const QString& date) { m_date = date; }
    void setTime(const QString& time) { m_time = time; }
    void setDistance(int distance) { m_distance = distance; }
    void setDuration(const QString& duration) { m_duration = duration; }
    void setSport(int sport) { m_sport = sport; }
    void addPlaces(const QSet<int>& newPlaceIds);
    void clearPlaces() { m_places.clear(); }
    void addPlace(int id);
    void setShoe(int shoe) { m_shoe = shoe; }
    void setComment(const QString& comment) { m_comment = comment; }
    void setMaxPulse(int maxPulse) { m_maxPulse = maxPulse; }
    void setAvgPulse(int avgPulse) { m_avgPulse = avgPulse; }
    void setCal(int cal) { m_cal = cal; }
    void setFat(int fat) { m_fat = fat; }
    void addWeathers(const QSet<int>& newWeatherIds);
    void clearWeathers() { m_weathers.clear(); }
    void addWeather(int id);

private:
    int m_id;
    QString m_date;
    QString m_time;
    int m_distance;
    QString m_duration;
    int m_sport;
    QSet<int> m_places;
    int m_shoe;
    QString m_comment;
    QSet<int> m_weathers;
    int m_maxPulse;
    int m_avgPulse;
    int m_cal;
    int m_fat;
};
