#include <QApplication>

#include "SimpleTrainingLogMainWindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(SimpleTrainingLog);
    QApplication app(argc, argv);
    SimpleTrainingLogMainWindow mw;
    mw.show();
    return app.exec();
}
