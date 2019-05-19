 #include "ui_widget.h"
#include "widget.h"

Widget::Widget(QWidget *parent) // конструктор
    : QGLWidget(parent), texture(nullptr), indexBuff(QOpenGLBuffer::IndexBuffer)
{
    int xsz = 300, ysz = 300;
    this->resize(xsz,ysz);// задаем размеры окна
    resizeGL(xsz,ysz);

}

void Widget::initializeGL()
{
    glClearColor(0.3f, 0.7f, 1.0f, 1.0f); // Set background color to black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glEnable(GL_CULL_FACE);


    initShaders();
    initCube(1.0f);


    cam = new CameraClass(QVector3D(0,0,0),QVector3D(0,0,0));
    initView(cam->pos,cam->rot);

    paintGL(); // filling 'n swaping 1st buffer
    paintGL(); // filling 'n swaping 2nd buffer
    fpsTick = new QTimer();
    fpsTick->setInterval(1000);
    connect(this->fpsTick, SIGNAL(timeout()), this, SLOT(printFPS()) );
    fpsTick->start();
}

void Widget::resizeGL(int nWidth, int nHeight)
{
    aspectRatio = nWidth / (float)nHeight;

    projection.setToIdentity();
    projection.perspective(45,aspectRatio,0.1f,10.0f);

}

void Widget::paintGL() // рисование
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очистка экрана
    drawCube(QVector3D(0,0,-5));
    drawCube(QVector3D(1,0,-5));
    swapBuffers();
}

void Widget::camUpdate()
{
    initView(cam->pos,cam->rot);
    paintGL();
}

void Widget::mousePressEvent(QMouseEvent *e)
{

}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    QVector2D delta = QVector2D(e->pos().x() - mousePos.x(),e->pos().y() - mousePos.y());
    cam->yRotShift(delta.x());
    cam->xRotShift(-delta.y());
    qDebug() << delta;
    mousePos.setX(e->pos().x());
    mousePos.setY(e->pos().y());
    camUpdate();
}

void Widget::keyPressEvent(QKeyEvent *e)
{
 if(e->key() == Qt::Key::Key_A)
 {
     cam->xShift(cam->speed.x());
 }
 if(e->key() == Qt::Key::Key_D)
 {
     cam->xShift(-cam->speed.x());
 }

 if(e->key() == Qt::Key::Key_W)
 {
     cam->zShift(cam->speed.z());
 }
 if(e->key() == Qt::Key::Key_S)
 {
     cam->zShift(-cam->speed.z());
 }
 while(angle < 0) angle += 360;
 while(angle >= 360) angle -= 360;
 qDebug() << e;
 qDebug() << angle;
 camUpdate();
}

void Widget::printFPS()
{
    if(fps)
    {
        qDebug() << "FPS: " << frames;
    }
    frames = 0;
}

void Widget::initShaders()
{
    if(!prog.addShaderFromSourceFile(QOpenGLShader::Vertex,":/vert.vsh"))
        close();
    if(!prog.addShaderFromSourceFile(QOpenGLShader::Fragment,":/frag.fsh"))
        close();
    if(!prog.link())
        close();
}

