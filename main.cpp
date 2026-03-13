#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Determinant Calculator");
    MainWindow w;
    w.show();
    return app.exec();
}
