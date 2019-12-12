#include <QGuiApplication>

#include <QtQuick/QQuickView>

#include "QQuickOpenGL.h"

//! [1]
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<QQuickOpenGL>("QQuickOpenGL", 1, 0, "QQuickOpenGL");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///main.qml"));
    view.show();

    return app.exec();
}
//! [1]
