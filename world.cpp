#include "world.h"
#include "cube.h"
#include "ofApp.h"
#include "vehicle.h"
#include "cylinder.h"
#include "flyingcar.h"
#include <string>

World::World()
{
    SetupWorld();
}

void World::SetupWorld(){
    SetupPhysics();
    TestScene();

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


      // only collide things with the ground
      //int g1 = (o1 == ground->GetGeom());
      //int g2 = (o2 == ground->GetGeom());
      //if (!(g1 ^ g2)) return;
      if(object1->GetName().compare("Player")||object2->GetName().compare("Player")){//contact check for "ground" check NEEDS MOVING
          if(ofApp::keyDown[32]&&!hasJumped){//TODO Add on ground check
              //TODO BUG! multiple collisions lead to multiple forces added meaning super high jump
              dBodyAddForce(player->GetBody(), 0, 0, 70);
              hasJumped = true;
          }
          if(!ofApp::keyDown[32]) hasJumped = false;
      }

    //if(object1->GetName().compare("Truck")||object2->GetName().compare("Truck")) return;

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
        //std::cout << "o1: " << geoms.find(o1)->second->GetName() << ", o2: " << geoms.find(o2)->second->GetName()<< std::endl;
        }
      }
}

void World::Draw(){
    player->FrameBegin();//Start Of Frame

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
    dWorldQuickStep (this->worldID, 0.05); //Simulates physics step forward (QUICK STEP FOR BIG PERFORMANCE BOOST)

    dJointGroupEmpty(contactGroup);//Remove all contact joints
}

void World::GamemodeCheck(GameObject* obj){
    map<GameObject*, bool>::iterator it = undelteableObj.find(obj);
    if(obj->GetPosition().x < (gameWall->GetPosition().x - 10) && it == undelteableObj.end()){
        //Check if truck
        if(obj->GetName().compare("Truck")) numOfTrucks--;



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
        truck->SetScaling(ofVec3f(11, 8, 8));
        numOfTrucks++;
        gameObjects.push_back(truck);
    }
}

void World::TestScene(){
    //SCENE CONSTRUCTION!!! TODO MOVE!

    //GROUND
    Cube* ground = new Cube(this->worldID, this->spaceID,  5000, 5000, 1);
    ground->SetPosition(ofVec3f(0.f, 0.f, 0.f));
    ground->SetColour(ofColor::green);
    ground->SetName("Ground");
    //ground->Freeze();
    ground->SetMass(100000);
    ground->SetKinematic(true);
    undelteableObj.insert(std::pair<GameObject*, bool>(ground, true));
    gameObjects.push_back(ground);

    //Walls
    Cube* leftWall = new Cube(worldID, spaceID, 5000, 5, 40);
    leftWall->SetPosition(ofVec3f(0, -15, 0));
    leftWall->SetKinematic(true);
    undelteableObj.insert(std::pair<GameObject*, bool>(leftWall, true));
    gameObjects.push_back(leftWall);

    Cube* rightWall = new Cube(worldID, spaceID, 5000, 5, 40);
    rightWall->SetPosition(ofVec3f(0, 15, 0));
    rightWall->SetKinematic(true);
    undelteableObj.insert(std::pair<GameObject*, bool>(rightWall, true));
    gameObjects.push_back(rightWall);

    //Player
    //std::cout<<"here\n";
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
        gameObjects.push_back(cube);
    }


    //Trucks
    for(int i = 0; i < 100; i++){
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
        truck->SetScaling(ofVec3f(11, 8, 8));
        numOfTrucks++;
        gameObjects.push_back(truck);
    }

    for(int i = 0; i < 100; i++){
        FlyingCar* truck = new FlyingCar(this->worldID, this->spaceID);
        if(i % 2 == 0){
            truck->SetPosition(ofVec3f(i*15,-5, 35));
            //truck->Rotate(ofVec3f(0,0,1), 12.5f);
        }else{
            truck->SetPosition(ofVec3f(i*15,5, 35));
            //truck->Rotate(ofVec3f(0,0,1), -12.5f);
        }
        truck->SetName("Flying_Car");
        truck->SetScaling(ofVec3f(10, 10, 1));
        truck->SetMass(100000);
        truck->DisableGravity();
        //truck->Rotate(ofVec3f(0,1,0), 45);
        gameObjects.push_back(truck);
    }


    gameWall = new Wall(worldID, spaceID, 1, 500, 500);
    gameWall->SetPosition(ofVec3f(-100, 0, gameWall->GetScale().z / 2));
    gameWall->SetColour(ofColor::red);
    gameWall->SetName("GameWall");
    undelteableObj.insert(std::pair<GameObject*, bool>(gameWall, true));
    gameObjects.push_back(gameWall);

    Cylinder* cy = new Cylinder(this->worldID, this->spaceID, 3, 3);
    cy->SetPosition(ofVec3f(5,5,10));
    gameObjects.push_back(cy);

    //groundTMP = dCreatePlane(spaceID, 0, 0, 1,0);//TODO REMOVE THIS IS TO TEST TORQUE
}


