#ifndef CYLINDER_H
#define CYLINDER_H

#include "gameobject.h"

class Cylinder : public GameObject
{
public:
    Cylinder(dWorldID w, dSpaceID s, float radius, float length);
    virtual ~Cylinder();

    virtual void Update(float deltaTime);
    virtual void Draw();
    void SetColour(ofColor colour);
private:
    ofColor colour = ofColor::mediumPurple;
    void SetupPhysics();
    float radius;
    float length;

};

#endif // CYLINDER_H
