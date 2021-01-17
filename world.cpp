#include "world.h"
#include "cube.h"
#include "ofApp.h"
#include "vehicle.h"
#include "cylinder.h"
#include "flyingcar.h"
#include "material.h"
#include"texture.h"
#include <string>
#include <ctime>
#include <stdlib.h>
#include <cmath>

World::World()
{
    SetupWorld();
}

void World::SetupWorld(){
    srand(time(0));
    //Material and Texture Handlers
    Material* material = new Material();
    Texture* texture = new Texture();
    SetupPhysics();
    //Scenes
    TestScene();
    //BlanckScene();
}

//Dispose of all ODE data
World::~World(){
    for(int i = 0; i < (int)gameObjects.size(); i++){
        gameObjects.at(i)->~GameObject();
    }
    player->~Player();
    geoms.clear();

    dJointGroupDestroy(contactGroup);
    dSpaceDestroy(spaceID);
    dWorldDestroy(worldID);
    dCloseODE();
}

/*
 *Sets up all necessary stuff for an ODE simulation
 */
void World::SetupPhysics(){
    //ODE setup Create world
    dInitODE2(0);
    this->worldID = dWorldCreate();
    this->spaceID = dHashSpaceCreate(0);
    this->contactGroup = dJointGroupCreate(0);
    dWorldSetGravity(this->worldID, 0, 0, gravityValue);

    dSpaceCollide(spaceID, 0, &nearCallback);
    dAllocateODEDataForThread(dAllocateMaskAll);
}


void World::SetWorldID(dWorldID id){
    this->worldID = id;
}

void World::SetSpaceID(dSpaceID id){
    this->spaceID = id;
}

static void nearCallback(void *, dGeomID o1, dGeomID o2){

    ofApp::GetWorld()->collide(o1, o2);


}

//Temp Jump Fix
bool hasJumped = false;
//Controlls Collsions
void World::collide(dGeomID o1, dGeomID o2){
    int i,n;
    GameObject* object1 = geoms.find(o1)->second;
    GameObject* object2 = geoms.find(o2)->second;

    if(object1->GetName() == "Truck" || object2->GetName() == "Truck"){
        crashSound.play();
    }

    const int N = 50;
    dContact contact[N];
    n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
    if (n > 0) {
    for (i=0; i<n; i++) {
      contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
        dContactSoftERP | dContactSoftCFM | dContactApprox1;
      contact[i].surface.mu = 1;

      //Controlls Surface Friction
      contact[i].surface.slip1 = 0.2;//Friction DIR 1
      contact[i].surface.slip2 = 0.2;//Friction DIR 2
      contact[i].surface.soft_erp = 0.3;
      contact[i].surface.soft_cfm = 0.1;
      dJointID c = dJointCreateContact (worldID,contactGroup,&contact[i]);
      dJointAttach (c,
                    dGeomGetBody(contact[i].geom.g1),
                    dGeomGetBody(contact[i].geom.g2));
      }
    }
}

void World::Draw(){

    player->FrameBegin();//Start Of Frame
    for(int i = 0; i < (int)gameObjects.size(); i++){
        gameObjects.at(i)->Draw();
    }


   player->FrameEnd();//End Of frame

   //Some usful GUI info
   ofDrawBitmapStringHighlight("Use WASD for movement\n Space for Jump\nP to Play/Restart on death", 20,20);
   std::string scoreStr = "Score: "  + std::to_string(score);
   ofDrawBitmapStringHighlight(scoreStr, 20,80);
}

/*
 * Update function for physics and game mechanics simulation
 */
