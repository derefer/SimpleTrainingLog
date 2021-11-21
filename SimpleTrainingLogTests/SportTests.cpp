#include <gtest/gtest.h>

#include "QtTypePrinters.h"
#include "Sport.h"

using namespace ::testing;

TEST(SportTests, testDefaultInitializationOfTheSportClass)
{
    const int sportId = 0;
    const QString sportName = "Running";
    const QColor sportColor = Qt::white;

    Sport sport(sportId, sportName);

    ASSERT_EQ(sport.getId(), sportId);
    ASSERT_EQ(sport.getName(), sportName);
    ASSERT_EQ(sport.getColor(), sportColor);
}

TEST(SportTests, testNonDefaultInitializationOfTheSportClass)
{
    const int sportId = 0;
    const QString sportName = "Running";
    const QColor sportColor = Qt::black;

    Sport sport(sportId, sportName, sportColor);

    ASSERT_EQ(sport.getId(), sportId);
    ASSERT_EQ(sport.getName(), sportName);
    ASSERT_EQ(sport.getColor(), sportColor);
}

TEST(SportTests, testTheSetterFunctionsOfTheSportClass)
{
    const int sportId = 0;
    const QString sportName = "Hiking";
    const QColor sportColor = Qt::black;

    Sport sport(0, "Running");

    ASSERT_NO_THROW(sport.setName(sportName));
    ASSERT_NO_THROW(sport.setColor(sportColor));

    ASSERT_EQ(sport.getId(), sportId);
    ASSERT_EQ(sport.getName(), sportName);
    ASSERT_EQ(sport.getColor(), sportColor);
}
