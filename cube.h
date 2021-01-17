#ifndef CUBE_H
#define CUBE_H

#include "gameobject.h"

class Cube : public GameObject
{
public:
    Cube(float x, float y, float z);
    Cube(dWorldID w, dSpaceID s, float x, float y, float z);
    ~Cube();

    virtual void Update(float deltaTime);
    virtual void Draw();

    virtual void SetColour(ofColor colour);

protected:
    ofColor colour = ofColor::mediumPurple;
};

#endif // CUBE_H
