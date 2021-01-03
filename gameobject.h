#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <glm/glm.hpp>
#include<ode/ode.h>
#include<ofMain.h>
#include <string>


class GameObject
{
public:
    GameObject(dWorldID w, dSpaceID s);
    virtual ~GameObject();

    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;

    //Transform Stuff
    void SetPosition(ofVec3f pos);
    ofVec3f GetPosition(){return position;}
    void SetScaling(ofVec3f scale);
    void SetRotation(ofQuaternion rotation);
    void Rotate(ofVec3f rotationAxis, float amount);
    ofVec3f GetScale()  { return scale; }
    ofQuaternion GetRotation() {return rotation;}
    ofVec3f GetRotationAngle()  {return rotationAngle;}
    float GetRotationAmount() {return rotationAmount;}
    void SetRotationAmount(float value){rotationAmount = value;}

    //Identifiers
    void SetName(char* name);
    std::string GetName() { return name; }

    //ODE
    void SetupPhysics(dWorldID id, dSpaceID collisionSpace);
    dxGeom* GetGeom() {return geom;}
    dBodyID GetBody(){return body;}
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


    bool isKinematic = false;

    //Identifiers
    std::string name;


};

#endif // GAMEOBJECT_H
