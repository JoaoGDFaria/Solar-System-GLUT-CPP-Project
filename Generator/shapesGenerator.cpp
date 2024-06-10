#include "../Includes/shapesGenerator.hpp"

void generatePlane(float length, int divisions, std::string fileName){

    std::list<Vertices> all_vertices;
    std::list<Vertices> all_normais;
    std::list<Vertices> all_texturas;

    float metadeTamanho = length/2;
    float tamanhoPorDivisao = length/divisions;

    float x, y=0, z;    

    for(int i = 0; i < divisions; i++){
        for(int j = 0; j < divisions; j++){
            x = -metadeTamanho + j * tamanhoPorDivisao;
            z = metadeTamanho - i * tamanhoPorDivisao;

            float s1 = static_cast<float>(j) / divisions;
            float s2 = static_cast<float>(j + 1) / divisions;
            float t1 = static_cast<float>(i) / divisions;
            float t2 = static_cast<float>(i + 1) / divisions;

            // 1º Triângulo
            all_vertices.push_back({x, y, z});
            all_vertices.push_back({x + tamanhoPorDivisao, y, z - tamanhoPorDivisao});
            all_vertices.push_back({x, y, z - tamanhoPorDivisao});

            all_normais.push_back({0, 1, 0});
            all_normais.push_back({0, 1, 0});
            all_normais.push_back({0, 1, 0});

            all_texturas.push_back({s1, t1, 0});
            all_texturas.push_back({s2, t2, 0});
            all_texturas.push_back({s1, t2, 0});

            // 2º Triângulo
            all_vertices.push_back({x, y, z});
            all_vertices.push_back({x + tamanhoPorDivisao, y, z});
            all_vertices.push_back({x + tamanhoPorDivisao, y, z - tamanhoPorDivisao});

            all_normais.push_back({0, 1, 0});
            all_normais.push_back({0, 1, 0});
            all_normais.push_back({0, 1, 0});

            all_texturas.push_back({s1, t1, 0});
            all_texturas.push_back({s2, t1, 0});
            all_texturas.push_back({s2, t2, 0});
        }
    }

    writeInFileList(all_vertices, all_normais, all_texturas, "../3dFiles/"+fileName);
}



