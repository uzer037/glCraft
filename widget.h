#ifndef WIDGET_H
#define WIDGET_H

#include <QGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QObject>
#include <QMatrix4x4>
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>
#include <QCursor>
#include <cmath>

struct vertData
{
    vertData(){}
    vertData(QVector3D p, QVector2D t, QVector3D n) : position(p), textCoord(t), normal(n)
    {
    }

    QVector3D position;
    QVector2D textCoord;
    QVector3D normal;
};

struct collisionBox
{
    QVector3D apos; // front top left
    QVector3D bpos; // back bot right
    //box from a.pos --> b.pos
};

class block
{
    public:

    collisionBox cbox;
    int id;
    double size = 1.0;
    QVector3D pos;
    QVector3D rot;



    block(QVector3D pos, int id)
    {
        this->pos = pos;
        this->id = id;

    }

    void calcCbox()
    {
        float sz2 = size/2;
        cbox.apos = QVector3D(pos.x() - sz2, pos.y() - sz2, pos.z() - sz2);
        cbox.bpos = QVector3D(pos.x() + sz2, pos.y() + sz2, pos.z() + sz2);
    }

    int getId()
    {
        return id;
    }

    collisionBox getCbox()
    {
        return cbox;
    }

    QVector3D getPos()
    {
        return pos;
    }

    void setPos(QVector3D pos)
    {
        this->pos = pos;
    }
};

// // // // // // // // //
//       MAIN CLASS     //
// // // // // // // // //

class Widget: public QGLWidget
{
    Q_OBJECT

public:

    Widget(QWidget *parent = nullptr);




    QTimer* mainTick;
    QTimer* fpsTick;
    int frames = 0;
    bool done = 1;
    float angle = 0;
    bool fps = 0;
    float aspectRatio = 1;

    // // // // // // // //
    //       camera      //
    // // // // // // // //

    class CameraClass
    {
        public:

        CameraClass(QVector3D pos, QVector3D rot)
        {
            this->pos = pos;
            this->rot = rot;
            speed = {0.1,0.1,0.1};
            angSpeed = {100,100,100};
        }

        void xShift(double x)
        {
            pos.setX(pos.x() + x);
        }
        void yShift(double y)
        {
            pos.setY(pos.y() + y);
        }
        void zShift(double z)
        {
            pos.setZ(pos.z() + z);
        }

        void xRotShift(double x)
        {
            if(rot.x() + x >= 360) x -= 360;
            if(rot.x() + x < 0) x += 360;
            rot.setX(rot.x() + x);
        }
        void yRotShift(double y)
        {
            if(rot.y() + y >= 360) y -= 360;
            if(rot.y() + y < 0) y += 360;
            rot.setY(rot.y() + y);
        }
        void zRotShift(double z)
        {
            if(rot.z() + z >= 360) z -= 360;
            if(rot.z() + z < 0) z += 360;
            rot.setZ(rot.z() + z);
        }

        QVector3D pos;
        QVector3D rot;
        QVector3D speed;
        QVector3D angSpeed;
    };
    CameraClass* cam;

    void camUpdate();

    // // // // // // // //
    //       input       //
    // // // // // // // //

    QCursor cur;

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

    QVector <bool> keyArr;
    QVector <bool> spKeyArr;

    QVector2D mousePos;
    bool mouseYInverse = 1;

    bool mouseStick = 0;
    void mouseCorrect();

    // // // // // // // // // // //
    //       OpenGL Main Func     //
    // // // // // // // // // // //
public slots:
    void paintGL();
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void printFPS();

    // // // // // // // // // // // // //
    //      OpenGL Structures Init      //
    // // // // // // // // // // // // //

private:

    QMatrix4x4 projection;
    QMatrix4x4 viewMatrix;

    QOpenGLShaderProgram prog;
    QVector <QOpenGLTexture *> texture;
    QImage *textureSheet;
    QOpenGLBuffer arrBuff;
    QOpenGLBuffer indexBuff;

    void loadTextures();

    // // // // // // // // // //
    //       SHADERS INIT      //
    // // // // // // // // // //

    void initShaders();
    void initCube(float w);
    void initView(QVector3D pos,QVector3D rot);

    // // // // // // // // // // //
    //          OBJ INIT          //
    // // // // // // // // // // //

    void initScene();
    QVector <block *> blocks;

    // // // // // // // // //
    //       OBJ DRAW       //
    // // // // // // // // //

    void drawCube(block *b);


public slots:
    void mainLoop();
public:

};

// // // // // // // // // // // //
//        CUSTOM FUNCTIONS       //
//                               //
//        NON  MAIN CLASS        //
// // // // // // // // // // // //

 bool collideCheck(collisionBox a, collisionBox b);

#endif // WIDGET_H
