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
    void SetScaling(ofVec3f scale);
    ofVec3f GetPosition();
    void Rotate(ofVec3f rot, float amount);
    void SetRotation(ofQuaternion quat);

    void SetKinematic(bool value);

    void SetSpace(dSpaceID sp);
    dSpaceID GetSpace(){return space;}

    dxGeom* GetGeom();

    void SetTorque(float x, float y, float z);
    void SetForce(float x, float y, float z);

    //TODO MAKE PRIVATE
    //Movement
    dReal speed = 0.f;
    dReal steer = 0.f;

    void EnableLights(bool value);

protected:
    void SetupBody(dWorldID worldID, dSpaceID spaceID);

    //Headlights
    ofLight* headlights[1];
    bool lightsEnabled= false;

    //ODE Physics stuff
    Cube* chasis;
    dJointGroupID group;

    //Wheels
    Cylinder* wheels[4];
    dJointID wheelJoint[4];
private:
    float minSpeed = 1.f;
    float maxSpeed = 8.f;
};

#endif // VEHICLE_H
