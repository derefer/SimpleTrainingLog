#pragma once

#include <QList>
#include <QStringList>
#include <QString>
#include <QColor>

// It is declared here.  All data types here are used by the lexer/parser.
#define YYSTYPE_IS_DECLARED 1
#define YYLTYPE_IS_DECLARED 1

// Semantic record for the parser propagated this way.  This is the semantic
// value of a given token.  That value is returned in yylval.  yylval's type
// is the union defined above.  Currently SimpleTrainingLog works only with integers.
// Floating point numbers are not supported yet.  Originally it was an "%union
// semrec { ... }" in the next section.  Include this header in
// exercisedatalexer.l and exercisedataparser.y.
typedef struct Result {
  int intval;  // Plain integers.
  QList<int> intlist;  // List of places.
  QString stringval;  // All string data.
} YYSTYPE;
// It's better to declare our own.
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;

class Exercise;

// Helper functions.
QString getShoeString(int id);
QString getSportString(int id);
QString getPlaceString(int id);
QString getWeatherString(int id);
QStringList getSportStrings();
QStringList getShoeStrings();
QStringList getPlaceStrings();
QStringList getWeatherStrings();
int getShoeId(const QString& name);
int getSportId(const QString& name);
int getPlaceId(const QString& name);
int getWeatherId(const QString& name);
void removeSportById(int id);
void removeShoeById(int id);
void removeExerciseById(int id);
void removePlaceById(int id);
void removeWeatherById(int id);
Exercise *getExerciseById(int id);

QString encodeComment(const QString& comment);
QString decodeComment(const QString& comment);

class Shoe
{
public:
    // Element of the shoe database.
    explicit Shoe(int id, QString name, int buy, QString comment)
        : m_id(id), m_name(name), m_buy(buy), m_comment(comment) { }

    inline int getId() const { return m_id; }
    inline QString getName() const { return m_name; }
    inline int getBuy() const { return m_buy; }
    inline QString getComment() const { return m_comment; }
    void setName(QString name) { m_name = name; }
    void setBuy(int buy) { m_buy = buy; }
    void setComment(QString comment) { m_comment = comment; }

private:
    int m_id;
    QString m_name;
    int m_buy;  // Year only.
    QString m_comment;
};

class Sport
{
public:
    explicit Sport(int id, QString name, QColor color = Qt::white)
      : m_id(id), m_name(name), m_color(color) { }

    inline int getId() const { return m_id; }
    inline QString getName() const { return m_name; }
    inline QColor getColor() const { return m_color; }
    void setName(QString name) { m_name = name; }
    void setColor(QColor color) { m_color = color; }
private:
    int m_id;
    QString m_name;
    QColor m_color;
};

class Exercise
{
public:
    explicit Exercise(int id) : m_id(id) { }

    inline int getId() const { return m_id; }
    inline QString getDate() const { return m_date; }
    inline QString getTime() const { return m_time; }
    inline int getDistance() const { return m_distance; }
    inline QString getDuration() const { return m_duration; }
    inline int getSport() const { return m_sport; }
    inline QList<int> getPlaces() const { return m_places; }
    inline int getShoe() const { return m_shoe; }
    inline QString getComment() const { return m_comment; }
    inline QList<int> getWeathers() const { return m_weathers; }
    inline int getMaxPulse() const { return m_maxPulse; }
    inline int getAvgPulse() const { return m_avgPulse; }
    inline int getCal() const { return m_cal; }
    inline int getFat() const { return m_fat; }
    inline double getDurationHours() const { return (m_duration.split(":")[0].toDouble() * 60.0 + m_duration.split(":")[1].toDouble()) / 60.0; }
    inline double getSpeed() const { return static_cast<double>(m_distance) / getDurationHours(); }
    void setId(int id) { m_id = id; }
    void setDate(QString date) { m_date = date; }
    void setTime(QString time) { m_time = time; }
    void setDistance(int distance) { m_distance = distance; }
    void setDuration(QString duration) { m_duration = duration; }
    void setSport(int sport) { m_sport = sport; }
    void setPlaces(QList<int> ids);
    void clearPlaces() { m_places.clear(); }
    void addPlace(int id);
    void setShoe(int shoe) { m_shoe = shoe; }
    void setComment(QString comment) { m_comment = comment; }
    void setMaxPulse(int maxPulse) { m_maxPulse = maxPulse; }
    void setAvgPulse(int avgPulse) { m_avgPulse = avgPulse; }
    void setCal(int cal) { m_cal = cal; }
    void setFat(int fat) { m_fat = fat; }
    void setWeathers(QList<int> ids);
    void clearWeathers() { m_weathers.clear(); }
    void addWeather(int id);

private:
    int m_id;
    QString m_date;
    QString m_time;
    int m_distance;
    QString m_duration;
    int m_sport;  // Reference to the "Sport" table.
    QList<int> m_places;  // References to the "Place" table.
    int m_shoe;  // Reference to the "Shoe" table.
    QString m_comment;
    QList<int> m_weathers;
    int m_maxPulse;
    int m_avgPulse;
    int m_cal;
    int m_fat;
};

class Place
{
public:
  explicit Place(int id, QString name) : m_id(id), m_lat(-1), m_lon(-1), m_name(name) { }
  inline int getId() const { return m_id; }
  inline QString getName() const { return m_name; }
  void setName(QString name) { m_name = name; }
private:
  int m_id;
  double m_lat;
  double m_lon;
  QString m_name;
};

class Weather
{
public:
    explicit Weather(int id, QString name) : m_id(id), m_name(name) { }

    inline int getId() const { return m_id; }
    inline QString getName() const { return m_name; }
    void setName(QString name) { m_name = name; }

private:
    int m_id;
    QString m_name;
};
