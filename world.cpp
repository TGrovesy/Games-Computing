#include "world.h"
#include "cube.h"
#include "ofApp.h"
#include <string>

World::World()
{
    SetupWorld();
}

void World::SetupWorld(){
    SetupPhysics();

    //Player
    std::cout<<"here\n";
    player = new Player(this->worldID, this->spaceID);
    player->SetPosition(ofVec3f(10, 10, 10));
    player->SetScaling(ofVec3f(1, 1, 3));
    player->SetMass(100);
    //player->SetName("Player");
    //player->SetKinematic(true);
    //TODO: DEBUG TO TEST REMOVE!
    for(int i = 0; i < 2; i++){
        Cube* cube = new Cube(this->worldID, this->spaceID, 5,5,5);
        cube->SetMass(1000);
        if(i == 0) cube->SetMass(5000);
        cube->SetPosition(ofVec3f(0, 0, ((float)i * 5) + 2.55));
        gameObjects.push_back(cube);
    }

    ground = new Cube(this->worldID, this->spaceID,  50, 50, 1);
    ground->SetPosition(ofVec3f(0.f, 0.f, 0));
    ground->SetColour(ofColor::green);
    //ground->Freeze();
    ground->SetMass(1000000);
    ground->SetKinematic(true);
    gameObjects.push_back(ground);
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


void World::collide(dGeomID o1, dGeomID o2){
    int i,n;

      // only collide things with the ground
      int g1 = (o1 == ground->GetGeom());
      int g2 = (o2 == ground->GetGeom());
      //if (!(g1 ^ g2)) return;
      //if(geoms.find(o1)->second->GetName() == "Player"||geoms.find(o2)->second->GetName() == "Player")return;
      const int N = 10;
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
        //std::cout << "o1: " << geoms.find(o1)->second->GetName() << std::endl;
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
    }


    dSpaceCollide (this->spaceID, 0, &nearCallback);
    dWorldStep (this->worldID, 0.05); //Simulates physics

    dJointGroupEmpty(contactGroup);//Remove all contact joints
}


