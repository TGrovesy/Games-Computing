#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <glm/glm.hpp>
#include<ode/ode.h>
#include<ofMain.h>


class GameObject
{
public:
    GameObject(dWorldID w, dSpaceID s, dJointGroupID j);
    virtual ~GameObject();

    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;

    //Transform Stuff
    void SetPositon(ofVec3f pos);
    void SetScaling(ofVec3f scale);
    void SetRotation(ofQuaternion rotation, float angleDegrees);
    ofVec3f GetPosition() const { return position; }
    ofVec3f GetScale() const { return scale; }
    ofQuaternion GetRotationAxis() const{return rotation;}
    float GetRotationAngle() const{return rotationAmount;}

    //Identifiers
    void SetName(char* name);
    char* GetName() const{ return name; }

    //ODE
    void SetupPhysics(dWorldID id, dSpaceID collisionSpace);
    dxGeom* GetGeom() {return geom;}
    void SetKinematic(bool value);
    void Freeze();

    void SetMass(dReal newMass);


protected:
    //Transforms
    ofVec3f position;
    ofVec3f scale;
    ofQuaternion rotation;
    ofVec3f rotationAngle;
    float rotationAmount;

    //ODE
    dWorldID world;
    dSpaceID space;
    dBodyID body;
    dMass mass;
    dGeomID geom;

    //test
    dJointGroupID jointGroupID;

    bool isKinematic = false;

    //Identifiers
    char* name;


};

#endif // GAMEOBJECT_H
