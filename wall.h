#ifndef WALL_H
#define WALL_H

#include "cube.h"

class Wall : public Cube
{
public:
    Wall(dWorldID w, dSpaceID s, float x, float y, float z);
    virtual ~Wall();

    virtual void Update(float deltaTime);
    virtual void Draw();


    void SetSpeed(float value){speed = value;}

protected:
    float speed = 0.15f;
};

#endif // WALL_H
