
#ifndef CALCULFORCES_HPP
#define CALCULFORCES_HPP

#include <cmath>
#include "Parametres.hpp"

class CalculForces {
public:
struct Input {
    double dc;
    double steering_angle;
    // Agrega aquí otros miembros necesarios para Input
};

// Definición de la estructura State
struct State {
    double x;
    double y;
    double yaw;
    double v_x;
    double v_y;
    double r;
    double a_x;
    double a_y;
    // Agrega aquí otros miembros necesarios para State
};

struct Alpha {
    double alphaEsquerreFront;
    double alphaDretaFront;
    double alphaEsquerreRear;
    double alphaDretaRear;
};

struct Fy {
    double FyEsquerreFront;
    double FyDretaFront;
    double FyEsquerreRear;
    double FyDretaRear;
    double FyFrontTotal;
    double FyRearTotal;
};


public:
    double getNormalForce(const Parametres& parametres);
    double getFx(const Input& input, const Parametres& parametres);
    void getAlpha(const State& state, const Parametres& parametres, const Input& input, Alpha& alpha);
    void getFy(const double Fz, const State& state, const Parametres& parametres, const Input& input, Fy& fy);

};


#endif  // CALCULFORCES_HPP