void generateBox(float length, int divisions, std::string fileName){
    
        std::list<Vertices> all_vertices;
        std::list<Vertices> all_normais;
        std::list<Vertices> all_texturas;
    
        float metadeTamanho = length/2;
        float tamanhoPorDivisao = length/divisions;
    
        float x, y, z;
    
    for(int i = 0; i < divisions; i++){
        for(int j = 0; j < divisions; j++){

            float s1 = static_cast<float>(j) / divisions;
            float s2 = static_cast<float>(j + 1) / divisions;
            float t1 = static_cast<float>(i) / divisions;
            float t2 = static_cast<float>(i + 1) / divisions;

            // Frente
            x = -metadeTamanho + j * tamanhoPorDivisao;
            y = metadeTamanho;
            z = metadeTamanho - i * tamanhoPorDivisao;
    
            // 1º Triângulo
            all_vertices.push_back({x, y, z});
            all_vertices.push_back({x + tamanhoPorDivisao, y, z - tamanhoPorDivisao});
            all_vertices.push_back({x, y, z - tamanhoPorDivisao});

            all_normais.push_back({0, 1, 0});
            all_normais.push_back({0, 1, 0});
            all_normais.push_back({0, 1, 0});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s2,t1});
            all_texturas.push_back({s1,t2});

    
            // 2º Triângulo
            all_vertices.push_back({x, y, z});
            all_vertices.push_back({x + tamanhoPorDivisao, y, z});
            all_vertices.push_back({x + tamanhoPorDivisao, y, z - tamanhoPorDivisao});
            
            all_normais.push_back({0, 1, 0});
            all_normais.push_back({0, 1, 0});
            all_normais.push_back({0, 1, 0});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s2,t2});
            all_texturas.push_back({s2,t1});


            ///////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////

            
            // Trás
            y = -metadeTamanho;
    
            // 1º Triângulo
            all_vertices.push_back({x, y, z});
            all_vertices.push_back({x, y, z-tamanhoPorDivisao});
            all_vertices.push_back({x + tamanhoPorDivisao, y, z - tamanhoPorDivisao});

            all_normais.push_back({0, -1, 0});
            all_normais.push_back({0, -1, 0});
            all_normais.push_back({0, -1, 0});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s1,t2});
            all_texturas.push_back({s2,t2});
    
            
            // 2º Triângulo
            all_vertices.push_back({x, y, z});
            all_vertices.push_back({x + tamanhoPorDivisao, y, z - tamanhoPorDivisao});
            all_vertices.push_back({x + tamanhoPorDivisao, y, z});

            all_normais.push_back({0, -1, 0});
            all_normais.push_back({0, -1, 0});
            all_normais.push_back({0, -1, 0});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s2,t2});
            all_texturas.push_back({s2,t1});
    
            ///////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////
            

            // Direita
            x = metadeTamanho;
            y = metadeTamanho - j * tamanhoPorDivisao;
            z = metadeTamanho - i * tamanhoPorDivisao;
            
            
            // 1º Triângulo
            all_vertices.push_back({x, y, z});
            all_vertices.push_back({x, y - tamanhoPorDivisao, z - tamanhoPorDivisao});
            all_vertices.push_back({x, y, z - tamanhoPorDivisao});

            all_normais.push_back({1, 0, 0});
            all_normais.push_back({1, 0, 0});
            all_normais.push_back({1, 0, 0});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s2,t2});
            all_texturas.push_back({s2,t1});


            // 2º Triângulo
            all_vertices.push_back({x, y, z});
            all_vertices.push_back({x, y - tamanhoPorDivisao, z});
            all_vertices.push_back({x, y - tamanhoPorDivisao, z - tamanhoPorDivisao});

            all_normais.push_back({1, 0, 0});
            all_normais.push_back({1, 0, 0});
            all_normais.push_back({1, 0, 0});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s2,t2});
            all_texturas.push_back({s1,t2});
    
    
            ///////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////
            

            // Esquerda
            x = -metadeTamanho;
    
            // 1º Triângulo
            all_vertices.push_back({x, y, z});
            all_vertices.push_back({x, y, z - tamanhoPorDivisao});
            all_vertices.push_back({x, y - tamanhoPorDivisao, z - tamanhoPorDivisao});

            all_normais.push_back({-1, 0, 0});
            all_normais.push_back({-1, 0, 0});
            all_normais.push_back({-1, 0, 0});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s2,t1});
            all_texturas.push_back({s1,t2});

            // 2º Triângulo
            all_vertices.push_back({x, y, z});
            all_vertices.push_back({x, y - tamanhoPorDivisao, z - tamanhoPorDivisao});
            all_vertices.push_back({x, y - tamanhoPorDivisao, z});

            all_normais.push_back({-1, 0, 0});
            all_normais.push_back({-1, 0, 0});
            all_normais.push_back({-1, 0, 0});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s1,t2});
            all_texturas.push_back({s2,t2});
    
            ///////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////

        
            // Cima
            x = metadeTamanho - i * tamanhoPorDivisao;
            y = metadeTamanho - j * tamanhoPorDivisao;
            z = metadeTamanho;

            // 1º Triângulo
            all_vertices.push_back({x, y, z});
            all_vertices.push_back({x - tamanhoPorDivisao, y - tamanhoPorDivisao, z});
            all_vertices.push_back({x, y - tamanhoPorDivisao, z});

            all_normais.push_back({0, 0, 1});
            all_normais.push_back({0, 0, 1});
            all_normais.push_back({0, 0, 1});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s2,t2});
            all_texturas.push_back({s1,t2});

            // 2º Triângulo
            all_vertices.push_back({x, y, z}); 
            all_vertices.push_back({x - tamanhoPorDivisao, y, z});
            all_vertices.push_back({x - tamanhoPorDivisao, y - tamanhoPorDivisao, z});

            all_normais.push_back({0, 0, 1});
            all_normais.push_back({0, 0, 1});
            all_normais.push_back({0, 0, 1});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s2,t1});
            all_texturas.push_back({s2,t2});
    
            ///////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////


            // Baixo
            z = -metadeTamanho;
            
            // 1º Triângulo
            all_vertices.push_back({x, y, z});
            all_vertices.push_back({x, y - tamanhoPorDivisao, z});
            all_vertices.push_back({x - tamanhoPorDivisao, y - tamanhoPorDivisao, z});

            all_normais.push_back({0, 0, -1});
            all_normais.push_back({0, 0, -1});
            all_normais.push_back({0, 0, -1});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s1,t2});
            all_texturas.push_back({s2,t2});

            // 2º Triângulo
            all_vertices.push_back({x, y, z}); 
            all_vertices.push_back({x - tamanhoPorDivisao, y - tamanhoPorDivisao, z});
            all_vertices.push_back({x - tamanhoPorDivisao, y, z});

            all_normais.push_back({0, 0, -1});
            all_normais.push_back({0, 0, -1});
            all_normais.push_back({0, 0, -1});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s2,t2});
            all_texturas.push_back({s2,t1});
            
        }
    }

    writeInFileList(all_vertices, all_normais, all_texturas, "../3dFiles/"+fileName);
}



