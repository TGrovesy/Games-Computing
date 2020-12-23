#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <glm/glm.hpp>
#include<ode/ode.h>
#include<ofMain.h>


class GameObject
{
public:
    GameObject();
    virtual ~GameObject();

    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;

    //Transform Stuff
    void SetPositon(ofVec3f pos);
    void SetScaling(ofVec3f scale);
    void SetRotation(ofVec3f rotation, float angleDegrees);
    ofVec3f GetPosition() const { return position; }
    ofVec3f GetScale() const { return scale; }
    ofVec3f GetRotationAxis() const{return rotationAxis;}
    float GetRotationAngle() const{return rotationAmount;}

    //Identifiers
    void SetName(char* name);
    char* GetName() const{ return name; }

    //ODE
    void SetWorldID(dWorldID id, dSpaceID collisionSpace);
    dGeomID GetGeom() {return geom;}
    void SetKinematic(bool value);


protected:
    //Transforms
    ofVec3f position;
    ofVec3f scale;
    ofVec3f rotationAxis;
    float rotationAmount;

    //ODE
    dWorldID world;
    dBodyID body;
    dMass mass;
    dGeomID geom;

    bool isKinematic = false;

    //Identifiers
    char* name;


};

#endif // GAMEOBJECT_H
