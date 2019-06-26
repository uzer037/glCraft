#include "world.h"

world::world()
{

}

block *world::blockAt(QVector3D pos)
{
    chunk *ch = chunkAt(QVector2D((int)pos.x()/16, (int)pos.z()/16));
    block *bl = nullptr;
    if(ch != nullptr)
    {
        bl = &ch->blocks[pos.x()*16*256 + pos.y()*16 + pos.z()];
    }
    return bl;
}

chunk* world::chunkAt(QVector2D pos)
{
    chunk* ch = nullptr;
    if(this->chunks.size() > 0)
    {
        for(int i = 0; i < this->chunks.size(); i++)
        {
            if(this->chunks[i].pos == pos)
            {
                ch = &this->chunks[i];
                break;
            }
        }
    }
    return ch;
}

chunk* world::addChunk(int x, int y)
{
    chunks.push_back(chunk(QVector2D(x,y)));
    return &chunks[chunks.size()-1];
}

block* world::addBlock(QVector3D pos, int id)
{
    block *bl = nullptr;
    if(pos.y() >=0 && pos.y() < 256)
    {
        int cx = pos.x() / 16;
        int cy = pos.z() / 16;
        chunk * ch = chunkAt(QVector2D(cx,cy));
        if( ch == nullptr)
            ch = addChunk(cx,cy);

        bl = blockAt(pos);
        if(bl == nullptr)
        {

            int x = (int)pos.x() % 16;
            int y = pos.y();
            int z = (int)pos.z() % 16;

            ch->blocks.push_back(block(QVector3D(x,y,z),id));
            bl = &ch->blocks[ch->blocks.size()-1];
        }
        else
            bl->id = id;
    }
    return bl;
}

void world::rmBlockAt(QVector3D pos)
{
    block * bl = blockAt(pos);
    if(bl != nullptr)
        bl->id = 0;
}