void generateSphere(float radius, int slices, int stacks, std::string fileName) {

    std::list<Vertices> all_vertices;
    std::list<Vertices> all_normais;
    std::list<Vertices> all_texturas;

    float pi = M_PI;
    float alpha, alpha2, beta, beta2;
    float px, py, pz;


    for (int i = 0; i < stacks; i++) {
        beta = pi / 2 - i * pi / stacks;
        beta2 = pi / 2 - (i + 1) * pi / stacks;

        for (int j = 0; j < slices; j++) {
            alpha = 2 * pi * j / slices;
            alpha2 =  2 * pi * (j + 1) / slices;

            px = radius * cos(beta) * sin(alpha);
            py = radius * sin(beta);
            pz = radius * cos(beta) * cos(alpha);

            
            float s1 = static_cast<float>(j) / slices;
            float s2 = static_cast<float>(j + 1) / slices;
            float t1 = 1.0f - static_cast<float>(i) / stacks;
            float t2 = 1.0f - static_cast<float>(i + 1) / stacks;

            // 1º Triângulo
            all_vertices.push_back({px, py, pz});
            all_vertices.push_back({radius * cos(beta2) * sin(alpha), radius * sin(beta2), radius * cos(beta2) * cos(alpha)});
            all_vertices.push_back({radius * cos(beta2) * sin(alpha2), radius * sin(beta2), radius * cos(beta2) * cos(alpha2)});

            all_normais.push_back({  cos(beta) * sin(alpha),   sin(beta),  cos(beta) * cos(alpha)});
            all_normais.push_back({  cos(beta2) * sin(alpha),  sin(beta2),  cos(beta2) * cos(alpha)});
            all_normais.push_back({  cos(beta2) * sin(alpha2),  sin(beta2),  cos(beta2) * cos(alpha2)});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s1,t2});
            all_texturas.push_back({s2,t2});


            // 2º Triângulo
            all_vertices.push_back({px, py, pz});
            all_vertices.push_back({radius * cos(beta2) * sin(alpha2), radius * sin(beta2), radius * cos(beta2) * cos(alpha2)});
            all_vertices.push_back({radius * cos(beta) * sin(alpha2), py, radius * cos(beta) * cos(alpha2)});

            all_normais.push_back({  cos(beta) * sin(alpha),  sin(beta),  cos(beta) * cos(alpha)});
            all_normais.push_back({  cos(beta2) * sin(alpha2),  sin(beta2),  cos(beta2) * cos(alpha2)});
            all_normais.push_back({  cos(beta) * sin(alpha2),  sin(beta),  cos(beta) * cos(alpha2)});

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s2,t2});
            all_texturas.push_back({s2,t1});
        }
    }

    writeInFileList(all_vertices, all_normais, all_texturas, "../3dFiles/" + fileName);
}



