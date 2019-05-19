#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QWidget>

class glView : public QGLWidget
{

    Q_OBJECT

public:
    glView(QWidget);
    void initializeGL();
    void resizeGL(int h, int w);
    void paintGL();
};

#endif // GLVIEW_H
