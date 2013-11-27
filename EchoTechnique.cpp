#include "EchoTechnique.h"

#include <QRegExp>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QQmlProperty>
#include <QColor>

EchoTechnique::EchoTechnique(QQuickItem *parentItem)
    : ParentItem(parentItem)
{
}

EchoTechnique::~EchoTechnique()
{
}

void EchoTechnique::render()
{
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFrontFace(GL_CCW);
    glDisable(GL_DEPTH_TEST);

    Program.bind();

    QMap<QString, int>::const_iterator itr = Uniforms.constBegin();
    for (;itr != Uniforms.constEnd(); ++itr) {
        QQmlProperty prop(ParentItem, itr.key());
        QVariant var = prop.read();
        QVariant::Type type = var.type();
        switch(type)
        {
        case QMetaType::QSizeF:
            Program.setUniformValue(itr.value(), var.toSize());
            break;
        case QMetaType::Double:
            Program.setUniformValue(itr.value(), var.toFloat());
            break;
        case QMetaType::QColor:
            Program.setUniformValue(itr.value(), var.value<QColor>());
            break;
//        case QMetaType::QRect:
//            Program.setUniformValue(itr.value(), var.to
//            break;
//        case QMetaType::QRectF:
//            Program.setUniformValue(itr.value(), var.toRectF());
//            break;
        default:
            qWarning() << "Unrecongized property type < " << type << " > attempted to passed to shader";
        }
    }

    Program.enableAttributeArray(VertexAttr);
    Program.setAttributeArray(VertexAttr, Vertices.constData());
    glDrawArrays(GL_TRIANGLE_FAN, 0, Vertices.size());
    Program.disableAttributeArray(VertexAttr);
    Program.release();
    glEnable(GL_DEPTH_TEST);
}

void EchoTechnique::initialize()
{
    Timer.start();
    QOpenGLShader *vshader1 = new QOpenGLShader(QOpenGLShader::Vertex, &Program);
    const char *vsrc1 =
        "attribute highp vec4 vertex;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = vertex;\n"
        "}\n";
    vshader1->compileSourceCode(vsrc1);

    QOpenGLShader *fshader1 = new QOpenGLShader(QOpenGLShader::Fragment, &Program);
    fshader1->compileSourceFile( "echo.frag" );

    Program.addShader(vshader1);
    Program.addShader(fshader1);
    Program.link();

    // gatheter all uniforms
    QRegExp regexp;
    regexp.setPatternSyntax(QRegExp::RegExp2);
    regexp.setPattern("^ *uniform [a-zA-Z0-9 ]+ ([a-zA-Z0-9_]*);");
    QFile file( "echo.frag" );
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    while (!in.atEnd()){
        QString line = in.readLine();
        if (regexp.indexIn(line, 0) != -1){
            QString uniform_name = regexp.cap(1);
            Uniforms[uniform_name] = Program.uniformLocation(uniform_name);
        }
    }

    VertexAttr = Program.attributeLocation("vertex");

    Vertices.clear();
    Vertices << QVector3D(-1,-1,0);
    Vertices << QVector3D( 1,-1,0);
    Vertices << QVector3D( 1, 1,0);
    Vertices << QVector3D(-1, 1,0);
}
