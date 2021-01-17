#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <map>

#include "ode/ode.h"
#include "gameobject.h"
#include "cube.h"
#include "player.h"
#include "wall.h"
#include "plane.h"

class World
{
public:
    World();
    ~World();


    void SetWorldID(dWorldID id);
    void SetSpaceID(dSpaceID id);
    dWorldID GetWorldID(){return worldID;}
    dSpaceID GetSpaceID(){return spaceID;}

    void Draw();
    void Update(float deltaTime);
    void collide(dGeomID o1, dGeomID o2);

    void InsertObject(GameObject* obj){gameObjects.push_back(obj);}
    std::vector<GameObject*> GetGameObjects(){return gameObjects;}
    Player* GetPlayer(){return player;}

private:
    //Objects
    std::vector<GameObject*> gameObjects;
    std::map<dxGeom*, GameObject*> geoms;

    //Sky Box
    Plane* skyBoxUp;
    Plane* skyBoxDown;
    Plane* skyBoxLeft;
    Plane* skyBoxRight;
    Plane* skyBoxBack;
    Plane* skyBoxFront;

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
    bool gameStart = false;
    bool gameOver = false;
    void GamemodeCheck(GameObject* obj);
    void GameReset();
    float roadWidth = 256;
    float roadLength = 1000000;
    float score = 0.f;
    //void GamemodeGeneration();

    //Gamemode bug fixing variables
    int objsMovedOnUpdate = 0;

    Wall* gameWall;
    std::map<GameObject*, bool> undelteableObj;

    int numOfTrucks = 0;
    int numOfPlatforms = 0;

    float truckSpawnZ = 12.f;

    //sounds
    ofSoundPlayer jumpSound;
    ofSoundPlayer crashSound;
};

#endif // WORLD_H

/* ODE requires a global function to use as the collision callback; this
* function, combined with the ofApp pointer, allows us to put the collision
* code within myApp. Look at the .cpp for details of how this works.
*/

static void nearCallback (void *data, dGeomID o1, dGeomID o2);
extern World *worldInstance;
