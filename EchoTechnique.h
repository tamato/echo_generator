#ifndef ECHO_TECHNIQUE_H
#define ECHO_TECHNIQUE_H

#include <QtGui/qvector3d.h>
#include <QtGui/qmatrix4x4.h>
#include <QtGui/qopenglshaderprogram.h>

#include <QTime>
#include <QVector>

class EchoTechnique
{
public:
    EchoTechnique();
    ~EchoTechnique();

    void render();
    void initialize();

private:

    QVector<QVector3D> Vertices;

    QOpenGLShaderProgram Program;
    int VertexAttr;
};

#endif // ECHO_TECHNIQUE_H
