#include "../Includes/parser.hpp"

int countElements(const std::string& str) {
    int count = 0;
    for (char c : str) {
        if (c == '/') {
            count += 1;
        }
    }
    return count;
};

std::tuple<std::unordered_map<std::string, std::unordered_map<std::string, std::string>>,std::vector<std::vector<Vertices>>, std::unordered_map<std::string, TextureColor>> xmlParser(const char* fileName) {

    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> engineInfo;
    std::unordered_map<std::string, TextureColor> textureColorinfo;
    std::vector<std::vector<Vertices>> allCatmullPoints;
    tinyxml2::XMLDocument xmlFile;


    if (xmlFile.LoadFile(fileName) != tinyxml2::XML_SUCCESS) {
        return {engineInfo, allCatmullPoints, textureColorinfo};
    }


    tinyxml2::XMLElement* world = xmlFile.FirstChildElement("world");
    tinyxml2::XMLElement* window = world->FirstChildElement("window");

    tinyxml2::XMLElement* camera = world->FirstChildElement("camera");
    tinyxml2::XMLElement* position = camera->FirstChildElement("position");
    tinyxml2::XMLElement* lookAt = camera->FirstChildElement("lookAt");
    tinyxml2::XMLElement* up = camera->FirstChildElement("up");
    tinyxml2::XMLElement* projection = camera->FirstChildElement("projection");

    tinyxml2::XMLElement* lights = world->FirstChildElement("lights");
    tinyxml2::XMLElement* group = world->FirstChildElement("group");

    if(lights){
        tinyxml2::XMLElement* light = lights->FirstChildElement("light");

        while(light != nullptr){

            if(std::strcmp(light->Attribute("type"),"point")==0) {
                if (engineInfo["light"].find(light->Attribute("type")) != engineInfo["light"].end()) {
                    std::string& existingValue = engineInfo["light"][light->Attribute("type")];
                    engineInfo["light"][light->Attribute("type")] = existingValue + "/" + std::string(light->Attribute("posx")) + "," + std::string(light->Attribute("posy")) + "," + std::string(light->Attribute("posz"));
                } else {
                    engineInfo["light"][light->Attribute("type")] = std::string(light->Attribute("posx")) + "," + std::string(light->Attribute("posy")) + "," + std::string(light->Attribute("posz"));
                }
            } 
            else if(std::strcmp(light->Attribute("type"),"directional")==0) {
                if (engineInfo["light"].find(light->Attribute("type")) != engineInfo["light"].end()) {
                    std::string& existingValue = engineInfo["light"][light->Attribute("type")];
                    engineInfo["light"][light->Attribute("type")] = existingValue + "/" + std::string(light->Attribute("dirx")) + "," + std::string(light->Attribute("diry")) + "," + std::string(light->Attribute("dirz"));
                } else {
                    engineInfo["light"][light->Attribute("type")] = std::string(light->Attribute("dirx")) + "," + std::string(light->Attribute("diry")) + "," + std::string(light->Attribute("dirz"));
                }
            }
            else if(std::strcmp(light->Attribute("type"),"spot")==0) {
                 if (engineInfo["light"].find(light->Attribute("type")) != engineInfo["light"].end()) {
                    std::string& existingValue = engineInfo["light"][light->Attribute("type")];
                    engineInfo["light"][light->Attribute("type")] = existingValue + "/" + std::string(light->Attribute("posx")) + "," + std::string(light->Attribute("posy")) + "," + std::string(light->Attribute("posz")) + "," + std::string(light->Attribute("dirx")) + "," + std::string(light->Attribute("diry")) + "," + std::string(light->Attribute("dirz")) + "," + std::string(light->Attribute("cutoff"));
                } else {
                    engineInfo["light"][light->Attribute("type")] = std::string(light->Attribute("posx")) + "," + std::string(light->Attribute("posy")) + "," + std::string(light->Attribute("posz")) + "," + std::string(light->Attribute("dirx")) + "," + std::string(light->Attribute("diry")) + "," + std::string(light->Attribute("dirz")) + "," + std::string(light->Attribute("cutoff"));
                }
            }
            else{
                std::cout << "Unknown light type" << std::endl;
            }

            light = light->NextSiblingElement();
        }
    }

    engineInfo["window"]["width"] = window->Attribute("width");
    engineInfo["window"]["height"] = window->Attribute("height");

    engineInfo["position"]["x"] = position->Attribute("x");
    engineInfo["position"]["y"] = position->Attribute("y");
    engineInfo["position"]["z"] = position->Attribute("z");

    engineInfo["lookAt"]["x"] = lookAt->Attribute("x");
    engineInfo["lookAt"]["y"] = lookAt->Attribute("y");
    engineInfo["lookAt"]["z"] = lookAt->Attribute("z");

    engineInfo["up"]["x"] = up->Attribute("x");
    engineInfo["up"]["y"] = up->Attribute("y");
    engineInfo["up"]["z"] = up->Attribute("z");
    

    if (projection){
        engineInfo["projection"]["fov"] = projection->Attribute("fov");
        engineInfo["projection"]["near"] = projection->Attribute("near");
        engineInfo["projection"]["far"] = projection->Attribute("far");
    }
    else{
        engineInfo["projection"]["fov"] = "60";
        engineInfo["projection"]["near"] = "1";
        engineInfo["projection"]["far"] = "1000";
    }

    

    allGroupsInfo(group, 0, "", &engineInfo, &allCatmullPoints, &textureColorinfo);

    return {engineInfo,allCatmullPoints,textureColorinfo};
}