void generateCone(float radius, float height, int slices, int stacks, std::string fileName){

    std::list<Vertices> all_vertices;
    std::list<Vertices> all_normais;
    std::list<Vertices> all_texturas;

    float pi = M_PI;
    float x, y = 0, z;   
    
    // Base
    for(int i = 0; i < slices; i++){
        float anguloPorVertice = 2 * pi * i / slices;
        float anguloPorVertice2 = 2 * pi * (i + 1) / slices;

        x = radius * cos(anguloPorVertice);
        z = radius * sin(anguloPorVertice);

        all_vertices.push_back({x, y, z});
        all_vertices.push_back({radius * cos(anguloPorVertice2), y, radius * sin(anguloPorVertice2)});
        all_vertices.push_back({0, y, 0});

        all_normais.push_back({0, -1, 0});
        all_normais.push_back({0, -1, 0});
        all_normais.push_back({0, -1, 0});

        all_texturas.push_back({0.5f + 0.5f * cos(anguloPorVertice), 0.5f + 0.5f * sin(anguloPorVertice)});
        all_texturas.push_back({0.5f + 0.5f * cos(anguloPorVertice2), 0.5f + 0.5f * sin(anguloPorVertice2)});
        all_texturas.push_back({0.5f, 0.5f});
    }

    // Lateral
    for (int i = 0; i < stacks; i++) {
        float y1 = i * height / stacks;
        float y2 = (i + 1) * height / stacks;

        float radius1 = radius * (1 - y1 / height);
        float radius2 = radius * (1 - y2 / height);

        for (int j = 0; j < slices; j++) {
            float anguloPorVertice = 2 * pi * j / slices;
            float anguloPorVertice2 = 2 * pi * (j + 1) / slices;

            float x1 = radius1 * cos(anguloPorVertice);
            float x2 = radius1 * cos(anguloPorVertice2);
            float z1 = radius1 * sin(anguloPorVertice);
            float z2 = radius1 * sin(anguloPorVertice2);

            float x3 = radius2 * cos(anguloPorVertice);
            float x4 = radius2 * cos(anguloPorVertice2);
            float z3 = radius2 * sin(anguloPorVertice);
            float z4 = radius2 * sin(anguloPorVertice2);

            float s1 = static_cast<float>(j) / slices;
            float s2 = static_cast<float>(j + 1) / slices;
            float t1 = static_cast<float>(i) / stacks;
            float t2 = static_cast<float>(i + 1) / stacks;


            // 1º Triângulo
            Vertices v1 = {x1, y1, z1};
            Vertices v2 = {x3, y2, z3};
            Vertices v3 = {x2, y1, z2};

            all_vertices.push_back(v1);
            all_vertices.push_back(v2);
            all_vertices.push_back(v3);

            Vertices normal1 = cross({x3 - x1, y2 - y1, z3 - z1}, {x2 - x1, y1 - y1, z2 - z1});
            normalize(normal1);

            all_normais.push_back(normal1);
            all_normais.push_back(normal1);
            all_normais.push_back(normal1);

            all_texturas.push_back({s1,t1});
            all_texturas.push_back({s1,t2});
            all_texturas.push_back({s2,t1});


            // 2º Triângulo
            Vertices v4 = {x2, y1, z2};
            Vertices v5 = {x3, y2, z3};
            Vertices v6 = {x4, y2, z4};

            all_vertices.push_back(v4);
            all_vertices.push_back(v5);
            all_vertices.push_back(v6);

            Vertices normal2 = cross({v5.x - v4.x, v5.y - v4.y, v5.z - v4.z}, {v6.x - v4.x, v6.y - v4.y, v6.z - v4.z});
            normalize(normal2);

            all_normais.push_back(normal2);
            all_normais.push_back(normal2);
            all_normais.push_back(normal2);

            all_texturas.push_back({s2,t1});
            all_texturas.push_back({s1,t2});
            all_texturas.push_back({s2,t2});
        } 
    }

    writeInFileList(all_vertices, all_normais, all_texturas, "../3dFiles/" + fileName);
}
            


