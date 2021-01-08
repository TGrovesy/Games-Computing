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

    void SetFixedHeight(float value){fixedHeight = value;}

private:
    float fixedHeight = 34.f;
    void SetupBody();
    void ForceUpright();
};

#endif // FLYINGCAR_H
