#include "screenshot.h"
#include "texturenode.h"

#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickPaintedItem>

ScreenShot::ScreenShot()
{
    setFlag(ItemHasContents, true);
}

QImage ScreenShot::ehcoImage()
{
    return EchoImage;
}

void ScreenShot::setEchoImage(QImage image)
{
    if (EchoImage == image) return;
    EchoImage = image;
    emit imageUpdated();
    if (window())
        window()->update();
}

QSGNode *ScreenShot::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    // Don't bother with resize and such, just recreate the node from scratch
    // when geometry changes.
    if (oldNode)
        delete oldNode;
    TextureNode *node = new TextureNode(window());
    node->setRect(boundingRect());
    return node;
}