void allGroupsInfo(tinyxml2::XMLElement* grupo, int cont, std::string transformations, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>* engineInfo, std::vector<std::vector<Vertices>>* allCatmullPoints, std::unordered_map<std::string, TextureColor>* textureColorinfo){
    
    
    while(grupo != nullptr) {

        if (std::strcmp(grupo->Name(), "group") == 0){
            allGroupsInfo(grupo->FirstChildElement(), cont+1, transformations, engineInfo, allCatmullPoints, textureColorinfo);
        }

        else if (std::strcmp(grupo->Name(), "transform") == 0){
            tinyxml2::XMLElement* transform = grupo->FirstChildElement();

            bool translate = false;
            bool rotate = false;
            bool scale = false;

            while(transform != nullptr) {

                if(std::strcmp(transform->Name(), "translate") == 0){
                    if(transform->Attribute("time")!= nullptr) {

                        int cont = 0;
                        tinyxml2::XMLElement* translatePoint = transform->FirstChildElement();
                        while(translatePoint != nullptr) {
                            cont ++;
                            translatePoint = translatePoint->NextSiblingElement();
                        }

                        if (cont < 4){
                            std::cout << "To build the Catmull-Rom curve, there needs to be at least 4 points!";
                        }
                        else{

                            std::string translateTime = std::string(transform->Attribute("time"));
                            std::string align =std::string(transform->Attribute("align"));
                            if (translate == false){
                                int alignInt = align == "true" ? 0 : 1; 
                                int length =  (*allCatmullPoints).size();
                                transformations.append("a"+translateTime+","+std::to_string(alignInt)+","+std::to_string(length)+"!");
                                translate = true;

                                tinyxml2::XMLElement* translatePoint = transform->FirstChildElement();

                                std::vector<Vertices> catmullPoints;

                                while(translatePoint != nullptr) {
                                    
                                    if(std::strcmp(translatePoint->Name(), "point") == 0){
                                        Vertices vertice;
                                        vertice.x = std::stof(translatePoint->Attribute("x"));
                                        vertice.y = std::stof(translatePoint->Attribute("y"));
                                        vertice.z = std::stof(translatePoint->Attribute("z"));
                                        catmullPoints.push_back(vertice);
                                    }  else{
                                        std::cout << "Unknown tag" << std::endl;
                                    }
                                    translatePoint = translatePoint->NextSiblingElement();
                                }
                                (*allCatmullPoints).push_back(catmullPoints);
                            }
                            else{
                                std::cout << "Duplicate translate time in the same group! Ignoring translate " +translateTime+","+align << std::endl;
                            }
                        }
                    } else {
                        std::string translateX = std::string(transform->Attribute("x"));
                        std::string translateY = std::string(transform->Attribute("y"));
                        std::string translateZ = std::string(transform->Attribute("z"));

                        if (translate == false){
                            transformations.append("t"+translateX+","+translateY+","+translateZ+"!");
                            translate = true;
                        }
                        else{
                            std::cout << "Duplicate translate time in the same group! Ignoring translate " +translateX+","+translateY+","+translateZ << std::endl;
                        }
                    }
                } else if(std::strcmp(transform->Name(), "rotate") == 0){
                    if(transform->Attribute("time")!= nullptr) {
                        std::string rotateTime = std::string(transform->Attribute("time"));
                        std::string rotateX = std::string(transform->Attribute("x"));
                        std::string rotateY = std::string(transform->Attribute("y"));
                        std::string rotateZ = std::string(transform->Attribute("z"));

                        if (rotate == false){
                            transformations.append("b"+rotateTime+","+rotateX+","+rotateY+","+rotateZ+"!");
                            rotate = true;
                        } else {
                            std::cout << "Duplicate rotate time in the same group! Ignoring rotate " + rotateTime+","+rotateX+","+rotateY+","+rotateZ << std::endl;
                        }
                    } else {
                        std::string rotateAngle = std::string(transform->Attribute("angle"));
                        std::string rotateX = std::string(transform->Attribute("x"));
                        std::string rotateY = std::string(transform->Attribute("y"));
                        std::string rotateZ = std::string(transform->Attribute("z"));

                        if (rotate == false){
                            transformations.append("r"+rotateAngle+","+rotateX+","+rotateY+","+rotateZ+"!");
                            rotate = true;
                        }
                        else{
                            std::cout << "Duplicate rotate in the same group! Ignoring rotate " + rotateAngle+"," +rotateX+","+rotateY+","+rotateZ << std::endl;
                        }
                    }
                }
                else if(std::strcmp(transform->Name(), "scale") == 0){
                    std::string scaleX = std::string(transform->Attribute("x"));
                    std::string scaleY = std::string(transform->Attribute("y"));
                    std::string scaleZ = std::string(transform->Attribute("z"));

                    if (scale == false){
                        transformations.append("s"+scaleX+","+scaleY+","+scaleZ+"!");
                        scale = true;
                    }
                    else{
                        std::cout << "Duplicate scale in the same group! Ignoring scale " +scaleX+","+scaleY+","+scaleZ << std::endl;
                    }

                    
                }

                else{
                    std::cout << "Unknown tag" << std::endl;
                }

                transform = transform->NextSiblingElement();
            }
        }

        else if (std::strcmp(grupo->Name(), "models") == 0){ 
            tinyxml2::XMLElement* models = grupo->FirstChildElement();
            while(models != nullptr) {
                
                std::string model = std::string(models->Attribute("file"));
                if(transformations == ""){
                    transformations = " ";
                }
                if ((*engineInfo)["model"].find(model) != (*engineInfo)["model"].end()) {
                    std::string& existingValue = (*engineInfo)["model"][model];
                    (*engineInfo)["model"][models->Attribute("file")] = existingValue + "/" + transformations;
                } else {
                    (*engineInfo)["model"][models->Attribute("file")] = transformations;
                }

                
                tinyxml2::XMLElement* filho = models->FirstChildElement();
                int ocorrencias = 0;
                std::string texturaFinal = "";
                TextureColor textureColor;

                if(filho != nullptr){
                    ocorrencias = countElements((*engineInfo)["model"][models->Attribute("file")]);   
                    texturaFinal = std::string(models->Attribute("file")) + "-" + std::to_string(ocorrencias);     
    
                    while(filho != nullptr){
                        if(std::strcmp(filho->Name(),"texture")==0){
                            textureColor.textureName = filho->Attribute("file");
                        } else if(std::strcmp(filho->Name(),"color")==0){
                            tinyxml2::XMLElement* cor = filho->FirstChildElement();
                            while(cor != nullptr){

                                Vertices vertices;
                                if(std::strcmp(cor->Name(),"diffuse")==0 || std::strcmp(cor->Name(),"ambient")==0 || std::strcmp(cor->Name(),"specular")==0 || std::strcmp(cor->Name(),"emissive")==0){
                                    vertices.x = std::stof(cor->Attribute("R"));
                                    vertices.y = std::stof(cor->Attribute("G"));
                                    vertices.z = std::stof(cor->Attribute("B"));

                                    textureColor.cores[cor->Name()] = vertices;
                                } else if(std::strcmp(cor->Name(),"shininess")==0){
                                    vertices.x = std::stof(cor->Attribute("value"));
                                    vertices.y = 0;
                                    vertices.z = 0;

                                    textureColor.cores[cor->Name()] = vertices;
                                } else {
                                    std::cout << "Unknown tag" << std::endl;
                                }

                                cor = cor->NextSiblingElement();
                            }
                        } else {
                            std::cout << "Unknown tag" << std::endl;
                        }
                        filho = filho->NextSiblingElement();
                    }
                    if(textureColor.cores.size() == 0){
                        textureColor.cores["diffuse"] = {200,200,200};
                        textureColor.cores["ambient"] = {50,50,50};
                        textureColor.cores["specular"] = {0,0,0};
                        textureColor.cores["emissive"] = {0,0,0};
                        textureColor.cores["shininess"] = {0,0,0};
                    }
                    (*textureColorinfo)[texturaFinal] = textureColor;
                }
                models = models->NextSiblingElement();
            }
        }   
        else{
            std::cout << "Unknown tag" << std::endl;  
        }
        
        grupo = grupo->NextSiblingElement();
    }
}


