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
    QTime Timer;
    QOpenGLShaderProgram Program;
    int VertexAttr;
    int Resolution;
    int Time;
};

#endif // ECHO_TECHNIQUE_H
