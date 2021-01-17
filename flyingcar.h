#ifndef FLYINGCAR_H
#define FLYINGCAR_H

#include "gameobject.h"

class FlyingCar : public GameObject
{
public:
    FlyingCar(dWorldID worldID, dSpaceID spaceID);
    ~FlyingCar();
    virtual void Update(float deltaTime);
    virtual void Draw();

    void SetPosition(ofVec3f pos);

    void SetFixedPosition(ofVec3f pos, float maxDeviation);
    void SetFixedHeight(float value){fixedHeight = value;}

private:
    ofVec3f fixedPos;
    float fixedHeight = 34.f;
    void SetupBody(dWorldID worldID, dSpaceID spaceID);
    void ForceUpright();

    float maxDeviation = 2.0f;
};

#endif // FLYINGCAR_H