void World::Update(float deltaTime){
    objsMovedOnUpdate = 0;
    if(!gameStart && ofApp::keyDown['p']) gameStart = true;//starts game by pressing P!
    if(gameStart && !gameOver){
        player->Update(deltaTime);//Update player

        geoms.clear();//delete previous map

        float jumpDev = 0.75f;//Leanancy for player mistake


        for(int i = 0; i < (int)gameObjects.size(); i++){
            GameObject* obj = gameObjects.at(i);
            obj->Update(deltaTime);//Update object
            geoms.insert(pair<dxGeom*, GameObject*>(obj->GetGeom(), obj));//links geom to gameobject

            //Jump Check
            ofVec3f p = obj->GetPosition();
            ofVec3f scaleAdj = obj->GetScale();
            if(obj->GetName() != "Ground"){
                if(std::abs(p.x - player->GetPosition().x) <= scaleAdj.x + jumpDev){
                    if(std::abs(p.y - player->GetPosition().y) <= scaleAdj.y+ jumpDev){
                        if(std::abs(p.z - player->GetPosition().z) <= scaleAdj.z+ jumpDev){
                            if(ofApp::keyDown[32]&&!hasJumped){
                                if(obj->GetName() == "Wall"){
                                    gameOver = true;
                                    break;
                                }
                                //TODO BUG! multiple collisions lead to multiple forces added meaning super high jump
                                dBodyAddForce(player->GetBody(), 0, 0, 55);

                                jumpSound.play();
                                hasJumped = true;
                            }
                        }
                    }
                }
            }
            GamemodeCheck(obj);//Check object is in correct position for gamemode

        }

        if(!ofApp::keyDown[32]) hasJumped = false;

        //Calculate player score
        score = (player->GetPosition().z + player->GetPosition().x) / 10;//height and distance from origin

        //Check game over
        if(player->GetPosition().z <= 2) gameOver = true;
        if(player->GetPosition().x < gameWall->GetPosition().x - 5) gameOver = true;


        //Skybox update
        ofVec3f skyUpPos = skyBoxUp->GetPosition();
        skyBoxUp->SetPosition(ofVec3f(player->GetPosition().x, skyUpPos.y, skyUpPos.z));
        ofVec3f skyDownPos = skyBoxDown->GetPosition();
        skyBoxDown->SetPosition(ofVec3f(player->GetPosition().x, skyDownPos.y, skyDownPos.z));
        ofVec3f skyLeftPos = skyBoxLeft->GetPosition();
        skyBoxLeft->SetPosition(ofVec3f(player->GetPosition().x, skyLeftPos.y, skyLeftPos.z));
        ofVec3f skyRightPos = skyBoxRight->GetPosition();
        skyBoxRight->SetPosition(ofVec3f(player->GetPosition().x, skyRightPos.y, skyRightPos.z));
        ofVec3f skyFrontPos = skyBoxFront->GetPosition();
        skyBoxFront->SetPosition(ofVec3f(15000 + player->GetPosition().x, skyFrontPos.y, skyFrontPos.z));
        ofVec3f skyBackPos = skyBoxBack->GetPosition();
        skyBoxBack->SetPosition(ofVec3f(-15000 + player->GetPosition().x, skyBackPos.y, skyBackPos.z));

        //GamemodeGeneration();
        dSpaceCollide (this->spaceID, 0, &nearCallback);
        dWorldQuickStep (this->worldID, 0.11); //Simulates physics step forward (QUICK STEP FOR BIG PERFORMANCE BOOST)

        dJointGroupEmpty(contactGroup);//Remove all contact joints
    }

    if(gameOver){

        GameReset();
        std::cout<< "Score: " << score << " points!\n";
        gameOver = false;
        gameStart = false;
    }
}


/*
 * Moves gameobjects as needed to create an endless runner
 */
void World::GamemodeCheck(GameObject* obj){


    map<GameObject*, bool>::iterator it = undelteableObj.find(obj);
    if((obj->GetPosition().x < (gameWall->GetPosition().x - 10) || obj->GetPosition().x < (player->GetPosition().x - 100)) && it == undelteableObj.end()){
        //Check if truck
        if(obj->GetName() == "Truck"){

            ofVec3f curPos = obj->GetPosition();
            float randY = ofRandom(-roadWidth/2, roadWidth/2);
            float randX = ofRandom(0, 200);
            //std::cout<< obj->GetName() << std::endl;
            obj->SetPosition(ofVec3f(curPos.x + 250 + randX + (objsMovedOnUpdate * 30), randY, truckSpawnZ));
            obj->SetRotation(ofQuaternion(0,0,0,0));
            obj->SetTorque(0,0,0);
            obj->SetForce(0,0,0);
            //objsMovedOnUpdate++;
            return;//dont contintue to delete
        }

        if(obj->GetName() == "Platform"){//Deleteing and recreating object was way too inefficent for truck and caused stuttering
            ofVec3f curPos = obj->GetPosition();
            float randZ = ofRandom(-150, 150);
            float randY = ofRandom(-roadWidth/2, roadWidth/2);
            float randX = ofRandom(0, 200);

            int adjZ = randZ + player->GetPosition().z;
            if(adjZ <= 15) adjZ = 15;

            obj->SetPosition(ofVec3f(curPos.x + 300 + randX+ (objsMovedOnUpdate * 15), randY, adjZ));
            obj->SetRotation(ofQuaternion(0,0,0,0));
            obj->SetTorque(0,0,0);
            obj->SetForce(0,0,0);
           // objsMovedOnUpdate++;
            return;//dont contintue to delete
        }


        if(obj->GetName() == "Ramp"){
            ofVec3f curPos = obj->GetPosition();
            float randX = ofRandom(200,400);
            float randY = ofRandom(-roadWidth/2, roadWidth/2);
            obj->SetPosition(ofVec3f(curPos.x + 250 + randX + (objsMovedOnUpdate * 30), randY, 1));
            return;//dont contintue to delete
        }


        //Delete Object From Vector
        for (vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it) {
            if (*it == obj) {
                gameObjects.erase(it);
                break;
            }
        }
        obj->~GameObject();//delete object
    }
}

