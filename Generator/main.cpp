#include "../Includes/shapesGenerator.hpp"

int main(int argc, char *argv[]) {

    if(argc < 2){
        std::cout << "Número de argumentos inválido" << std::endl;
        return 1;
    }

    std::string shape;
    shape = argv[1];
    
    if(shape == "sphere"){

        if(argc != 6){
            std::cout << "Número de argumentos inválidos para uma esfera" << std::endl;
            return 1;
        }

        float radius;
        int slices, stacks;
        std::string fileToStore;

        try{
            radius = std::stof(argv[2]);
            slices = std::stoi(argv[3]);
            stacks = std::stoi(argv[4]);
            fileToStore = argv[5];
        }
        catch(std::invalid_argument){
            std::cout << "Argumentos de tipo inválido" << std::endl;
            return 1;
        }

        generateSphere(radius, slices, stacks, fileToStore);

    }
    else if(shape == "box"){

        if(argc != 5){
            std::cout << "Número de argumentos inválidos para uma caixa" << std::endl;
            return 1;
        }

        float length;
        int divisions;
        std::string fileToStore;

        try{
            length = std::stof(argv[2]);
            divisions = std::stoi(argv[3]);
            fileToStore = argv[4];
        }
        catch(std::invalid_argument){
            std::cout << "Argumentos de tipo inválido" << std::endl;
            return 1;
        }
        
        generateBox(length, divisions, fileToStore); 

    }
    else if(shape == "cone"){

        if(argc != 7){
            std::cout << "Número de argumentos inválidos para um cone" << std::endl;
            return 1;
        }

        float radius, height;
        int slices, stacks;
        std::string fileToStore;

        try{
            radius = std::stof(argv[2]);
            height = std::stof(argv[3]);
            slices = std::stoi(argv[4]);
            stacks = std::stoi(argv[5]);
            fileToStore = argv[6];
        }
        catch(std::invalid_argument){
            std::cout << "Argumentos de tipo inválido" << std::endl;
            return 1;
        }

        generateCone(radius, height, slices, stacks, fileToStore);

    }
    else if(shape == "plane"){

        if(argc != 5){
            std::cout << "Número de argumentos inválidos para um plano" << std::endl;
            return 1;
        }

        float length;
        int divisions;
        std::string fileToStore;

        try{
            length = std::stof(argv[2]);
            divisions = std::stoi(argv[3]);
            fileToStore = argv[4];
        }
        catch(std::invalid_argument){
            std::cout << "Argumentos de tipo inválido" << std::endl;
            return 1;
        }

        generatePlane(length, divisions, fileToStore);

    }
    else if(shape == "patch"){

        if(argc != 5){
            std::cout << "Número de argumentos inválidos para um patch" << std::endl;
            return 1;
        }

        std::string fileToFetch;
        int tessellationLevel;
        std::string fileToStore;

        try{
            fileToFetch = argv[2];
            tessellationLevel = std::stoi(argv[3]);
            fileToStore = argv[4];
        }
        catch(std::invalid_argument){
            std::cout << "Argumentos de tipo inválido" << std::endl;
            return 1;
        }

        generateBezierPatch(fileToFetch, tessellationLevel, fileToStore);

    }

    else {
        std::cout << "Comando inválido" << std::endl;
    }

    return 0;
}
