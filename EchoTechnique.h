#ifndef ECHO_TECHNIQUE_H
#define ECHO_TECHNIQUE_H

#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qopenglshaderprogram.h>
#include <QtQuick/QQuickItem>
#include <QTime>
#include <QVector>
#include <QMap>

class EchoTechnique
{
public:
    EchoTechnique(QQuickItem *parentItem);
    ~EchoTechnique();

    void render();
    void initialize();

private:

    QVector<QVector3D> Vertices;
    QTime Timer;
    QOpenGLShaderProgram Program;
    int VertexAttr;

    QMap<QString, int> Uniforms;
    QQuickItem *ParentItem;
};

#endif // ECHO_TECHNIQUE_H