Vertices bezierCoeficientCalculator(std::vector<Vertices> patchVertices, float u, float v) {
    int grau = 3;
    Vertices vertice = {0, 0, 0};

    for (int i = 0; i <= grau; i++) {
        for (int j = 0; j <= grau; j++) {
            float resultado = std::tgamma(grau + 1) / (std::tgamma(i + 1) * std::tgamma(grau - i + 1)) * std::tgamma(grau + 1) / (std::tgamma(j + 1) * std::tgamma(grau - j + 1)) * std::pow(u, i) *std::pow(1 - u, grau - i) * std::pow(v, j) * std::pow(1 - v, grau - j);

            vertice.x += resultado * patchVertices[i * (grau+1) + j].x;
            vertice.y += resultado * patchVertices[i * (grau+1) + j].y;
            vertice.z += resultado * patchVertices[i * (grau+1) + j].z;
        }
    }

    return vertice;
}

void generateBezierPatch(std::string fileToFetch, int tessellationLevel, std::string fileName){
    fileToFetch = "../3dFiles/" + fileToFetch;
    std::vector<std::vector<Vertices>> verticeMatches = readBezierFile(fileToFetch);

    std::vector<Vertices> all_vertices;
    std::vector<Vertices> all_normais;
    std::vector<Vertices> all_texturas;

    float divisoes = 1.0 / tessellationLevel;
    float u, v, uNext, vNext;

    int cont = 0;
    for (std::vector<Vertices> patchVertices : verticeMatches){

        for (u=0; u < 1.0; u += divisoes){
            uNext = u + divisoes;
            for (v=0; v < 1.0; v += divisoes){
                cont += 1;
                vNext = v + divisoes;

                float s1 = u;
                float s2 = uNext;
                float t1 = v;
                float t2 = vNext;


                all_vertices.push_back(bezierCoeficientCalculator(patchVertices,u,v));
                all_vertices.push_back(bezierCoeficientCalculator(patchVertices,u,vNext));
                all_vertices.push_back(bezierCoeficientCalculator(patchVertices,uNext,v));

                all_normais.push_back(bezierCoeficientCalculator(patchVertices,u,v));
                all_normais.push_back(bezierCoeficientCalculator(patchVertices,u,vNext));
                all_normais.push_back(bezierCoeficientCalculator(patchVertices,uNext,v));

                all_texturas.push_back({s1, t1});
                all_texturas.push_back({s1, t2});
                all_texturas.push_back({s2, t1});


                all_vertices.push_back(bezierCoeficientCalculator(patchVertices,uNext,v));
                all_vertices.push_back(bezierCoeficientCalculator(patchVertices,u,vNext));
                all_vertices.push_back(bezierCoeficientCalculator(patchVertices,uNext,vNext));  
                
                all_normais.push_back(bezierCoeficientCalculator(patchVertices,uNext,v));
                all_normais.push_back(bezierCoeficientCalculator(patchVertices,u,vNext));
                all_normais.push_back(bezierCoeficientCalculator(patchVertices,uNext,vNext));

                all_texturas.push_back({s2, t1});
                all_texturas.push_back({s1, t2});
                all_texturas.push_back({s2, t2});  
            }
        }
    }
    writeInFileVector(all_vertices, all_normais, all_texturas, "../3dFiles/"+ fileName);
}


