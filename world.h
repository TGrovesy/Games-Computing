#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <map>

#include "ode/ode.h"
#include "gameobject.h"
#include "cube.h"
#include "player.h"
#include "wall.h"

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

    std::vector<GameObject*> GetGameObjects(){return gameObjects;}
    Player* GetPlayer(){return player;}

private:
    //Objects
    std::vector<GameObject*> gameObjects;
    std::map<dxGeom*, GameObject*> geoms;

    //ofLight tmpLight;

    //Player
    Player* player;

    //Physics
    dWorldID worldID;
    dSpaceID spaceID;
    dJointGroupID contactGroup;
    float gravityValue = -0.5f;
    //static void nearCallback(void *, dGeomID o1, dGeomID o2);

    //DEBUG
    void TestScene();
    void BlanckScene();
    ofLight dirLight;

    void SetupWorld();
    void SetupPhysics();

    //Gamemode (Endless Runner)
    void GamemodeCheck(GameObject* obj);
    void GamemodeGeneration();

    Wall* gameWall;
    std::map<GameObject*, bool> undelteableObj;

    int numOfTrucks = 0;
    int numOfPlatforms = 0;
};

#endif // WORLD_H

/* ODE requires a global function to use as the collision callback; this
* function, combined with the ofApp pointer, allows us to put the collision
* code within myApp. Look at the .cpp for details of how this works.
*/

static void nearCallback (void *data, dGeomID o1, dGeomID o2);
extern World *worldInstance;
