#ifndef PARAMETRES_HPP
#define PARAMETRES_HPP

#include <string>

struct Parametres {
    double gravetat;
    int massa;
    double transfmotorroda;
    double B;
    double C;
    double D;
    double E;
    double axle;
    double lever_arm_length_;
    double axle_width_;
    double Iz;
    double bf;
    double dt;
    double massalong_transmissio;
    double distCG_EixR;
    double longEixF;


};

Parametres carregarParametresDesdeArxiu(const std::string& nomArxiu);

#endif  // PARAMETRES_HPP
