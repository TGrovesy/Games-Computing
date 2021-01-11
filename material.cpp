#include "material.h"

std::map<std::string, ofMaterial*>* Material::materials = new std::map<std::string, ofMaterial*>();

Material::Material()
{
    CreateMaterials();
}

void Material::CreateMaterials(){
    ofMaterial* grass = new ofMaterial();
    grass->setDiffuseColor(ofFloatColor::green);
    grass->setAmbientColor(ofFloatColor::green / 2);
    grass->setShininess(0.01f);
    grass->setSpecularColor(ofColor(0,0,0));
    materials->insert(std::pair<std::string, ofMaterial*>("grass", grass));

    ofMaterial* chrome = new ofMaterial();
    chrome->setAmbientColor(ofColor(0.25,0.25,0.25));
    chrome->setDiffuseColor(ofColor(0.4,0.4,0.4));
    chrome->setSpecularColor(ofColor(0.774597,0.774597,0.774597));
    chrome->setShininess(0.6);
    materials->insert(std::pair<std::string, ofMaterial*>("chrome", chrome));

    ofMaterial* rubber = new ofMaterial();
    rubber->setAmbientColor(ofColor(0.02,0.02,0.02));
    rubber->setDiffuseColor(ofColor(0.01,0.01,0.01));
    rubber->setSpecularColor(ofColor(0.4,0.4,0.4));
    rubber->setShininess(0.078125);
    materials->insert(std::pair<std::string, ofMaterial*>("rubber", rubber));
}


ofMaterial* Material::GetMaterial(std::string matName){
    ofMaterial* material = materials->at(matName);
    return material;
}
