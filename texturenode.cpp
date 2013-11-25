#include "texturenode.h"
#include <QDebug>

TextureNode::TextureNode(QQuickWindow *window)
    : m_fbo(0)
    , m_texture(0)
    , m_window(window)
    , echo(0)
{
    // Connect the beforeRendering signal to our paint function.
    // Since this call is executed on the rendering thread it must be
    // a Qt::DirectConnection
    connect(m_window, SIGNAL(beforeRendering()), this, SLOT(renderFBO()), Qt::DirectConnection);
}

TextureNode::~TextureNode()
{
    delete m_texture;
    delete m_fbo;
    delete echo;
}

void TextureNode::renderFBO()
{
    QSize size = rect().size().toSize();

    if (!m_fbo) {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        m_fbo = new QOpenGLFramebufferObject(size, format);

        m_texture = m_window->createTextureFromId(m_fbo->texture(), size);
        echo = new EchoTechnique();
        echo->initialize();
        setTexture(m_texture);
    }

    m_fbo->bind();
    glViewport(0, 0, size.width(), size.height());

    echo->render();

    m_fbo->bindDefault();
    m_window->update();
}
