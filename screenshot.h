#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QtQuick/QQuickItem>
#include <QImage>

class ScreenShot : public QQuickItem
{
    Q_OBJECT
//    Q_PROPERTY(QImage echoImage READ ehcoImage WRITE setEchoImage NOTIFY imageUpdated)

public:
    explicit ScreenShot();

    QImage ehcoImage();
    void setEchoImage(QImage image);

    // QDeclarativeItem -> QQuickItem as per: http://qt-project.org/doc/qt-5.0/qtquick/qtquick-porting-qt5.html#c-code
    //void capture(QQuickItem *item);

signals:
    //void capture(QString const &path);
    void imageUpdated();
    void grab();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *);

private:
    QImage EchoImage;
};

#endif // SCREENSHOT_H
