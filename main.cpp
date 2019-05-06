#include "gui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUI w;
    w.show();
    w.setWindowTitle("FreeSurferGUI");
    return a.exec();
}
