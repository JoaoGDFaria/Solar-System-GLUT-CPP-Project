
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include "../Includes/parser.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <IL/il.h>


#define TESSELATION 100 

float pos[3]; 
float deriv[3]; 
float angle = 0;
float px, py, pz, lx, ly, lz, ux, uy, uz, fov, near, far;
std::unordered_map<std::string, std::unordered_map<std::string, std::string>> engineInfo;
std::list<Transformation> allModels;
std::vector<std::vector<Vertices>> allCatmullPoints;
std::unordered_map<std::string, TextureColor> textureColorinfo;
std::list<Vertices> allNormais;
std::list<Vertices> allVertices;
std::list<Vertices> allTexturas;

std::vector<Light> allLights;

std::vector<float> normalB;
std::vector<GLuint> buffersNormal;

std::vector<float> vertexB;
std::vector<GLuint> buffersVertex;
std::vector<GLuint> vertexCounts;

std::vector<float> texturasB;
std::vector<GLuint> buffersTexturas;

GLuint textureID;
std::unordered_map<std::string, GLuint> textureMap;



void changeViewSettings(){
    fov = std::stod(engineInfo["projection"]["fov"]);
    near = std::stod(engineInfo["projection"]["near"]);
    far = std::stod(engineInfo["projection"]["far"]);

    px = std::stod(engineInfo["position"]["x"]);
    py = std::stod(engineInfo["position"]["y"]);
    pz = std::stod(engineInfo["position"]["z"]);

    lx = std::stod(engineInfo["lookAt"]["x"]);
    ly = std::stod(engineInfo["lookAt"]["y"]);
    lz = std::stod(engineInfo["lookAt"]["z"]);

    ux = std::stod(engineInfo["up"]["x"]);
    uy = std::stod(engineInfo["up"]["y"]);
    uz = std::stod(engineInfo["up"]["z"]);
}


void changeSize(int w, int h) {
    if (h == 0)
        h = 1;

    float ratio = w * 1.0f / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(fov, ratio, near, far);
    glMatrixMode(GL_MODELVIEW);
}


void processKeys(unsigned char c, int xx, int yy) {
    angle = fmod(angle, 360.0);
    float radius;
    switch (c) {
        // Rodar para a direita
        case 'd':
            angle -= 5;
            radius = sqrt(px * px + pz * pz); 
            px = cos(angle * M_PI / 180.0) * radius; 
            pz = sin(angle * M_PI / 180.0) * radius;
            lx = 0;
            lz = 0;
            break;
        
        // Rodar para a esquerda
        case 'a':
            angle += 5;
            radius = sqrt(px * px + pz * pz);
            px = cos(angle * M_PI / 180.0) * radius; 
            pz = sin(angle * M_PI / 180.0) * radius;
            lx = 0;
            lz = 0;
            break;

        // Rodar para cima
        case 'x':
            py += 0.1;
            break;
        
        // Rodar para baixo
        case 'z':
            py -= 0.1;
            break;

        // Mover para trás
        case 's':
            px -= sin(angle * M_PI / 180.0) * 3;
            pz -= cos(angle * M_PI / 180.0) * 3;
            lx = 0;
            lz = 0;
            break;

        // Mover para a frente
        case 'w':
            px += sin(angle * M_PI / 180.0) * 3;
            pz += cos(angle * M_PI / 180.0) * 3;
            lx = 0;
            ly = 0;
            lz = 0;
            break;
            
        // Reiniciar posição
        case 'r':
            changeViewSettings();    
            break;
    }

    glutPostRedisplay();
}

void cross(float *a, float *b, float *res) {
    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

void normalize(float *a) {
    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}

void multMatrixVector(float m[4][4], float *v, float *res) {
    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j][k];
        }
    }
}

void buildRotMatrix(float *x, float *y, float *z, float *m) {

    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *deriv) {
    // Matriz Catmull-Rom
    float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
                      { 1.0f, -2.5f,  2.0f, -0.5f},
                      {-0.5f,  0.0f,  0.5f,  0.0f},
                      { 0.0f,  1.0f,  0.0f,  0.0f}};

    for (int i = 0; i < 3; i++) {  // i = x, y, z
        float p[4] = { p0[i], p1[i], p2[i], p3[i] };
        float a[4];
        multMatrixVector(m, p, a);
        pos[i] = powf(t, 3.0) * a[0] + powf(t, 2.0) * a[1] + t * a[2] + a[3];
        deriv[i] = 3 * powf(t, 2.0) * a[0] + 2 * t * a[1] + a[2];
    }
}