std::tuple<std::list<Vertices>, std::list<Vertices>, std::list<Vertices>> files3dParser(const char* fileName){
    std::ifstream file(fileName, std::ios::in);

    if (!file.is_open()) {
        std::cerr << "File " << fileName << " doesn't exist!" << std::endl;
        return {};
    }

    std::list<Vertices> allVertices;
    std::list<Vertices> allNormais;
    std::list<Vertices> allTexturas;
    std::string line;

    getline(file, line);
    int numberOfVertices = std::stoi(line);


    for (int i = 0; i < numberOfVertices; i++) {
        getline(file, line);

        std::stringstream ss(line);
        std::string element;

        std::list<float> axis;
        while (getline(ss, element, ',')){
            axis.push_back(stof(element));
        }

        auto it = axis.begin();
        float x = *it;
        std::advance(it, 1);
        float y = *it;
        std::advance(it, 1);
        float z = *it;

        allVertices.push_back({x, y, z});
        axis.clear();

    }





    for (int i = 0; i < numberOfVertices; i++) {
        getline(file, line);

        std::stringstream ss(line);
        std::string element;

        std::list<float> axis;
        while (getline(ss, element, ',')){
            axis.push_back(stof(element));
        }

        auto it = axis.begin();
        float x = *it;
        std::advance(it, 1);
        float y = *it;
        std::advance(it, 1);
        float z = *it;

        allNormais.push_back({x, y, z});
        axis.clear();

    }

    for (int i = 0; i < numberOfVertices; i++) {
        getline(file, line);

        std::stringstream ss(line);
        std::string element;

        std::list<float> axis;
        while (getline(ss, element, ',')){
            axis.push_back(stof(element));
        }

        auto it = axis.begin();
        float x = *it;
        std::advance(it, 1);
        float y = *it;
        std::advance(it, 1);
        float z = *it;

        allTexturas.push_back({x, y, z});
        axis.clear();
    }

    file.close();
    
    return std::make_tuple(allVertices, allNormais, allTexturas);    

}

