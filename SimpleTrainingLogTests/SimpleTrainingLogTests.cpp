#include <gtest/gtest.h>

#include <QApplication>
#include <QSignalSpy>
#include <QTest>

#include "ktrainer.h"
#include "QtTypePrinters.h"

using namespace ::testing;

class SimpleTrainingLogTests : public Test
{
public:
    void SetUp()
    {
        // To create a SimpleTrainingLogMainWindow a QApplication is also needed.
        int argc = 0;
        char *argv[] = {};
        a = new QApplication(argc, argv);
    }

    void TearDown()
    {
        delete a;
    }

protected:

private:
    QApplication *a;
};

TEST_F(SimpleTrainingLogTests, dummyTest)
{
}

TEST_F(SimpleTrainingLogTests, testIfSimpleTrainingLogMainWindowCanBeInstantiated)
{
    // TODO: Fix!!!
    //KTrainer ktrainer;
}
