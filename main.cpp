#include <QApplication>
#include "qutmainwindow.h"

int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    Pile p;
    Manager m(p);
    QUTMainWindow window(p,m);
    window.show();
    return app.exec();
}
