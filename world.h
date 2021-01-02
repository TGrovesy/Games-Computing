#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "ode/ode.h"
#include "gameobject.h"
#include "cube.h"

class World
{
public:
    World();


    void SetWorldID(dWorldID id);
    void SetSpaceID(dSpaceID id);
    dWorldID GetWorldID(){return worldID;}
    dSpaceID GetSpaceID(){return spaceID;}

    void Draw();
    void Update(float deltaTime);
    void collide(dGeomID o1, dGeomID o2);

private:
    std::vector<GameObject*> gameObjects;

    //Physics
    dWorldID worldID;
    dSpaceID spaceID;
    dJointGroupID contactGroup;
    float gravityValue = -.5f;
    //static void nearCallback(void *, dGeomID o1, dGeomID o2);

    //DEBUG
    dSpaceID groundSpace;
    dGeomID groundTMP;
    Cube* ground;

    void SetupWorld();
    void SetupPhysics();
};

#endif // WORLD_H

/* ODE requires a global function to use as the collision callback; this
* function, combined with the ofApp pointer, allows us to put the collision
* code within myApp. Look at the .cpp for details of how this works.
*/

static void nearCallback (void *data, dGeomID o1, dGeomID o2);
extern World *worldInstance;
