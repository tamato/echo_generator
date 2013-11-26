#ifndef TEXTURENODE_H
#define TEXTURENODE_H

#include <QtQuick/QQuickWindow>
#include <QObject>
#include <qsgsimpletexturenode.h>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLShaderProgram>

#include "EchoTechnique.h"

class TextureNode : public QObject, public QSGSimpleTextureNode
{
    Q_OBJECT

public:
    TextureNode(QQuickWindow *window, QQuickItem *parent);
    ~TextureNode();

public slots:
    void renderFBO();

private:
    QOpenGLFramebufferObject *m_fbo;
    QSGTexture *m_texture;
    QQuickWindow *m_window;
    EchoTechnique *echo;
    QQuickItem *Parent;
};

#endif // TEXTURENODE_H
