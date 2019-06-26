#include "block.h"

block::block(QVector3D pos, short id)
{
    color = QVector4D(1,1,1,1);
    this->pos = pos;
    this->id = id;
}
