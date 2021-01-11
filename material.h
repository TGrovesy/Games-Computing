#ifndef MATERIAL_H
#define MATERIAL_H
#include<map>
#include<string>
#include <ofMain.h>

class Material
{
public:
    Material();

    static ofMaterial* GetMaterial(std::string matName);

private:
    void CreateMaterials();
    static std::map<std::string, ofMaterial*>* materials;

};

#endif // MATERIAL_H
