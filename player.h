#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"
#include <string>

class Player : public GameObject
{
public:
    Player(dWorldID w, dSpaceID s);
    ~Player();


    virtual void Update(float deltaTime);
    virtual void Draw();

    void SetRotation(ofQuaternion rotation);
    void Rotate(ofVec3f rotationAxis, float amount);

    void SetPosition(ofVec3f position);

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
