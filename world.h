#ifndef WORLD_H
#define WORLD_H

#include <map>
#include <QVector2D>
#include <vector>
#include "chunk.h"

class world
{
public:
    world();
    std::vector<chunk> chunks;

    block * blockAt(QVector3D pos);
    chunk * chunkAt(QVector2D pos);

    chunk* addChunk(int x, int y);
    block* addBlock(QVector3D pos, int id);
};

#endif // WORLD_H
