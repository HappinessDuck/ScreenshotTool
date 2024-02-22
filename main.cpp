#include "widget.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);//防止应用程序在最后一个窗口关闭时终止。
    Widget widget;
    widget.hide();
//        widget.show();


    return app.exec();
}
