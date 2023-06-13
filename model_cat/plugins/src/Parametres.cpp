#include "Parametres.hpp"
#include <yaml-cpp/yaml.h>
#include <iostream>

Parametres carregarParametresDesdeArxiu(const std::string& nomArxiu) {
    Parametres parametres;

    try {
        // Cargar el archivo YAML
        YAML::Node config = YAML::LoadFile(nomArxiu);

        // Leer los valores de los parámetros y asignarlos a la estructura Parametros
        parametres.gravetat = config["gravetat"].as<double>();
        parametres.massa = config["massa"].as<int>();
        parametres.transfmotorroda = config["transfmotorroda"].as<double>();
        parametres.B = config["B"].as<double>();
        parametres.C = config["C"].as<double>();
        parametres.D = config["D"].as<double>();
        parametres.E = config["E"].as<double>();
        parametres.axle = config["axle"].as<double>();
        parametres.lever_arm_length_ = config["lever_arm_length_"].as<double>();
        parametres.axle_width_ = config["axle_width_"].as<double>();
        parametres.Iz = config["Iz"].as<double>();
        parametres.bf = config["bf"].as<double>();
        parametres.dt = config["dt"].as<double>();
        parametres.massalong_transmissio = config["massalong_transmissio"].as<double>();
        parametres.distCG_EixR = config["distCG_EixR"].as<double>();
        parametres.longEixF = config["longEixF"].as<double>();

        

    } catch (const YAML::Exception& e) {
        // Manejar cualquier error al cargar el archivo YAML
        // Por ejemplo, puedes imprimir un mensaje de error y lanzar una excepción
        std::cerr << "Error al cargar el archivo YAML: " << e.what() << std::endl;
        throw;
    }

    return parametres;
}