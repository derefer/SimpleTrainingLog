#include <QApplication>

#include "ktrainer.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(SimpleTrainingLog);
    QApplication app(argc, argv);
    KTrainer mw;
    mw.show();
    return app.exec();
}