/*
 * Brings game back into a "Starting State"
 */
void World::GameReset(){

    int truck = 0;
    int platform = 0;
    int ramp = 0;
    //Player
    player->SetPosition(ofVec3f(0, 0, 55));
    player->SetForce(0,0,0);

    for(int i = 0; i < (int)gameObjects.size(); i++){
        GameObject* obj = gameObjects.at(i);
        //Truck Reset
        if(obj->GetName() == "Truck"){
            if(truck % 2 == 0){
                obj->SetPosition(ofVec3f(truck*15,-5, 10));
            }else{
                obj->SetPosition(ofVec3f(truck*15,5, 10));
            }
            obj->SetRotation(ofQuaternion(0,0,0,0));
            obj->SetTorque(0,0,0);
            obj->SetForce(0,0,0);
            truck++;
            continue;
        }

        //Platform Reset
        if(obj->GetName() == "Platform"){
            float randHeight = ofRandom(15, 50);
            float randY = ofRandom(0, roadWidth/2);
            if(i % 2 == 0){
                obj->SetPosition(ofVec3f(platform*25, - randY, randHeight));
            }else{
                obj->SetPosition(ofVec3f(platform*25, randY, randHeight));
            }
            obj->SetRotation(ofQuaternion(0,0,0,0));
            obj->SetTorque(0,0,0);
            obj->SetForce(0,0,0);
            platform++;
            continue;
        }

        if(obj->GetName() == "Ramp"){
            float randY = ofRandom(-roadWidth/2, roadWidth/2);
            obj->SetPosition(ofVec3f(ramp++ * 25, randY, 1));
            continue;
        }
    }
    gameWall->SetSpeed(0);
    gameWall->SetPosition(ofVec3f(-200,0,gameWall->GetScale().z / 2));
}




