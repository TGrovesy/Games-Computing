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

    //void SetColour(ofColor colour);

protected:
    ofColor colour = ofColor::mediumPurple;
};

#endif // WALL_H
