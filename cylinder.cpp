#include "cylinder.h"

Cylinder::Cylinder(dWorldID w, dSpaceID s, float radius, float length) : GameObject(w,s)
{
    this->radius = radius;
    this->length = length;
    SetupPhysics();
    //TODO MOVE TO PROPER TEXTURE SYSTEM
    if(!ofLoadImage(texture, "dirt2.jpg")) { std::cerr << "Failed to load ground texture." << std::endl; }
    texture.setTextureWrap(GL_REPEAT, GL_REPEAT);
}

Cylinder::~Cylinder(){

}


void Cylinder::Update(float deltaTime){
    GameObject::Update(deltaTime);//SUPER!
    //ODE STUFF!
}

void Cylinder::SetupPhysics(){
    //erase old bodys
    dBodyDestroy(body);
    dGeomDestroy(geom);

    //create new bodys
    body = dBodyCreate(world);
    dQuaternion q;
    dQFromAxisAndAngle(q, 1,0,0, 3*0.5);//TODO USE MATH LIB FOR PI
    dBodySetQuaternion(body, q);
    dMassSetSphere(&mass, 1, this->radius);
    dMassAdjust(&mass, 20);//ALTER MASS!
    dBodySetMass(body, &mass);
    geom = dCreateSphere(space, this->radius);
    dGeomSetBody(geom, body);
}

void Cylinder::Draw(){
    //front left wheel
    ofPushMatrix();

    texture.bind();//bind texture
    ofTranslate(ofVec3f(position.x, position.y, position.z));
    ofRotateDeg(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
    ofDrawCylinder(0,0,0, this->radius, this->length);
    texture.unbind();//unbind texture
    ofPopMatrix();
}


void Cylinder::SetColour(ofColor colour){
    this->colour = colour;
}