//Empty Scene for testing
void World::BlanckScene(){

    //SCENE CONSTRUCTION!!! TODO MOVE!
    ofDisableAlphaBlending();
    ofDisableArbTex();

    ofEnableLighting();
    ofSetSmoothLighting(true);
    //dirLight = new ofLight();


    //Trucks
    for(int i = 0; i < 30; i++){
        //TEST!
        dSpaceID vSpace = dSimpleSpaceCreate(spaceID);
        dSpaceSetCleanup(vSpace, 0);
        Vehicle* truck = new Vehicle(this->worldID, vSpace, 0);
        if(i % 2 == 0){
            truck->SetPosition(ofVec3f(i*15,-5, 10));
            //truck->Rotate(ofVec3f(0,0,1), 10.5f);
        }else{
            truck->SetPosition(ofVec3f(i*15,5, 10));
            //truck->Rotate(ofVec3f(0,0,1), -12.5f);
        }
        truck->SetName("Truck");
        truck->SetScaling(ofVec3f(2, 2, 2));
        numOfTrucks++;
        gameObjects.push_back(truck);
    }

    //platforms
    for(int i = 0; i < 50; i++){
        FlyingCar* platform = new FlyingCar(this->worldID, this->spaceID);
        int randHeight  = rand() % 80 + 25;
        if(i % 2 == 0){
            platform->SetPosition(ofVec3f(i*15,-5, randHeight));
            //truck->Rotate(ofVec3f(0,0,1), 12.5f);
        }else{
            platform->SetPosition(ofVec3f(i*15,5, randHeight));
            //truck->Rotate(ofVec3f(0,0,1), -12.5f);
        }

        platform->SetFixedPosition(platform->GetPosition(), 4);
        platform->SetName("Platform");
        platform->SetMaterial(Material::GetMaterial("chrome"));
        platform->SetTexture("metal.jpg");
        platform->SetScaling(ofVec3f(10, 10, 1));
        platform->SetMass(100000);

        platform->SetKinematic(true);
        numOfPlatforms++;
        //truck->Rotate(ofVec3f(0,1,0), 45);
        gameObjects.push_back(platform);
    }

    //GROUND
    Cube* ground = new Cube(this->worldID, this->spaceID,  5000, 5000, 1);
    ground->SetPosition(ofVec3f(0.f, 0.f, 0.f));
    ground->SetColour(ofColor::lightGrey);
    ground->SetName("Ground");
    ground->SetTexture("grass.jpg");
    ground->SetTextureScale(ofVec2f(256));
    ground->SetMass(100000);
    ground->SetKinematic(true);
    ground->SetMaterial(Material::GetMaterial("grass"));

    undelteableObj.insert(std::pair<GameObject*, bool>(ground, true));
    gameObjects.push_back(ground);



    //Player
    player = new Player(this->worldID, this->spaceID);
    player->SetPosition(ofVec3f(0, -5, 20));
    player->SetScaling(ofVec3f(1, 1, 3));
    player->SetMass(100);
    player->SetName("Player");

    gameWall = new Wall(worldID, spaceID, 1, 5000, 5000);
    gameWall->SetPosition(ofVec3f(-10000, 0, gameWall->GetScale().z / 2));
    gameWall->SetColour(ofColor::red);
    gameWall->SetName("GameWall");
    undelteableObj.insert(std::pair<GameObject*, bool>(gameWall, true));
    gameObjects.push_back(gameWall);


    ofMaterial* wallMat = new ofMaterial();
    wallMat->setDiffuseColor(ofFloatColor::purple);
    wallMat->setShininess(0.01f);

    //Walls
    Cube* leftWall = new Cube(worldID, spaceID, 5000, 5, 40);
    leftWall ->SetName("leftwall");
    leftWall->SetPosition(ofVec3f(0, -15, 0));
    leftWall->SetKinematic(true);
    leftWall->SetMaterial(wallMat);
    undelteableObj.insert(std::pair<GameObject*, bool>(leftWall, true));
    gameObjects.push_back(leftWall);

    Cube* rightWall = new Cube(worldID, spaceID, 5000, 5, 40);
    rightWall->SetPosition(ofVec3f(0, 15, 0));
    rightWall ->SetName("rightwall");
    rightWall->SetKinematic(true);
    rightWall->SetMaterial(wallMat);
    undelteableObj.insert(std::pair<GameObject*, bool>(rightWall, true));
    gameObjects.push_back(rightWall);


    dirLight.setPosition(ofVec3f(0,0,50));
   // dirLight.
    dirLight.setDirectional();
    dirLight.setSpecularColor(ofColor(255));
    dirLight.setDiffuseColor(ofColor(255));
    dirLight.setAmbientColor(ofColor(25));
    dirLight.tiltDeg(-45);
    dirLight.setup();
    dirLight.enable();
}

