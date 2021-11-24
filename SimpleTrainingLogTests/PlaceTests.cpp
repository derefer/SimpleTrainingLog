#include <gtest/gtest.h>

#include "QtTypePrinters.h"
#include "Place.h"

using namespace ::testing;

TEST(PlaceTests, testInitializationOfThePlaceClass)
{
    const int placeId = 0;
    const QString placeName = "Forest";

    Place sport(placeId, placeName);

    ASSERT_EQ(sport.getId(), placeId);
    ASSERT_EQ(sport.getName(), placeName);
}

TEST(PlaceTests, testTheSetterFunctionsOfThePlaceClass)
{
    const int placeId = 0;
    const QString placeName = "Park";

    Place place(0, "Forest");

    ASSERT_NO_THROW(place.setName(placeName));

    ASSERT_EQ(place.getId(), placeId);
    ASSERT_EQ(place.getName(), placeName);
}
