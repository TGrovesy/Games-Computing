#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <glm/glm.hpp>
#include<ode/ode.h>
#include<ofMain.h>
#include <string>


class GameObject
{
public:
    GameObject();
    GameObject(dWorldID w, dSpaceID s);
    virtual ~GameObject();

    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;

    //Rendering
    void SetTexture(std::string path);
    void SetTexture(ofTexture* tex);
    ofTexture* GetTexture(){return texture;}
    void SetTextureScale(ofVec2f value){textureScale = value;}

    void SetMaterial(ofMaterial* material);

    //Transform Stuff
    virtual void SetPosition(ofVec3f pos);
    virtual ofVec3f GetPosition(){return position;}
    virtual void SetScaling(ofVec3f scale);
    virtual void SetRotation(ofQuaternion rotation);
    void Rotate(ofVec3f rotationAxis, float amount);
    ofVec3f GetScale()  { return scale; }
    ofQuaternion GetRotation() {return rotation;}
    ofVec3f GetRotationAngle()  {return rotationAngle;}
    float GetRotationAmount() {return rotationAmount;}
    void SetRotationAmount(float value){rotationAmount = value;}

    //Identifiers
    void SetName(std::string value){name = value;}
    std::string GetName() { return name; }

    //ODE
    void SetupPhysics(dWorldID id, dSpaceID collisionSpace);
    virtual dxGeom* GetGeom() {return geom;}
    dBodyID GetBody(){return body;}
    virtual void SetTorque(float x, float y, float z);
    virtual void SetForce(float x, float y, float z);
    virtual dSpaceID GetSpace(){return space;}

    virtual void SetKinematic(bool value);

    virtual void DisableGravity();

    void SetMass(dReal newMass);
    bool GetCanJump(){return playerCanJump;}
    void SetPlayerJump(bool value){playerCanJump = value;}

    void DisableBody();



protected:
    //Transforms
    ofVec3f position = ofVec3f(0,0,0);
    ofVec3f scale = ofVec3f(1,1,1);;
    ofQuaternion rotation = ofQuaternion(0.f, 0.0f, 1.0f, 0.0f);
    ofVec3f rotationAngle = ofVec3f(0,1,0);
    float rotationAmount = 0.f;

    //Rendering
    ofTexture* texture = new ofTexture();
    ofVec2f textureScale = ofVec2f(1,1);
    bool isTextured = false;

    ofMaterial* material;
    bool hasMaterial = false;

    //ODE
    dWorldID world;
    dSpaceID space;
    dBodyID body;
    dMass mass;
    dGeomID geom;
    bool hasBody = true;
    bool playerCanJump = false;



    bool isKinematic = false;

    //Identifiers
    std::string name = "UNNAMED";


};

#endif // GAMEOBJECT_H
