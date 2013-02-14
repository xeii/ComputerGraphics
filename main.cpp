#include <QtGui/QApplication>
#include "widget.h";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget widget;
    widget.resize(800, 600);
    widget.show();
    return a.exec();
}
