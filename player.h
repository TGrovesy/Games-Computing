#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"

class Player : public GameObject
{
public:
    Player(dWorldID w, dSpaceID s);


    virtual void Update(float deltaTime);
    virtual void Draw();

    void SetRotation(ofQuaternion rotation);
    void Rotate(ofVec3f rotationAxis, float amount);

    void FrameBegin();
    void FrameEnd();


    ofCamera &GetCamera(){return camera;}

    //TODO MAKE PRIVEATE
    float horizontalAngle;
    float verticalAngle;
    float angularSpeed;
    ofVec3f lookAt;
    ofCamera camera;

private:
    void SetupCamera();
};

#endif // PLAYER_H