void getGlobalCatmullRomPoint(float gt, float *pos, float *deriv,int indexOfCatmullPoints) {
    
    int size = allCatmullPoints[indexOfCatmullPoints].size(); 
    float t = gt * size;
    int index = floor(t);
    t = t - index;

    int indices[4];
    indices[0] = (index + size - 1) % size;
    indices[1] = (indices[0] + 1) % size;
    indices[2] = (indices[1] + 1) % size;
    indices[3] = (indices[2] + 1) % size;

    float p0[3] = { allCatmullPoints[indexOfCatmullPoints][indices[0]].x, allCatmullPoints[indexOfCatmullPoints][indices[0]].y, allCatmullPoints[indexOfCatmullPoints][indices[0]].z };
    float p1[3] = { allCatmullPoints[indexOfCatmullPoints][indices[1]].x, allCatmullPoints[indexOfCatmullPoints][indices[1]].y, allCatmullPoints[indexOfCatmullPoints][indices[1]].z };
    float p2[3] = { allCatmullPoints[indexOfCatmullPoints][indices[2]].x, allCatmullPoints[indexOfCatmullPoints][indices[2]].y, allCatmullPoints[indexOfCatmullPoints][indices[2]].z };
    float p3[3] = { allCatmullPoints[indexOfCatmullPoints][indices[3]].x, allCatmullPoints[indexOfCatmullPoints][indices[3]].y, allCatmullPoints[indexOfCatmullPoints][indices[3]].z };

    getCatmullRomPoint(t, p0, p1, p2, p3, pos, deriv);
}


void renderCatmullRomCurve(int indexOfCatmullPoints) {
    glBegin(GL_LINE_LOOP);
    float t = 0;
    while (t < 1) {
        getGlobalCatmullRomPoint(t, pos, deriv, indexOfCatmullPoints);
        glVertex3f(pos[0], pos[1], pos[2]);


        t += 1.0 / TESSELATION;
    }
    glEnd();
}


void buildVBOs(){
    int cont = 0;
    for (const auto& model : allModels) {
        std::tie(allVertices, allNormais, allTexturas) = files3dParser((std::string("../../3dFiles/") + model.name).c_str());
   

        for (const auto& vertex : allVertices) {
            vertexB.push_back(vertex.x);
            vertexB.push_back(vertex.y);
            vertexB.push_back(vertex.z);
        }
        vertexCounts.push_back(vertexB.size() / 3);
        buffersVertex.resize(cont + 1);

        glGenBuffers(1, &buffersVertex[cont]);
        glBindBuffer(GL_ARRAY_BUFFER, buffersVertex[cont]);
        glBufferData(GL_ARRAY_BUFFER, vertexB.size() * sizeof(float), vertexB.data(), GL_STATIC_DRAW);

        vertexB.clear();

        for (const auto& normal : allNormais) {
            normalB.push_back(normal.x);
            normalB.push_back(normal.y);
            normalB.push_back(normal.z);
        }
        buffersNormal.resize(cont + 1);

        glGenBuffers(1, &buffersNormal[cont]);
        glBindBuffer(GL_ARRAY_BUFFER, buffersNormal[cont]);
        glBufferData(GL_ARRAY_BUFFER, normalB.size() * sizeof(float), normalB.data(), GL_STATIC_DRAW);

        normalB.clear();

        for (const auto& textura : allTexturas) {
            texturasB.push_back(textura.x);
            texturasB.push_back(textura.y);
        }
       
        buffersTexturas.resize(cont + 1);

        glGenBuffers(1, &buffersTexturas[cont]);
        glBindBuffer(GL_ARRAY_BUFFER, buffersTexturas[cont]);
        glBufferData(GL_ARRAY_BUFFER, texturasB.size() * sizeof(float), texturasB.data(), GL_STATIC_DRAW);

        texturasB.clear();

        cont++;
    }
}


