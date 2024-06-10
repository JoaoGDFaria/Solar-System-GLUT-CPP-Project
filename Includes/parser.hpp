#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <sstream>
#include "tinyxml2.h"

struct Vertices {
    float x, y, z;
};

struct TextureColor {
    std::string textureName;
    std::unordered_map<std::string,Vertices> cores;
};

struct Transformation {
    std::string name;
    std::list<char> tran;
    std::vector<float> values;
    TextureColor texture;
};

struct Light{
    std::string nome;
    std::vector<float> direcao;
    std::vector<float> posicao;
    float angulo; 
};


int countElements(const std::string& str);
std::tuple<std::unordered_map<std::string, std::unordered_map<std::string, std::string>>,std::vector<std::vector<Vertices>>, std::unordered_map<std::string, TextureColor>> xmlParser(const char* fileName);
void allGroupsInfo(tinyxml2::XMLElement* grupo, int cont, std::string transformations, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>* engineInfo, std::vector<std::vector<Vertices>>* allCatmullPoints, std::unordered_map<std::string, TextureColor>* textureColorinfo);
std::tuple<std::list<Vertices>, std::list<Vertices>, std::list<Vertices>> files3dParser(const char* fileName);
std::list<Transformation> allModelsxml(std::unordered_map<std::string, std::unordered_map<std::string, std::string>> engineInfo, std::unordered_map<std::string, TextureColor> textureColorinfo);
std::list<std::string> divideString (std::string parameters, char divider);
std::vector<Light> populateLights(std::unordered_map<std::string, std::string> lightsInfo);

#endif
