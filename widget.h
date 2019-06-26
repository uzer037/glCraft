#ifndef WIDGET_H
#define WIDGET_H

#ifndef OUTERCLASS
#define OUTERCLASS(className, memberName) \
    reinterpret_cast<className*>(reinterpret_cast<unsigned char*>(this) - offsetof(className, memberName))
#endif

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
#include <QDateTime>
#include <cmath>

#include "block.h"
#include "chunk.h"
#include "world.h"

static int randomBetween(int low, int high)
{
    return (qrand() % ((high + 1) - low) + low);
}

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

bool isOverlap (collisionBox a, collisionBox b);

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


//    bool checkOverlap(collisionBox obj);

    // // // // // // // //
    //       camera      //
    // // // // // // // //

    class CameraClass
    {
        public:
        Widget* widg;
        QVector3D pos;
        QVector3D rot;
        QVector3D speed;
        QVector3D angSpeed;
        collisionBox cbox;
        QVector3D cboxSz;

        CameraClass(Widget *widg, QVector3D pos, QVector3D rot)
        {

            cboxSz = QVector3D(1,1,1);
            cbox.apos = pos + cboxSz;
            cbox.bpos = pos - cboxSz;
            this->pos = pos;
            this->rot = rot;
            speed = {0.5,0.5,0.5};
            angSpeed = {100,100,100};
        }

        void moved()
        {
            cbox.apos = pos + cboxSz;
            cbox.bpos = pos - cboxSz;
        }

        void xShift(double x)
        {
//            if(widg->checkOverlap(cbox))
//            {
                pos.setX(pos.x() + x);
                moved();
//            }
        }
        void yShift(double y)
        {
//            if(widg->checkOverlap(cbox))
//            {
                    pos.setY(pos.y() + y);
                    moved();
//            }
        }
        void zShift(double z)
        {
//            if(widg->checkOverlap(cbox))
//            {
                pos.setZ(pos.z() + z);
                moved();
//            }
        }

        void xRotShift(double x)
        {
            if(rot.x() + x < -90) rot.setX(rot.x());
            else
            {
                if(rot.x() + x > 90) rot.setX(rot.x());
                else
                    rot.setX(rot.x() + x);
            }
        }
        void yRotShift(double y)
        {
            if(rot.y() + y >= 360) y -= 360;
            if(rot.y() + y < 0) y += 360;
            rot.setY(rot.y() + y);
        }
        void zRotShift(double z)
        {
            if(rot.z() + z >= 180) z -= 360;
            if(rot.z() + z < 0) z += 360;
            rot.setZ(rot.z() + z);
        }

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

    world w;

    void pillar(QVector3D pos, int h, int id);
    void genPillar(QVector3D pos, int h);
    void genChunk(QVector2D pos);

    void initScene();
    QVector <block *> blocks;

    // // // // // // // // //
    //       OBJ DRAW       //
    // // // // // // // // //

    void drawCube(block* bl);


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

 class player
 {

 };

#endif // WIDGET_H