int loadTexture(std::string s) {
    unsigned int t,tw,th;
    unsigned char *texData;
    unsigned int texID;
    
    // Iniciar o DevIL
    ilInit();
    
    // Colocar a origem da textura no canto inferior esquerdo
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    
    // Carregar a textura para memória
    ilGenImages(1,&t);
    ilBindImage(t);
    ilLoadImage((ILstring)s.c_str());
    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    
    // Assegurar que a textura se encontra em RGBA (Red, Green, Blue, Alpha) com um byte (0 -255) por componente
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();
    
    // Gerar a textura para a placa gráfica
    glGenTextures(1,&texID);

    glBindTexture(GL_TEXTURE_2D,texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    // Upload dos dados de imagem
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texID;
}

void renderScene(void){
    static float t = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // definição da câmera
    glLoadIdentity();
    gluLookAt(px, py, pz,
              lx, ly, lz,
              ux, uy, uz);


    glDisable(GL_LIGHTING);
    // Desenhar os eixos
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-100.0f, 0.0f, 0.0f);
        glVertex3f(100.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -100.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -100.0f);
        glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
    glEnable(GL_LIGHTING); //DESATIVAR ATE A FASE3
    glColor3f(1.0f,1.0f,1.0f);
    int p = 0;
    for (const auto& light : allLights){
        if(light.nome == "point"){
            float pos[4] = {light.posicao[0], light.posicao[1], light.posicao[2], 1.0};
            glLightfv(GL_LIGHT0 + p, GL_POSITION, pos);
        }

        else if(light.nome == "directional"){
            float dir[4] = {light.direcao[0], light.direcao[1], light.direcao[2], 0.0};
            glLightfv(GL_LIGHT0 + p, GL_POSITION, dir);
        }

        else if(light.nome == "spot"){
            float pos[4] = {light.posicao[0], light.posicao[1], light.posicao[2], 1.0};
            float dir[4] = {light.direcao[0], light.direcao[1], light.direcao[2], 0.0};
            glLightfv(GL_LIGHT0 + p, GL_POSITION, pos);
            glLightfv(GL_LIGHT0 + p, GL_SPOT_DIRECTION, dir);
            glLightf(GL_LIGHT0 + p, GL_SPOT_CUTOFF, light.angulo);
        }
    
        p += 1;
    }

    int i = 0;
    for (const auto& model : allModels) {
        glPushMatrix();
        
        TextureColor tcolor = model.texture;
        std::unordered_map<std::string, Vertices> cores = tcolor.cores;

        Vertices v = cores["diffuse"];
        GLfloat array2[] = { v.x/255, v.y/255, v.z/255, 1};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, array2);
      
        v = cores["ambient"];
        GLfloat array[] = { v.x/255, v.y/255, v.z/255, 1};
        glMaterialfv(GL_FRONT, GL_AMBIENT, array);
     
        v = cores["specular"];
        GLfloat array1[] = { v.x/255, v.y/255, v.z/255, 1};
        glMaterialfv(GL_FRONT, GL_SPECULAR, array1);
      
        v = cores["emissive"];
        GLfloat array3[] = { v.x/255, v.y/255, v.z/255, 1};
        glMaterialfv(GL_FRONT, GL_EMISSION, array3);
        
        v = cores["shininess"];
        glMaterialf(GL_FRONT, GL_SHININESS, v.x);
    
       
        int cont=0;
        for (const auto& tr : model.tran){
            // Translation without curve
            if(tr == 't'){
                float x = model.values[cont++];
                float y = model.values[cont++];
                float z = model.values[cont++];
                glTranslatef(x,y,z);
            }

            // Rotation with angle
            else if(tr == 'r'){
                float angle = model.values[cont++];
                float x = model.values[cont++];
                float y = model.values[cont++];
                float z = model.values[cont++];
                glRotatef(angle,x,y,z);
            }

            // Scale
            else if(tr == 's'){
                float x = model.values[cont++];
                float y = model.values[cont++];
                float z = model.values[cont++];
                glScalef(x,y,z);
            }

            // Translation with curve
            else if (tr == 'a'){
                float time = model.values[cont++];
                float align = model.values[cont++];
                float indexOfCatmullPoints = model.values[cont++];
                
                renderCatmullRomCurve(indexOfCatmullPoints);
                
                if(align == 0){
                    t = glutGet(GLUT_ELAPSED_TIME) / (time*1000);
                    getGlobalCatmullRomPoint(t, pos, deriv,indexOfCatmullPoints);

                    glTranslatef(pos[0], pos[1], pos[2]);

                    float up[3] = {0, 1, 0};
                    float forward[3] = {deriv[0], deriv[1], deriv[2]};
                    normalize(forward);
                    float side[3];
                    cross(up, forward, side);
                    normalize(side);
                    cross(forward, side, up);
                    normalize(up);

                    float b[16];

                    buildRotMatrix(forward, up, side, b);

                    glMultMatrixf(b);
                    
                }
            }

            // Rotation with time
            else if (tr == 'b'){
                float time = model.values[cont++];
                float x = model.values[cont++];
                float y = model.values[cont++];
                float z = model.values[cont++];


                float angle = 360.0 * glutGet(GLUT_ELAPSED_TIME) / (time*1000);
                glRotatef(angle, x, y, z);
            }   
        }

        if(tcolor.textureName.empty()){
            glBindBuffer(GL_ARRAY_BUFFER, buffersVertex[i]);
            glVertexPointer(3, GL_FLOAT, 0, 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, buffersNormal[i]);
            glNormalPointer(GL_FLOAT, 0, 0);


            glDrawArrays(GL_TRIANGLES, 0, vertexCounts[i]);
        } else {
            textureID = textureMap["../../Images/" + tcolor.textureName];
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureID);
            

            glBindBuffer(GL_ARRAY_BUFFER, buffersVertex[i]);
            glVertexPointer(3, GL_FLOAT, 0, 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, buffersNormal[i]);
            glNormalPointer(GL_FLOAT, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, buffersTexturas[i]);
            glTexCoordPointer(2, GL_FLOAT, 0, 0);

            glDrawArrays(GL_TRIANGLES, 0, vertexCounts[i]);

            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
        }
        
        glEnd();
        glPopMatrix();
        i++;
    }
    glutSwapBuffers();
}


