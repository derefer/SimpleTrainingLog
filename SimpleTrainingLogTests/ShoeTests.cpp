#include <gtest/gtest.h>

#include "QtTypePrinters.h"
#include "Shoe.h"

using namespace ::testing;

TEST(ShoeTests, testInitializationOfTheShoeClass)
{
    const int shoeId = 0;
    const QString shoeName = "Nike Vomero 11";
    const int shoeBuy = 2018;
    const QString shoeComment = "A very nice shoe...";

    Shoe shoe(shoeId, shoeName, shoeBuy, shoeComment);

    ASSERT_EQ(shoe.getId(), shoeId);
    ASSERT_EQ(shoe.getName(), shoeName);
    ASSERT_EQ(shoe.getBuy(), shoeBuy);
    ASSERT_EQ(shoe.getComment(), shoeComment);
}

TEST(ShoeTests, testTheSetterFunctionsOfTheShoeClass)
{
    const int shoeId = 0;
    const QString shoeName = "Nike Pegasus 36";
    const int shoeBuy = 2021;
    const QString shoeComment = "Another nice shoe...";

    Shoe shoe(0, "Nike Vomero 11", 2018, "A very nice shoe...");

    ASSERT_NO_THROW(shoe.setName(shoeName));
    ASSERT_NO_THROW(shoe.setBuy(shoeBuy));
    ASSERT_NO_THROW(shoe.setComment(shoeComment));

    ASSERT_EQ(shoe.getId(), shoeId);
    ASSERT_EQ(shoe.getName(), shoeName);
    ASSERT_EQ(shoe.getBuy(), shoeBuy);
    ASSERT_EQ(shoe.getComment(), shoeComment);
}
