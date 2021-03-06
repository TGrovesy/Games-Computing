#include "ofApp.h"
#include <ode/ode.h>
#include <glm/glm.hpp>
#define LENGTH 0.7      // chassis length
#define WIDTH 0.5       // chassis width
#define HEIGHT 0.2      // chassis height
#define RADIUS 0.18     // wheel radius
#define STARTZ 0.5      // starting height of chassis
#define CMASS 1         // chassis mass
#define WMASS 0.2       // wheel mass

static const dVector3 yunit = { 0, 1, 0 }, zunit = { 0, 0, 1 };
World* ofApp::world;
bool ofApp::keyDown[255];

//--------------------------------------------------------------
void ofApp::setup(){
    ofApp::world = new World();
}

//--------------------------------------------------------------
void ofApp::update(){
    ofApp::world->Update(3);

    //TODO MOVE TO PLAYER!!!!

    Player* player= world->GetPlayer();
    ofVec3f lookat = player->camera.getLookAtDir() * 0.5f;
    ofVec3f side = player->camera.getSideDir() * 0.5f;

    float xSpeed = dBodyGetLinearVel(player->GetBody())[0];

    if(xSpeed < 30){
        if(keyDown['w']){
            ofVec3f t = player->GetPosition();
            t += 0.5f * player->camera.getLookAtDir();
            //player->SetPosition(t);
            dBodyAddForce(player->GetBody(), lookat.x, lookat.y, 0);
        }

        if(keyDown['s']){
            dBodyAddForce(player->GetBody(), -lookat.x, -lookat.y, 0);
        }

        if(keyDown['a']){
            dBodyAddForce(player->GetBody(), -side.x, -side.y, 0);
        }

        if(keyDown['d']){
            dBodyAddForce(player->GetBody(), side.x, side.y, 0);
        }
    }



    if(keyDown['q']){
        ofExit();
    }
}
//--------------------------------------------------------------
void ofApp::draw(){

    ofApp::world->Draw();
}

//--------------------------------------------------------------
void ofApp::exit() {

}




bool press = false;
float x = 0;
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    keyDown[key] = true;

    //std::cout << key << std::endl;
//    cout<<speed<<endl;
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    keyDown[key] = false;
}


int prevMouseX = 0, prevMouseY = 0;
float horizontalAngle = 0, verticalAngle = 90;
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    //TODO MOVE TO PLAYER FILE
    int dX = x - prevMouseX;
    int dY = y - prevMouseY;
    world->GetPlayer()->Rotate(ofVec3f(0,0,1), -dX * 0.25f);
    if(verticalAngle > 22.5 && verticalAngle < 180){
        world->GetPlayer()->Rotate(world->GetPlayer()->camera.getSideDir(), -dY * 0.25f);//vertical
    }


    verticalAngle += -dY * 0.25;

    if(verticalAngle > 180)verticalAngle = 180;
    if(verticalAngle < 22.5) verticalAngle = 22.5;

    //end pos
    prevMouseX = x;
    prevMouseY = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