std::list<Transformation> allModelsxml(std::unordered_map<std::string, std::unordered_map<std::string, std::string>> engineInfo, std::unordered_map<std::string, TextureColor> textureColorinfo){

    std::list<Transformation> transformationsPerModel;
    Transformation tr;
    
    for (auto& model: engineInfo["model"]){
        
        int contador = 0;
        tr.name = model.first;
        std::string transformation = model.second;

        // Sem transformações
        if (transformation == ""){
            tr.tran = {};
            tr.values = {};
            tr.texture.textureName = "";
            tr.texture.cores = {};
            transformationsPerModel.push_back(tr);
        }
        else{
            
            std::list<std::string> differentModels = divideString(transformation, '/');

            int c = 0;
            for (const auto& str1 : differentModels) {
                c++;
                std::list<std::string> differentTransformations = divideString(str1, '!');
                std::list<char> tran;
                std::vector<float> values;
                for (const auto& str2 : differentTransformations) {
                    std::string v = str2;
                    if(str2 != " "){
                        if(str2[0] == ' '){                             
                            v = str2.substr(1);
                            }
                        tran.push_back(v[0]);
                        std::string tail = v.substr(1);

                        std::list<std::string> differentValues = divideString(tail, ',');
                        for(const auto& str3 : differentValues){
                            values.push_back(std::stof(str3));
                        }
                    }
                }
                tr.tran = tran;
                tr.values = values;
                TextureColor aux = textureColorinfo[model.first + "-" + std::to_string(contador)];
                tr.texture.cores = aux.cores;
                if(aux.cores.empty()){
                    std::unordered_map<std::string,Vertices> aa;
                    aa["diffuse"] = {200,200,200};
                    aa["ambient"] = {50,50,50};
                    aa["specular"] = {0,0,0};
                    aa["emissive"] = {0,0,0};
                    aa["shininess"] = {0,0,0};
                    tr.texture.cores = aa;
                }
                tr.texture.textureName = aux.textureName;
                

                transformationsPerModel.push_back(tr);

                contador +=1;
            }
        }
    }

    return transformationsPerModel;
}