void World::TestScene(){
    //SCENE CONSTRUCTION!!! TODO MOVE!
    ofEnableLighting();
    ofSetSmoothLighting(true);
    ofEnableTextureEdgeHack();
    ofEnableAlphaBlending();
    //ofHideCursor();
    //ofToggleFullscreen();

    dirLight.setPosition(ofVec3f(0,0,50));
    dirLight.setDirectional();
    dirLight.setSpecularColor(ofColor(255));
    dirLight.setDiffuseColor(ofColor(255));
    dirLight.setAmbientColor(ofColor(64));
    dirLight.tiltDeg(-45);
    dirLight.setup();
    dirLight.enable();

    //SCENE CONSTRUCTION!!! TODO MOVE!
    ofDisableAlphaBlending();
    ofDisableArbTex();
    //GROUND
    Cube* ground = new Cube(this->worldID, this->spaceID,  roadWidth, roadLength, 1);
    ground->SetPosition(ofVec3f(0.f, 0.f, 0.f));
    ground->SetColour(ofColor::lightGrey);
    ground->Rotate(ofVec3f(0,0,1), 90);
    ground->SetName("Ground");
    ground->SetTexture("road.jpg");
    ground->SetTextureScale(ofVec2f(8,500000));
    ground->SetMaterial(Material::GetMaterial("grass"));
    ground->SetMass(100000);
    ground->SetKinematic(true);
    undelteableObj.insert(std::pair<GameObject*, bool>(ground, true));
    gameObjects.push_back(ground);

    Cube* startingPlatform = new Cube(this->worldID, this->spaceID, 15,15,1);
    startingPlatform->SetName("StartingPlatform");
    startingPlatform->SetPosition(ofVec3f(0, 0, 50));
    startingPlatform->SetKinematic(true);
    startingPlatform->SetPlayerJump(true);
    undelteableObj.insert(std::pair<GameObject*, bool>(startingPlatform, true));
    gameObjects.push_back(startingPlatform);


    //Walls
    Cube* leftWall = new Cube(worldID, spaceID, roadLength, 5, 40);
    leftWall->SetPosition(ofVec3f(0, -(roadWidth/2), 0));
    leftWall->SetName("Wall");
    leftWall->SetKinematic(true);
    undelteableObj.insert(std::pair<GameObject*, bool>(leftWall, true));
    gameObjects.push_back(leftWall);

    Cube* rightWall = new Cube(worldID, spaceID, roadLength, 5, 40);
    rightWall->SetPosition(ofVec3f(0, roadWidth/2, 0));
    rightWall->SetName("Wall");
    rightWall->SetKinematic(true);
    undelteableObj.insert(std::pair<GameObject*, bool>(rightWall, true));
    gameObjects.push_back(rightWall);


    //sounds
    jumpSound.load("jump.aiff");
    crashSound.load("crash.wav");

    for(int i = 0; i < 8; i++){
        float randY = ofRandom((-roadWidth / 2) + 1, (roadWidth / 2) - 1);
        Cube* ramp = new Cube(worldID, spaceID, 15, 35, 3);
        ramp->SetColour(ofColor::blanchedAlmond);
        ramp->SetPosition(ofVec3f((i * 10) + 20, randY, 1));
        ramp->Rotate(ofVec3f(0,1,0), -22.5f);
        ramp->SetName("Ramp");
        ramp->SetKinematic(true);
        gameObjects.push_back(ramp);
    }

    //Player
    player = new Player(this->worldID, this->spaceID);
    player->SetPosition(ofVec3f(0, 0, 55));
    player->Rotate(ofVec3f(1,0,0), 270);
    player->Rotate(ofVec3f(0,0,1), -90);
    //player->SetRotation(ofQuaternion(0,0,0,1));
    player->SetScaling(ofVec3f(1, 1, 3));
    player->SetMass(100);
    player->SetName("Player");


    //Trucks
    for(int i = 0; i < 25; i++){
        //TEST!
        dSpaceID vSpace = dHashSpaceCreate (spaceID);
        dSpaceSetCleanup(vSpace, 0);
        Vehicle* truck = new Vehicle(this->worldID, vSpace, 0);
        if(i % 2 == 0){
            truck->SetPosition(ofVec3f(i*15,-5, truckSpawnZ));
            //truck->Rotate(ofVec3f(0,0,1), 10.5f);
        }else{
            truck->SetPosition(ofVec3f(i*15,5, truckSpawnZ));
            //truck->EnableLights(true);
            //truck->Rotate(ofVec3f(0,0,1), -12.5f);
        }
        truck->SetName("Truck");
        truck->SetScaling(ofVec3f(15, 8, 8));
        truck->SetPlayerJump(true);


        numOfTrucks++;
        gameObjects.push_back(truck);
    }

    for(int i = 0; i < 25; i++){
        FlyingCar* platform = new FlyingCar(this->worldID, this->spaceID);
        int randHeight  = ofRandom(15, 80);
        if(i % 2 == 0){
            platform->SetPosition(ofVec3f(i*15,-5, randHeight));
            //truck->Rotate(ofVec3f(0,0,1), 12.5f);
        }else{
            platform->SetPosition(ofVec3f(i*15,5, randHeight));
            //truck->Rotate(ofVec3f(0,0,1), -12.5f);
        }
        platform->SetKinematic(true);
        //platform->SetFixedPosition(platform->GetPosition(), 4);
        platform->SetName("Platform");
        //platform->SetMaterial(Material::GetMaterial("chrome"));
        //platform->SetTexture("metal.jpg");
        platform->SetScaling(ofVec3f(15, 15, 1));
        platform->SetPlayerJump(true);
        numOfPlatforms++;
        gameObjects.push_back(platform);
    }

    skyBoxUp = new Plane(30000, 30000, 1);
    skyBoxUp->SetName("Skybox");
    skyBoxUp->SetTexture("sky2.jpg");
    skyBoxUp->SetTextureScale(ofVec2f(3,3));
    skyBoxUp->SetPosition(ofVec3f(0,0, 15000));
    skyBoxUp->Rotate(ofVec3f(0,0,1), -90);
    undelteableObj.insert(std::pair<GameObject*, bool>(skyBoxUp,true));
    gameObjects.push_back(skyBoxUp);
    skyBoxLeft = new Plane(30000, 30000, 1);
    skyBoxLeft->SetName("Skybox");
    skyBoxLeft->SetTexture("sky2.jpg");
    skyBoxLeft->SetTextureScale(ofVec2f(3,3));
    skyBoxLeft->SetPosition(ofVec3f(0,-15000, 0));
    skyBoxLeft->Rotate(ofVec3f(1,0,0), 90);
    skyBoxLeft->Rotate(ofVec3f(0,1,0), 180);
    undelteableObj.insert(std::pair<GameObject*, bool>(skyBoxLeft,true));
    gameObjects.push_back(skyBoxLeft);
    skyBoxRight = new Plane(30000, 30000, 1);
    skyBoxRight->SetName("Skybox");
    skyBoxRight->SetTexture("sky2.jpg");
    skyBoxRight->SetTextureScale(ofVec2f(3,3));
    skyBoxRight->SetPosition(ofVec3f(0, 15000, 0));
    skyBoxRight->Rotate(ofVec3f(1,0,0), 90);
    skyBoxRight->Rotate(ofVec3f(0,1,0), 180);
    undelteableObj.insert(std::pair<GameObject*, bool>(skyBoxRight,true));
    gameObjects.push_back(skyBoxRight);
    skyBoxFront = new Plane(30000, 30000, 1);
    skyBoxFront->SetName("Skybox");
    skyBoxFront->SetTexture("sky2.jpg");
    skyBoxFront->SetTextureScale(ofVec2f(3,3));
    skyBoxFront->SetPosition(ofVec3f(15000, 0, 0));
    skyBoxFront->Rotate(ofVec3f(1,0,0), 90);
    skyBoxFront->Rotate(ofVec3f(0,1,0), 180);
    skyBoxFront->Rotate(ofVec3f(0,0,1), 90);
    undelteableObj.insert(std::pair<GameObject*, bool>(skyBoxFront,true));
    gameObjects.push_back(skyBoxFront);
    skyBoxDown = new Plane(30000, 30000, 1);
    skyBoxDown->SetName("Skybox");
    skyBoxDown->SetTexture("sky2.jpg");
    skyBoxDown->SetTextureScale(ofVec2f(3,3));
    skyBoxDown->SetPosition(ofVec3f(0, 0, -15000));
    undelteableObj.insert(std::pair<GameObject*, bool>(skyBoxDown,true));
    gameObjects.push_back(skyBoxDown);
    skyBoxBack = new Plane(30000, 30000, 1);
    skyBoxBack->SetName("Skybox");
    skyBoxBack->SetTexture("sky2.jpg");
    skyBoxBack->SetTextureScale(ofVec2f(3,3));
    skyBoxBack->SetPosition(ofVec3f(-15000, 0, 0));
    skyBoxBack->Rotate(ofVec3f(1,0,0), 90);
    skyBoxBack->Rotate(ofVec3f(0,1,0), 180);
    skyBoxBack->Rotate(ofVec3f(0,0,1), 90);
    undelteableObj.insert(std::pair<GameObject*, bool>(skyBoxBack,true));
    gameObjects.push_back(skyBoxBack);


    gameWall = new Wall(worldID, spaceID, 1, 500, 500);
    gameWall->SetPosition(ofVec3f(-100, 0, gameWall->GetScale().z / 2));
    gameWall->SetColour(ofColor::red);
    gameWall->SetKinematic(true);
    gameWall->SetName("GameWall");
    undelteableObj.insert(std::pair<GameObject*, bool>(gameWall, true));
    gameObjects.push_back(gameWall);


}


