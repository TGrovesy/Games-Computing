#ifndef VEHICLE_H
#define VEHICLE_H

#include "gameobject.h"
#include "cylinder.h"
#include "cube.h"

class Vehicle : public GameObject
{
public:
    Vehicle(dWorldID worldID, dSpaceID spaceID, dJointGroupID jointGroup);
    ~Vehicle();
    virtual void Update(float deltaTime);
    virtual void Draw();

    void SetPosition(ofVec3f pos);

    void Rotate(ofVec3f rot, float amount);

    void SetKinematic(bool value);

    void SetSpace(dSpaceID sp);

    //TODO MAKE PRIVATE
    //Movement
    dReal speed = 3.f;
    dReal steer = 0.f;

protected:
    void SetupBody(dWorldID worldID, dSpaceID spaceID);


    //ODE Physics stuff
    Cube* chasis;
    dJointGroupID group;

    //Wheels
    Cylinder* wheels[4];
    dJointID wheelJoint[4];

};

#endif // VEHICLE_H