std::list<std::string> divideString (std::string parameters, char divider){
    std::list<std::string> finalList;
    std::stringstream ss(parameters);
    
    std::string element;

    while (getline(ss, element, divider)){
        finalList.push_back(element);
    }
    return finalList;
}


std::vector<Light> populateLights(std::unordered_map<std::string, std::string> lightsInfo){
    std::vector<Light> lights;
    for (const auto& lightInfo : lightsInfo) {
        const std::string& name = lightInfo.first;
        const std::string& positions = lightInfo.second;
        Light luz;
        luz.nome = name;


        if (luz.nome == "point") {
            std::list<std::string> differentModels = divideString(positions, '/');

            for (const auto& str1 : differentModels) {
                std::list<std::string> positionList = divideString(str1, ',');
                auto it = positionList.begin();
                float posX = std::stof(*it++);
                float posY = std::stof(*it++);
                float posZ = std::stof(*it);
                luz.posicao = {posX, posY, posZ, 1};

                lights.push_back(luz);
            }
        } 
        else if (luz.nome == "directional"){
            std::list<std::string> differentModels = divideString(positions, '/');

            for (const auto& str1 : differentModels) {
                std::list<std::string> positionList = divideString(str1, ',');
                auto it = positionList.begin();
                float dirX = std::stof(*it++);
                float dirY = std::stof(*it++);
                float dirZ = std::stof(*it);
                luz.direcao = {dirX, dirY, dirZ, 0};
                
                lights.push_back(luz);
            }
        }
        else if (luz.nome == "spot"){
            std::list<std::string> differentModels = divideString(positions, '/');

            for (const auto& str1 : differentModels) {
            
                std::list<std::string> positionList = divideString(str1, ',');
                auto it = positionList.begin();
                float posX = std::stof(*it++);
                float posY = std::stof(*it++);
                float posZ = std::stof(*it++);
                float dirX = std::stof(*it++);
                float dirY = std::stof(*it++);
                float dirZ = std::stof(*it++);
                float cutoff = std::stof(*it);
                luz.posicao = {posX, posY, posZ, 1};
                luz.direcao = {dirX, dirY, dirZ, 0};
                luz.angulo = cutoff;

                lights.push_back(luz);
            } 
        }
    }
    return lights;
}