std::vector<std::vector<Vertices>> readBezierFile(std::string fileToFetch){
    std::ifstream file(fileToFetch, std::ios::in);

    if (!file.is_open()) {
        std::cerr << "File " << fileToFetch << " doesn't exist!" << std::endl;
        return {};
    }

    std::vector<std::vector<float>> allPatches;
    std::string line;

    getline(file, line);
    int numberOfPatches = std::stoi(line);

    for (int i = 0; i < numberOfPatches; i++) {
        getline(file, line);
    
        std::stringstream ss(line);
        std::string element;

        std::vector<float> axis;
        while (getline(ss, element, ',')){
            axis.push_back(stof(element));
        }
        allPatches.push_back(axis);
        axis.clear();
    }

    getline(file, line);
    int numberOfControlPoints = std::stoi(line);


    std::vector<Vertices> allVertices;

    for (int i = 0; i < numberOfControlPoints; i++) {
        getline(file, line);
    
        std::stringstream ss(line);
        std::string element;

        Vertices vertex;
        std::vector<float> axis;
        while (getline(ss, element, ',')){
            axis.push_back(stof(element));
        }
        vertex.x = axis[0];
        vertex.y = axis[1];
        vertex.z = axis[2];
        allVertices.push_back(vertex);
        axis.clear();
    }



    std::vector<std::vector<Vertices>> verticeMatches;

    for (int i = 0; i < numberOfPatches; i++) {
        std::vector<Vertices> vertices;
        for (int j = 0; j < allPatches[i].size(); j++) {
            vertices.push_back(allVertices[allPatches[i][j]]);
        }
        verticeMatches.push_back(vertices);
    }

    file.close();
    return verticeMatches;    

}



void writeInFileList(std::list<Vertices> all_vertices, std::list<Vertices> all_normais , std::list<Vertices> all_texturas, std::string fileName){

    std::ofstream file (fileName);
    int size = all_vertices.size();

    file << size << std::endl;

    for (const auto& element : all_vertices) {
        file << element.x << ",";
        file << element.y << ",";
        file << element.z;
        file << std::endl;
    }

    for (const auto& element : all_normais) {
        file << element.x << ",";
        file << element.y << ",";
        file << element.z;
        file << std::endl;
    }

    for (const auto& element : all_texturas) {
        file << element.x << ",";
        file << element.y << ",";
        file << element.z;
        file << std::endl;
    }

    file.close();
}

void writeInFileVector(std::vector<Vertices> all_vertices, std::vector<Vertices> all_normais, std::vector<Vertices> all_texturas, std::string fileName){
    

    std::ofstream file (fileName);
    int size = all_vertices.size();

    file << size << std::endl;

    for (const auto& element : all_vertices) {
        file << element.x << ",";
        file << element.y << ",";
        file << element.z;
        file << std::endl;
    }

    for (const auto& element : all_normais) {
        file << element.x << ",";
        file << element.y << ",";
        file << element.z;
        file << std::endl;
    }

    for (const auto& element : all_texturas) {
        file << element.x << ",";
        file << element.y << ",";
        file << element.z;
        file << std::endl;
    }

    file.close();
}



Vertices cross(Vertices a, Vertices b) {
    Vertices res;

    res.x = (a.y * b.z) - (a.z * b.y);
    res.y = (a.z * b.x) - (a.x * b.z);
    res.z = (a.x * b.y) - (a.y * b.x);

    return res;
}

// Normaliza o vetor ou seja faz com que o vetor tenha tamanho 1 
Vertices normalize(Vertices a) {
    Vertices novoVetor;

    float l = sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));

    if (l == 0.0f) {
        novoVetor.x = 0.0f;
        novoVetor.y = 0.0f;
        novoVetor.z = 0.0f;
    } else {
        novoVetor.x = a.x / l;
        novoVetor.y = a.y / l;
        novoVetor.z = a.z / l;
    }

    return novoVetor;
}