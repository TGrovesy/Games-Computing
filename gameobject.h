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
    void SetPositon(glm::vec3 pos);
    void SetScaling(glm::vec3 scale);
    void SetRotation(glm::vec3 rotation, float angleDegrees);
    glm::vec3 GetPosition() const { return position; }
    glm::vec3 GetScale() const { return scale; }
    glm::vec3 GetRotationAxis() const{return rotationAxis;}
    float GetRotationAngle() const{return rotationAmount;}

    //Identifiers
    void SetName(char* name);
    char* GetName() const{ return name; }

    //ODE
    void SetWorldID(dWorldID id);

protected:
    //Transforms
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotationAxis;
    float rotationAmount;

    //ODE
    dWorldID world;
    dBodyID body;
    dMass mass;
    dGeomID geom;

    //Identifiers
    char* name;


};

#endif // GAMEOBJECT_H
