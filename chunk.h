#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"
#include <vector>
#include <QVector2D>
#include <QDebug>

class chunk
{
public:
    QVector2D pos;
    std::vector<block> blocks;
    chunk(QVector2D pos);

    block *blockAt(QVector3D pos);

    void update();
};

#endif // CHUNK_H
