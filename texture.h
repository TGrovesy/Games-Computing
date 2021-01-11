#ifndef TEXTURE_H
#define TEXTURE_H
#include<ofMain.h>
#include <string>
#include <map>


class Texture
{
public:
    Texture();
    static ofTexture GetTexture(std::string textureName);

private:
    void CreateTextures();
    static std::map<std::string, ofTexture>* textures;
};

#endif // TEXTURE_H
