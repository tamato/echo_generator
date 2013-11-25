#include "EchoTechnique.h"

#include <QDebug>

EchoTechnique::EchoTechnique()
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

    Program.setUniformValue(Resolution, QVector2D(512,512));
    Program.setUniformValue(Time, Timer.elapsed()*0.001f);

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
    const char *fsrc1 =
        "uniform lowp vec2 resolution;\n"
        "uniform lowp float time;\n"
        "void main(void)\n"
        "{\n"
        "    vec2 uv = gl_FragCoord.xy/resolution.xy;\n"
        "    gl_FragColor = vec4(uv, sin(time)*.5+.5,1);\n"
        "}\n";
    fshader1->compileSourceCode(fsrc1);

    Program.addShader(vshader1);
    Program.addShader(fshader1);
    Program.link();

    VertexAttr = Program.attributeLocation("vertex");
    Resolution = Program.uniformLocation("resolution");
    Time = Program.uniformLocation("time");

    Vertices.clear();
    Vertices << QVector3D(-1,-1,0);
    Vertices << QVector3D( 1,-1,0);
    Vertices << QVector3D( 1, 1,0);
    Vertices << QVector3D(-1, 1,0);
}


/*
 *    ScreenCapture {
        id: echo_rect
        width : parent.echo_size
        height: parent.echo_size
        anchors.right: controls_rect.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        property real elapsed_time: 0

        Timer {
            interval: 10
            running: true
            repeat: true
            onTriggered: parent.elapsed_time += interval * 0.001
        }

        ShaderEffect {
            id: echo
            anchors.fill: parent
            property variant resolution: Qt.size(width,height)
            property real time: parent.elapsed_time

            property variant center_offset: Qt.size(0.0,0.0)
            property real darkness: 0.2
            property real animation_mod: 1.0
            property real animation_from_center_mod: 1.0/8.0
            property real contrast_str: 5.0
            property real object_density: 24.0
            fragmentShader:
                "
                    uniform float time;
                    uniform vec2 resolution;
                    uniform vec2 center_offset;

                    uniform float darkness;		            // was 0.2
                    uniform float animation_mod;	            // 1.
                    uniform float animation_from_center_mod;	// 1./8.
                    uniform float contrast_str;	            // 5.
                    uniform float object_density;           	// 24.

                    #define PI 3.14159265359

                    //     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
                    //               Distributed under the MIT License. See LICENSE file.
                    //               https://github.com/ashima/webgl-noise
                    //

                    vec3 mod289(vec3 x) {
                      return x - floor(x * (2.0 / 289.0)) * 289.0;
                    }

                    vec4 mod289(vec4 x) {
                      return x - floor(x * (1.0 / 289.0)) * 289.0;
                    }

                    vec4 permute(vec4 x) {
                         return mod289(((x*34.0)+1.0)*x);
                    }

                    vec4 taylorInvSqrt(vec4 r)
                    {
                      return 1.79284291400159 - 0.85373472095314 * r;
                    }

                    float snoise(vec3 v)
                    {
                      const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
                      const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

                    // First corner
                      vec3 i  = floor(v + dot(v, C.yyy) );
                      vec3 x0 =   v - i + dot(i, C.xxx) ;

                    // Other corners
                      vec3 g = step(x0.yzx, x0.xyz);
                      vec3 l = 1.0 - g;
                      vec3 i1 = min( g.xyz, l.zxy );
                      vec3 i2 = max( g.xyz, l.zxy );

                      //   x0 = x0 - 0.0 + 0.0 * C.xxx;
                      //   x1 = x0 - i1  + 1.0 * C.xxx;
                      //   x2 = x0 - i2  + 2.0 * C.xxx;
                      //   x3 = x0 - 1.0 + 3.0 * C.xxx;
                      vec3 x1 = x0 - i1 + C.xxx;
                      vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
                      vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

                    // Permutations
                      i = mod289(i);
                      vec4 p = permute( permute( permute(
                                 i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
                               + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
                               + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

                    // Gradients: 7x7 points over a square, mapped onto an octahedron.
                    // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
                      float n_ = 0.142857142857; // 1.0/7.0
                      vec3  ns = n_ * D.wyz - D.xzx;

                      vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

                      vec4 x_ = floor(j * ns.z);
                      vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

                      vec4 x = x_ *ns.x + ns.yyyy;
                      vec4 y = y_ *ns.x + ns.yyyy;
                      vec4 h = 1.0 - abs(x) - abs(y);

                      vec4 b0 = vec4( x.xy, y.xy );
                      vec4 b1 = vec4( x.zw, y.zw );

                      //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
                      //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
                      vec4 s0 = floor(b0)*2.0 + 1.0;
                      vec4 s1 = floor(b1)*2.0 + 1.0;
                      vec4 sh = -step(h, vec4(0.0));

                      vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
                      vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

                      vec3 p0 = vec3(a0.xy,h.x);
                      vec3 p1 = vec3(a0.zw,h.y);
                      vec3 p2 = vec3(a1.xy,h.z);
                      vec3 p3 = vec3(a1.zw,h.w);

                    //Normalise gradients
                      vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
                      p0 *= norm.x;
                      p1 *= norm.y;
                      p2 *= norm.z;
                      p3 *= norm.w;

                    // Mix final noise value
                      vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
                      m = m * m;
                      return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),
                                                    dot(p2,x2), dot(p3,x3) ) );
                    }

                    void main( void ) {
                        float animation_rate = time*animation_mod;
                        float animation_from_center = time * animation_from_center_mod;

                        vec2 p = ( gl_FragCoord.xy / resolution.xy ) - vec2(0.5);
                        p.x *= resolution.x/resolution.y;
                        p -= center_offset;

                        float p_len = length(p);

                        float color = 4.0 - (4.*p_len);
                        float color1 = color; // right side of the screen
                        float color2 = color; // left side of the screen
                        for(int i = 1; i <= 5; i++)
                        {
                            float power = pow(2.0,float(i));
                            float vec_y = (object_density * p_len - animation_from_center) * power;

                            float sn0 = snoise( cos(power)+vec3( (atan(p.y,-p.x))*power, vec_y, animation_rate ));
                            float sn1 = snoise( sin(power)+vec3( (atan(p.y, p.x))*power, vec_y, animation_rate ));

                            float contrast = contrast_str / power;
                            color1 -= ( contrast * sn0 );
                            color2 -= ( contrast * sn1 );
                        }

                        float curr_val = abs(atan(p.y, p.x));
                        color1 *= smoothstep(0.,PI,curr_val);
                        color2 *= smoothstep(PI,0.,curr_val);
                        // what if i try length instead of atan?

                        float final_color = color1+color2;
                        float black_dot = smoothstep(0.03, 0.08, length(p));
                        final_color *= black_dot;

                        gl_FragColor = vec4( vec3(final_color)*darkness, 1.0);
                    }
                "
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("saving img")
                    //screen.capture("test_img4.png")
                }
            }
        }
*/
