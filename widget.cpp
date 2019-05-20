 #include "ui_widget.h"
#include "widget.h"

Widget::Widget(QWidget *parent) // конструктор
    : QGLWidget(parent), indexBuff(QOpenGLBuffer::IndexBuffer)
{
    int xsz = 1000, ysz = 800;
    this->resize(xsz,ysz);// задаем размеры окна
    resizeGL(xsz,ysz);
    glViewport(0,0,xsz,ysz);
    for(int i = 0; i < 300; i++)
    {
        keyArr.push_back(0);
    }
    for(int i = 0; i < 50; i++)
    {
        spKeyArr.push_back(0);
    }
}

void Widget::initializeGL()
{
    glClearColor(0.3f, 0.7f, 1.0f, 1.0f); // Set background color to black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glEnable(GL_CULL_FACE);

    glViewport(0,0,this->size().width(),this->size().height());

    initShaders();
    initCube(1.0f);

    initScene();

    cam = new CameraClass(QVector3D(0,0,-10),QVector3D(0,0,0));
    initView(cam->pos,cam->rot);

    paintGL(); // filling 'n swaping 1st buffer
    paintGL(); // filling 'n swaping 2nd buffer
    fpsTick = new QTimer();
    fpsTick->setInterval(1000);
    connect(this->fpsTick, SIGNAL(timeout()), this, SLOT(printFPS()) );
    fpsTick->start();

    mainTick = new QTimer();
    mainTick->setInterval(10);
    connect(this->mainTick, SIGNAL(timeout()), this, SLOT(mainLoop()) );
    mainTick->start();
}

void Widget::resizeGL(int nWidth, int nHeight)
{
    aspectRatio = nWidth / (float)nHeight;
    glViewport(0,0,nWidth,nHeight);
    projection.setToIdentity();
//    projection.frustum(0,nWidth,0,nHeight,0,1);
    projection.perspective(45,aspectRatio,0.1f,100.0f);
    //paintGL();
}

void Widget::paintGL() // рисование
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очистка экрана
//    drawCube(QVector3D(0,0,0));
    int n = 15;
    for(auto i : blocks)
    {
        drawCube(i);
    }
    glFinish();
    swapBuffers();
}

void Widget::camUpdate()
{
    initView(cam->pos,cam->rot);
    paintGL();
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    mouseStick = 1;
    QWidget::setCursor( Qt::BlankCursor);
    mousePos.setX(e->pos().x());
    mousePos.setY(e->pos().y());
    mouseCorrect();
}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    mouseStick = 0;
    QWidget::setCursor( Qt::CursorShape::ArrowCursor);
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    QVector2D delta = QVector2D(e->pos().x() - mousePos.x(), e->pos().y() - mousePos.y());
    double speed = delta.length();
    cam->yRotShift(delta.x()/(float)this->size().height()*cam->angSpeed.x());
    cam->xRotShift(-delta.y()/(float)this->size().width()*cam->angSpeed.y() * (1 - 2 * mouseYInverse));
    qDebug() << delta;
    //qDebug() << cur.pos();


    if(abs(delta.x()) > 0 || abs(delta.y()) > 0)
    {
        mouseCorrect();
        mousePos.setX(cur.pos().x() - this->pos().x());
        mousePos.setY(cur.pos().y() - this->pos().y()-36);

    }
}

void Widget::mouseCorrect()
{
    if(mouseStick)
    {
        if(cur.pos().x() - this->pos().x() > this->size().width()) cur.setPos(this->pos().x(),cur.pos().y());

        if(cur.pos().x() - this->pos().x() < 0) cur.setPos(this->pos().x() + this->size().width(),cur.pos().y());

        if(cur.pos().y() - this->pos().y() > this->size().height()) cur.setPos(cur.pos().x(),this->pos().y());

        if(cur.pos().y() - this->pos().y() < 0) cur.setPos(cur.pos().x(),this->pos().y() + this->size().height());
    }
}

void Widget::keyPressEvent(QKeyEvent *e)
{
    if(e->key() < 300 && e->key() > 0)
        keyArr[e->key()] = 1;
    else {
        switch(e->key())
        {
            case Qt::Key::Key_Control :
            {
                spKeyArr[0] = 1;
                break;
            }

            case Qt::Key::Key_Alt :
            {
                spKeyArr[1] = 1;
                break;
            }

            case Qt::Key::Key_AltGr :
            {
                spKeyArr[2] = 1;
                break;
            }

            case Qt::Key::Key_Shift :
            {
                spKeyArr[3] = 1;
                break;
            }
        }
    }
}
void Widget::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() < 300 && e->key() > 0)
        keyArr[e->key()] = 0;
    else
    {
        switch(e->key())
        {
            case Qt::Key::Key_Control :
            {
                spKeyArr[0] = 0;
                break;
            }

            case Qt::Key::Key_Alt :
            {
                spKeyArr[1] = 0;
                break;
            }

            case Qt::Key::Key_AltGr :
            {
                spKeyArr[2] = 0;
                break;
            }

            case Qt::Key::Key_Shift :
            {
                spKeyArr[3] = 0;
                break;
            }
        }
    }
}

