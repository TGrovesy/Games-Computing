#ifndef PLANE_H
#define PLANE_H

#include "gameobject.h"

class Plane : public GameObject
{
public:
    Plane(float x, float y, float z);
    void Draw();
    void Update(float deltaTime);
};

#endif // PLANE_H
