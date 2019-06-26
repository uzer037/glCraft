#include "chunk.h"

chunk::chunk(QVector2D pos)
{
    this->pos = pos;
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 256; j++)
        {
            for(int k = 0; k < 16; k++)
            {
                blocks.push_back(block(QVector3D(i,j,k),0));
            }
        }
    }
}

block *chunk::blockAt(QVector3D pos)
{
    return &blocks[pos.x()*16*256 + pos.y()*16 + pos.z()];
}

void chunk::update()
{
    for(auto i : blocks)
    {
        bool draw = 0;
        if(blockAt(QVector3D((int)i.pos.x()+1,(int)i.pos.y()+1,(int)i.pos.z()+1))->id == 0)
            draw = 1;
        if(blockAt(QVector3D((int)i.pos.x()+1,(int)i.pos.y()+1,(int)i.pos.z()-1))->id == 0)
            draw = 1;
        if(blockAt(QVector3D((int)i.pos.x()+1,(int)i.pos.y()-1,(int)i.pos.z()+1))->id == 0)
            draw = 1;
        if(blockAt(QVector3D((int)i.pos.x()+1,(int)i.pos.y()-1,(int)i.pos.z()-1))->id == 0)
            draw = 1;
        if(blockAt(QVector3D((int)i.pos.x()-1,(int)i.pos.y()+1,(int)i.pos.z()+1))->id == 0)
            draw = 1;
        if(blockAt(QVector3D((int)i.pos.x()-1,(int)i.pos.y()+1,(int)i.pos.z()-1))->id == 0)
            draw = 1;
        if(blockAt(QVector3D((int)i.pos.x()-1,(int)i.pos.y()-1,(int)i.pos.z()+1))->id == 0)
            draw = 1;
        if(blockAt(QVector3D((int)i.pos.x()-1,(int)i.pos.y()-1,(int)i.pos.z()-1))->id == 0)
            draw = 1;

        if(draw)
        {
            i.draw = 1;
        }
        else
            i.draw = 0;
    }
}