int main(int argc, char** argv)
{

    // Ler o caminho do XML
    std::string xmlPath;
    while (true){
        std::cout << "Enter the name of the XML file [inside Config directory]: ";
        std::cin >> xmlPath;
        std::string path = "../../Config/" + xmlPath;
        std::tuple<std::unordered_map<std::string, std::unordered_map<std::string, std::string>>,std::vector<std::vector<Vertices>>, std::unordered_map<std::string, TextureColor>> result = xmlParser(path.c_str());
        engineInfo = std::get<0>(result);
        allCatmullPoints = std::get<1>(result);
        textureColorinfo = std::get<2>(result);
        if (engineInfo.empty()) {
            std::cerr << "XML file " << xmlPath << " does not exist!\n" << std::endl;
        }
        else {
            break;
        }
    }
    allModels = allModelsxml(engineInfo,textureColorinfo);


    // Ler as configurações da câmera
    changeViewSettings();

    // Iniciar o Glut e criar a janela
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);

    int width = std::stoi(engineInfo["window"]["width"]);
    int height = std::stoi(engineInfo["window"]["height"]);

    glutInitWindowSize(width, height);    
    glutCreateWindow("Engine");

    glutKeyboardFunc(processKeys);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT, GL_LINE);  // ATIVAR PARA MOSTRAR PRIMEIRAS 3 FASES
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

    glewInit();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_RESCALE_NORMAL);

    for (const auto& model : allModels) {
        std::string textureFile = "../../Images/" + model.texture.textureName;
        if (!textureFile.empty() && textureMap.find(textureFile) == textureMap.end()) {
            textureID = loadTexture(textureFile);
            textureMap[textureFile] = textureID;
        }
    }

    buildVBOs();


    if(engineInfo.find("light") != engineInfo.end()){
        allLights = populateLights(engineInfo["light"]);
        
        glEnable(GL_LIGHTING); // DESATIVAR PARA MOSTRAR PRIMEIRAS 3 FASES


        float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

        int contador = 0;

        for (const auto& light : allLights) {       
            glEnable(GL_LIGHT0 + contador);
            float white[4] = {1.0,1.0,1.0};
            
            glLightfv(GL_LIGHT0 + contador, GL_DIFFUSE, white);
            glLightfv(GL_LIGHT0 + contador, GL_SPECULAR, white);

            contador += 1;
        }
    }


    // Loop principal do glut
    glutMainLoop();


    return 1;
}
