#ifndef SHAPESGENERATOR_HPP
#define SHAPESGENERATOR_HPP

#include <iostream>
#include <list>
#include <string>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include <vector>

struct Vertices {
    float x, y, z;
};

void generatePlane(float length, int divisions, std::string fileName);
void generateBox(float length, int divisions, std::string fileName);
void generateSphere(float radius, int slices, int stacks, std::string fileName);
void generateCone(float radius, float height, int slices, int stacks, std::string fileName);
void generateBezierPatch(std::string fileToFetch, int tessellationLevel, std::string fileName);
std::vector<std::vector<Vertices>> readBezierFile(std::string fileToFetch);
Vertices bezierCoeficientCalculator(std::vector<Vertices> patchVertices, float u, float v);
void generateBezierPatch(std::string fileToFetch, int tessellationLevel, std::string fileName);
void writeInFileList(std::list<Vertices> all_vertices, std::list<Vertices> all_normais , std::list<Vertices> all_texturas, std::string fileName);
void writeInFileVector(std::vector<Vertices> all_vertices, std::vector<Vertices> all_normais, std::vector<Vertices> all_texturas, std::string fileName);
Vertices cross(Vertices a, Vertices b);
Vertices normalize(Vertices a);

#endif
