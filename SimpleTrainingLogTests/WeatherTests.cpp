#include <gtest/gtest.h>

#include "QtTypePrinters.h"
#include "Weather.h"

using namespace ::testing;

TEST(WeatherTests, testInitializationOfThePlaceClass)
{
    const int placeId = 0;
    const QString placeName = "Rainy";

    Weather weather(placeId, placeName);

    ASSERT_EQ(weather.getId(), placeId);
    ASSERT_EQ(weather.getName(), placeName);
}

TEST(WeatherTests, testTheSetterFunctionsOfThePlaceClass)
{
    const int placeId = 0;
    const QString placeName = "Sunny";

    Weather weather(0, "Rainy");

    ASSERT_NO_THROW(weather.setName(placeName));

    ASSERT_EQ(weather.getId(), placeId);
    ASSERT_EQ(weather.getName(), placeName);
}
