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

World::World()
{
    SetupWorld();
}

void World::SetupWorld(){
    srand(time(0));
    Material* material = new Material();
    Texture* texture = new Texture();
    SetupPhysics();
    //TestScene();
    BlanckScene();
}

void World::SetupPhysics(){
    //ODE setup Create world
    dInitODE2(0);
    this->worldID = dWorldCreate();
    this->spaceID = dHashSpaceCreate(0);
    this->contactGroup = dJointGroupCreate(0);
    dWorldSetGravity(this->worldID, 0, 0, gravityValue);

    //DEBUG REMOVE AND REPLACE WITH PROPER GROUND
    //groundTMP = dCreatePlane(this->spaceID, 0, 0, 1, 0);//THIS IS WHAT COLLIDES
    dWorldSetDamping (worldID, 0.001,0.001);
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

//TODO INCORRECT JUMP METHOD CHANGE THIS
bool hasJumped = false;

void World::collide(dGeomID o1, dGeomID o2){
    int i,n;
    GameObject* object1 = geoms.find(o1)->second;
    GameObject* object2 = geoms.find(o2)->second;
    const int N = 100;
    dContact contact[N];
    n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
    if (n > 0) {
    for (i=0; i<n; i++) {
      contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
        dContactSoftERP | dContactSoftCFM | dContactApprox1;
      contact[i].surface.mu = 1;
      contact[i].surface.slip1 = 0.1;
      contact[i].surface.slip2 = 0.1;
      contact[i].surface.soft_erp = 0.5;
      contact[i].surface.soft_cfm = 0.3;
      dJointID c = dJointCreateContact (worldID,contactGroup,&contact[i]);
      dJointAttach (c,
                    dGeomGetBody(contact[i].geom.g1),
                    dGeomGetBody(contact[i].geom.g2));
      //OBJECT EXAMPLE TEST DEBUG REMOVE

      if((object1->GetGeom() == player->GetGeom() || object2->GetGeom() == player->GetGeom()) && object1 != object2 ){//contact check for "ground" check NEEDS MOVING
          if(ofApp::keyDown[32]&&!hasJumped){//TODO Add on ground check
              //TODO BUG! multiple collisions lead to multiple forces added meaning super high jump
              dBodyAddForce(player->GetBody(), 0, 0, 70);
              hasJumped = true;
              std::cout << "o1: " << object1->GetName() << ", o2: " << object2->GetName() << std::endl;
          }
          if(!ofApp::keyDown[32]) hasJumped = false;
      }

  }
}
}

void World::Draw(){

    player->FrameBegin();//Start Of Frame
    dirLight.draw();
    //Draw Axis
    ofSetColor(ofColor::lightGrey);
    //ofDrawGrid(0.5f, 100, false, false,false,true);
    ofDrawAxis(10);

    for(int i = 0; i < (int)gameObjects.size(); i++){
        gameObjects.at(i)->Draw();
    }

   player->FrameEnd();//End Of frame
}

void World::Update(float deltaTime){
    player->Update(deltaTime);//Update player

    //Update GameObjects and Map Geoms for contacts
    //THIS IS PROBABLY HORRIBLEY INEFFICENT
    //TODO: Make more efficent
    geoms.clear();//delete previous map
    //geoms.insert(pair<dxGeom*, GameObject*>(player->GetGeom(), player));//Player is not in vector so insert here
    for(int i = 0; i < (int)gameObjects.size(); i++){
        gameObjects.at(i)->Update(deltaTime);
        geoms.insert(pair<dxGeom*, GameObject*>(gameObjects.at(i)->GetGeom(), gameObjects.at(i)));//links geom to gameobject
        GamemodeCheck(gameObjects.at(i));
    }

    GamemodeGeneration();
    dSpaceCollide (this->spaceID, 0, &nearCallback);
    dWorldQuickStep (this->worldID, 0.1); //Simulates physics step forward (QUICK STEP FOR BIG PERFORMANCE BOOST)

    dJointGroupEmpty(contactGroup);//Remove all contact joints
}

void World::GamemodeCheck(GameObject* obj){
    map<GameObject*, bool>::iterator it = undelteableObj.find(obj);
    if(obj->GetPosition().x < (gameWall->GetPosition().x - 10) && it == undelteableObj.end()){
        //Check if truck
        if(obj->GetName().compare("Truck")) numOfTrucks--;
        if(obj->GetName().compare("Platform")) numOfPlatforms--;



        //Delete Object From Vector
        for (vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it) {
            if (*it == obj) {
                gameObjects.erase(it);
                break;
            }
        }
        obj->~GameObject();//delete object
        //std::cout << obj->GetName() << " delete\n";
    }
}

void World::GamemodeGeneration(){
    if(numOfTrucks < 10){//TODO const value for trucks
        dSpaceID vSpace = dSimpleSpaceCreate(spaceID);
        dSpaceSetCleanup(vSpace, 0);
        Vehicle* truck = new Vehicle(this->worldID, vSpace, 0);
        truck->SetPosition(ofVec3f(gameWall->GetPosition().x + 500, -5, 10));
        truck->SetName("Truck");
        truck->SetScaling(ofVec3f(1, 1, 1));
        numOfTrucks++;
        gameObjects.push_back(truck);
    }

    if(numOfPlatforms < 10){//TODO const value for trucks
        FlyingCar* platform = new FlyingCar(this->worldID, this->spaceID);

        platform->SetPosition(ofVec3f(gameWall->GetPosition().x + 500, -5, 35));

        platform->SetFixedPosition(platform->GetPosition(), 4);
        platform->SetName("Platform");
        platform->SetScaling(ofVec3f(10, 10, 1));
        platform->SetMass(100000);
        platform->DisableGravity();
        numOfPlatforms++;
        //truck->Rotate(ofVec3f(0,1,0), 45);
        gameObjects.push_back(platform);
    }
}

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
        truck->SetScaling(ofVec3f(1, 1, 1));
        numOfTrucks++;
        gameObjects.push_back(truck);
    }

    //GROUND
    Cube* ground = new Cube(this->worldID, this->spaceID,  5000, 5000, 1);
    ground->SetPosition(ofVec3f(0.f, 0.f, 0.f));
    ground->SetColour(ofColor::lightGrey);
    ground->SetName("Ground");
    ground->SetTexture("grass.jpg");
    ground->SetTextureScale(256);
    ground->SetMass(100000);
    ground->SetKinematic(true);
    ground->SetMaterial(Material::GetMaterial("grass"));

    undelteableObj.insert(std::pair<GameObject*, bool>(ground, true));
    gameObjects.push_back(ground);



    //Player
    //std::cout<<"here\n";
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

    dirLight.setPosition(ofVec3f(0,0,50));
    dirLight.setDirectional();
    dirLight.setSpecularColor(ofColor(255));
    dirLight.setDiffuseColor(ofColor(255));
    dirLight.setAmbientColor(ofColor(25));
    dirLight.tiltDeg(-45);
    dirLight.setup();
    dirLight.enable();

    //SCENE CONSTRUCTION!!! TODO MOVE!
    ofDisableAlphaBlending();
    ofDisableArbTex();
    //GROUND
    Cube* ground = new Cube(this->worldID, this->spaceID,  5000, 5000, 1);
    ground->SetPosition(ofVec3f(0.f, 0.f, 0.f));
    ground->SetColour(ofColor::lightGrey);
    ground->SetName("Ground");
    ground->SetTexture("grass.jpg");
    ground->SetTextureScale(256);
    ground->SetMaterial(Material::GetMaterial("grass"));
    ground->SetMass(100000);
    ground->SetKinematic(true);
    undelteableObj.insert(std::pair<GameObject*, bool>(ground, true));
    gameObjects.push_back(ground);


    //Walls
    Cube* leftWall = new Cube(worldID, spaceID, 5000, 5, 40);
    leftWall->SetPosition(ofVec3f(0, -15, 0));
    leftWall->SetName("LeftWall");
    leftWall->SetKinematic(true);
    undelteableObj.insert(std::pair<GameObject*, bool>(leftWall, true));
    gameObjects.push_back(leftWall);

    Cube* rightWall = new Cube(worldID, spaceID, 5000, 5, 40);
    rightWall->SetPosition(ofVec3f(0, 15, 0));
    rightWall->SetName("RightWall");
    rightWall->SetKinematic(true);
    undelteableObj.insert(std::pair<GameObject*, bool>(rightWall, true));
    gameObjects.push_back(rightWall);

    //Player
    player = new Player(this->worldID, this->spaceID);
    player->SetPosition(ofVec3f(-10, -10, 10));
    player->SetScaling(ofVec3f(1, 1, 3));
    player->SetMass(100);
    player->SetName("Player");
    //player->SetKinematic(true);
    //TODO: DEBUG TO TEST REMOVE!
    for(int i = 0; i < 2; i++){
        Cube* cube = new Cube(this->worldID, this->spaceID, 5,5,5);
        cube->SetMass(1000);
        cube->SetPosition(ofVec3f(50, 50, ((float)i * 5) + 2.55));
        cube->SetTexture("metal.jpg");
        cube->SetMaterial(Material::GetMaterial("chrome"));
        gameObjects.push_back(cube);
    }


    //Trucks
    for(int i = 0; i < 50; i++){
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
        truck->SetScaling(ofVec3f(1, 1, 1));
        numOfTrucks++;
        gameObjects.push_back(truck);
    }

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
        platform->DisableGravity();
        numOfPlatforms++;
        //truck->Rotate(ofVec3f(0,1,0), 45);
        gameObjects.push_back(platform);
    }


    gameWall = new Wall(worldID, spaceID, 1, 500, 500);
    gameWall->SetPosition(ofVec3f(-100, 0, gameWall->GetScale().z / 2));
    gameWall->SetColour(ofColor::red);
    gameWall->SetName("GameWall");
    undelteableObj.insert(std::pair<GameObject*, bool>(gameWall, true));
    gameObjects.push_back(gameWall);


}


