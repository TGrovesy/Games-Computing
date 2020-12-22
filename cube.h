#ifndef CUBE_H
#define CUBE_H

#include "gameobject.h"

class Cube : public GameObject
{
public:
    Cube(float x, float y, float z);
    virtual ~Cube();

    virtual void Update(float deltaTime);
    virtual void Draw();

private:
    ofColor colour = ofColor::mediumPurple;
};

#endif // CUBE_H
