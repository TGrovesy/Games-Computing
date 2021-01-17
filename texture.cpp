#include "texture.h"
#include <ofImage.h>
#include"ofApp.h"

std::map<std::string, ofTexture*>* Texture::textures = new std::map<std::string, ofTexture*>();

Texture::Texture()
{
    CreateTextures();
}

void Texture::CreateTextures(){
    ofTexture* grass = new ofTexture();
    if(!ofLoadImage(*grass, "grass.jpg")) { std::cerr << "Failed to load ground texture." << std::endl; }
    textures->insert(std::pair<std::string, ofTexture*>("grass", grass));



    ofTexture* metal = new ofTexture();
    if(!ofLoadImage(*metal, "metal.jpg")) { std::cerr << "Failed to load metal texture." << std::endl; }
    textures->insert(std::pair<std::string, ofTexture*>("metal", metal));

    ofTexture* rubber = new ofTexture();
    std::string path = "tire2.jpg";
    if(!ofLoadImage(*rubber, path)) { std::cerr << "Failed to load metal texture." << std::endl; }
    textures->insert(std::pair<std::string, ofTexture*>("rubber", rubber));

    ofTexture* road = new ofTexture();
    if(!ofLoadImage(*road, "road.jpg")) { std::cerr << "Failed to load road texture." << std::endl; }
    textures->insert(std::pair<std::string, ofTexture*>("road", road));
}


ofTexture* Texture::GetTexture(std::string matName){
    ofTexture* texture = textures->at(matName);
    return texture;
}
