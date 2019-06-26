#ifndef BLOCK_H
#define BLOCK_H

#include <QVector3D>
#include <QVector4D>

class block
{
public:
    block(QVector3D pos, short id);
    QVector3D pos;
    QVector4D color;
    short id;
    bool draw;
};

#endif // BLOCK_H