void Widget::initCube(float w)
{
    float wDiv2 = w/2.0f;
    QVector <vertData> vert;
    vert.append(vertData(QVector3D(-wDiv2,wDiv2,wDiv2), QVector2D(0.0,1.0), QVector3D(0.0, 0.0, 1.0))); //
    vert.append(vertData(QVector3D(-wDiv2,-wDiv2,wDiv2), QVector2D(0.0,0.0), QVector3D(0.0, 0.0, 1.0)));
    vert.append(vertData(QVector3D(wDiv2,wDiv2,wDiv2), QVector2D(1.0,1.0), QVector3D(0.0, 0.0, 1.0)));
    vert.append(vertData(QVector3D(wDiv2,-wDiv2,wDiv2), QVector2D(1.0,0.0), QVector3D(0.0, 0.0, 1.0)));

    vert.append(vertData(QVector3D(wDiv2,wDiv2,wDiv2), QVector2D(0.0,1.0), QVector3D(1.0, 0.0, 0.0))); //
    vert.append(vertData(QVector3D(wDiv2,-wDiv2,wDiv2), QVector2D(0.0,0.0), QVector3D(1.0, 0.0, 0.0)));
    vert.append(vertData(QVector3D(wDiv2,wDiv2,-wDiv2), QVector2D(1.0,1.0), QVector3D(1.0, 0.0, 0.0)));
    vert.append(vertData(QVector3D(wDiv2,-wDiv2,-wDiv2), QVector2D(1.0,0.0), QVector3D(1.0, 0.0, 0.0)));

    vert.append(vertData(QVector3D(wDiv2,wDiv2,wDiv2), QVector2D(0.0,1.0), QVector3D(0.0, 1.0, 0.0))); //
    vert.append(vertData(QVector3D(wDiv2,wDiv2,-wDiv2), QVector2D(0.0,0.0), QVector3D(0.0, 1.0, 0.0)));
    vert.append(vertData(QVector3D(-wDiv2,wDiv2,wDiv2), QVector2D(1.0,1.0), QVector3D(0.0, 1.0, 0.0)));
    vert.append(vertData(QVector3D(-wDiv2,wDiv2,-wDiv2), QVector2D(1.0,0.0), QVector3D(0.0, 1.0, 0.0)));

    vert.append(vertData(QVector3D(wDiv2,wDiv2,-wDiv2), QVector2D(0.0,1.0), QVector3D(0.0, 0.0, -1.0))); //
    vert.append(vertData(QVector3D(wDiv2,-wDiv2,-wDiv2), QVector2D(0.0,0.0), QVector3D(0.0, 0.0, -1.0)));
    vert.append(vertData(QVector3D(-wDiv2,wDiv2,-wDiv2), QVector2D(1.0,1.0), QVector3D(0.0, 0.0, -1.0)));
    vert.append(vertData(QVector3D(-wDiv2,-wDiv2,-wDiv2), QVector2D(1.0,0.0), QVector3D(0.0, 0.0, -1.0)));

    vert.append(vertData(QVector3D(-wDiv2,wDiv2,wDiv2), QVector2D(0.0,1.0), QVector3D(-1.0, 0.0, 0.0))); //
    vert.append(vertData(QVector3D(-wDiv2,wDiv2,-wDiv2), QVector2D(0.0,0.0), QVector3D(-1.0, 0.0, 0.0)));
    vert.append(vertData(QVector3D(-wDiv2,-wDiv2,wDiv2), QVector2D(1.0,1.0), QVector3D(-1.0, 0.0, 0.0)));
    vert.append(vertData(QVector3D(-wDiv2,-wDiv2,-wDiv2), QVector2D(1.0,0.0), QVector3D(-1.0, 0.0, 0.0)));

    vert.append(vertData(QVector3D(-wDiv2,-wDiv2,wDiv2), QVector2D(0.0,1.0), QVector3D(0.0, -1.0, 0.0))); //
    vert.append(vertData(QVector3D(-wDiv2,-wDiv2,-wDiv2), QVector2D(0.0,0.0), QVector3D(0.0, -1.0, 0.0)));
    vert.append(vertData(QVector3D(wDiv2,-wDiv2,wDiv2), QVector2D(1.0,1.0), QVector3D(0.0, -1.0, 0.0)));
    vert.append(vertData(QVector3D(wDiv2,-wDiv2,-wDiv2), QVector2D(1.0,0.0), QVector3D(0.0, -1.0, 0.0)));

    QVector <GLuint> index;
    for(int i = 0; i < 24; i += 4)
    {
        index.append(i+0);
        index.append(i+1);
        index.append(i+2);
        index.append(i+2);
        index.append(i+1);
        index.append(i+3);
    }

    arrBuff.create();
    arrBuff.bind();
    arrBuff.allocate(vert.constData(), vert.size() * sizeof(vertData));
    arrBuff.release();

    indexBuff.create();
    indexBuff.bind();
    indexBuff.allocate(index.constData(), index.size() * sizeof(GLuint));
    indexBuff.release();

    texture = new QOpenGLTexture(QImage(":/tex.jpg").mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Widget::initView(QVector3D pos, QVector3D rot)
{
    viewMatrix.setToIdentity();
    viewMatrix.rotate(rot.x(), 1, 0, 0);
    viewMatrix.rotate(rot.y(), 0, 1, 0);
    viewMatrix.rotate(rot.z(), 0, 0, 1);
    viewMatrix.translate(pos);
}

void Widget::drawCube(QVector3D pos)
{
    QMatrix4x4 matrixModel;
    matrixModel.setToIdentity();
    matrixModel.translate(pos);
    texture->bind(0);

    prog.bind();
    prog.setUniformValue("qt_ModelViewProjectionMatrix",  projection * viewMatrix*matrixModel);
    prog.setUniformValue("qt_Texture0", 0);

    arrBuff.bind();

    int offset = 0;

    int vertLoc = prog.attributeLocation("qt_Vertex");
    prog.enableAttributeArray(vertLoc);
    prog.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(vertData));

    offset += sizeof(QVector3D);

    int texLoc = prog.attributeLocation("qt_MultiTexCoord0");
    prog.enableAttributeArray(texLoc);
    prog.setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(vertData));

    indexBuff.bind();

    glDrawElements(GL_TRIANGLES, indexBuff.size(), GL_UNSIGNED_INT, 0);
}

void Widget::mainLoop()
{
    frames++; // for FPS counter
}