void Widget::printFPS()
{
    if(fps)
    {
        qDebug() << "FPS: " << frames;
    }
    frames = 0;
}

void Widget::loadTextures()
{
    int w = textureSheet->size().width()/16;
    int h = textureSheet->size().height()/16;
    QImage textureTmp;
    for(int i = 0; i < 16; i++) // x
    {
        for(int j = 0; j < 16; j++) // y
        {
            textureTmp = textureSheet->copy(i*16,j*16,16,16);

            texture.push_back(new QOpenGLTexture(textureTmp.mirrored()));

            texture[i*16+j]->setMinificationFilter(QOpenGLTexture::Nearest);
            texture[i*16+j]->setMagnificationFilter(QOpenGLTexture::Nearest);
            texture[i*16+j]->setWrapMode(QOpenGLTexture::Repeat);
        }
    }
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

    textureSheet = new QImage(":/texture.png");
    loadTextures();
}

void Widget::initView(QVector3D pos, QVector3D rot)
{
    viewMatrix.setToIdentity();
//    viewMatrix.translate(pos);
    viewMatrix.rotate(rot.x(), 1, 0, 0);
    viewMatrix.rotate(rot.y(), 0, 1, 0);
    viewMatrix.rotate(rot.z(), 0, 0, 1);
    QVector3D newPos(pos.x(), -pos.y(), pos.z());
    viewMatrix.translate(newPos);

}

void Widget::initScene()
{
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            blocks.push_back(new block(QVector3D(i,-2,j), i*16+j) );
        }
    }
}

void Widget::drawCube(block* b)
{

    QMatrix4x4 matrixModel;
    matrixModel.setToIdentity();
    matrixModel.translate(b->pos);

    texture[b->id]->bind(0);

    prog.bind();
    prog.setUniformValue("u_ProjectionMatrix",  projection);
    prog.setUniformValue("u_ViewMatrix",  viewMatrix);
    prog.setUniformValue("u_ModelMatrix",  matrixModel);
    prog.setUniformValue("v_texCoord0", 0);
    prog.setUniformValue("u_lightSourcePos", QVector4D(0.0,0.0,0.0,1.0));
    prog.setUniformValue("u_lightPower", 5);

    arrBuff.bind();

    int offset = 0;

    int vertLoc = prog.attributeLocation("a_position");
    prog.enableAttributeArray(vertLoc);
    prog.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(vertData));

    offset += sizeof(QVector3D);

    int texLoc = prog.attributeLocation("a_texCoord0");
    prog.enableAttributeArray(texLoc);
    prog.setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(vertData));

    offset += sizeof(QVector2D);

    int normLoc = prog.attributeLocation("a_normal");
    prog.enableAttributeArray(normLoc);
    prog.setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(vertData));

    indexBuff.bind();

    glDrawElements(GL_TRIANGLES, indexBuff.size(), GL_UNSIGNED_INT, 0);
}

void Widget::mainLoop()
{
    // //
    // special keys array (spKeyArr):
    // 0 = ctrl
    // 1 = alt
    // 2 = altGr
    // 3 = shift
    // 4 = enter
    // 5 = tab
    // 6 = backspace
    // //

    if(keyArr[Qt::Key::Key_A])
    {
        cam->pos.setX(cam->pos.x() + cam->speed.x() * cos(cam->rot.y()/180*M_PI));
        cam->pos.setZ(cam->pos.z() + cam->speed.z() * sin(cam->rot.y()/180*M_PI));
    }
    if(keyArr[Qt::Key::Key_D])
    {
        cam->pos.setX(cam->pos.x() - cam->speed.x() * cos(cam->rot.y()/180*M_PI));
        cam->pos.setZ(cam->pos.z() - cam->speed.z() * sin(cam->rot.y()/180*M_PI));
    }

    if(keyArr[Qt::Key::Key_W])
    {
        cam->pos.setX(cam->pos.x() - cam->speed.x() * sin(cam->rot.y()/180*M_PI));
        cam->pos.setZ(cam->pos.z() + cam->speed.z() * cos(cam->rot.y()/180*M_PI));
    }
    if(keyArr[Qt::Key::Key_S])
    {
        cam->pos.setX(cam->pos.x() + cam->speed.x() * sin(cam->rot.y()/180*M_PI));
        cam->pos.setZ(cam->pos.z() - cam->speed.z() * cos(cam->rot.y()/180*M_PI));
    }
    if(keyArr[Qt::Key::Key_Space])
    {
        cam->pos.setY(cam->pos.y()+cam->speed.y());
    }
    if(spKeyArr[3])
    {
        cam->pos.setY(cam->pos.y()-cam->speed.y());
    }

    camUpdate();

    //qDebug() << cam->rot;
    frames++; // for FPS counter
}

bool collideCheck(collisionBox a, collisionBox b)
{

}
