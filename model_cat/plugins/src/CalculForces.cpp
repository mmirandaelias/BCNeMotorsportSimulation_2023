#include "CalculForces.hpp"

// Implementación de la función getNormalForce()
double CalculForces::getNormalForce(const Parametres& parametres) {
    const double Fz = parametres.gravetat * parametres.massa;
    return Fz;
}

// Implementación de la función getFx()
double CalculForces::getFx(const Input& input, const Parametres& parametres) {
    const double Fx = input.dc * parametres.transfmotorroda;
    return Fx;
}


// Implementación de la función getAlpha()
void CalculForces::getAlpha(const State& state, const Parametres& parametres, const Input& input, Alpha& alpha) {
    alpha.alphaEsquerreFront = std::atan((state.v_y + parametres.axle * parametres.lever_arm_length_ * state.r) / (state.v_x - 0.5 * parametres.axle_width_ * state.r)) - input.steering_angle;
    alpha.alphaDretaFront = std::atan((state.v_y + parametres.axle * parametres.lever_arm_length_ * state.r) / (state.v_x + 0.5 * parametres.axle_width_ * state.r)) - input.steering_angle;
    alpha.alphaEsquerreRear = std::atan((state.v_y - parametres.axle * parametres.lever_arm_length_ * state.r) / (state.v_x - 0.5 * parametres.axle_width_ * state.r));
    alpha.alphaDretaRear = std::atan((state.v_y - parametres.axle * parametres.lever_arm_length_ * state.r) / (state.v_x + 0.5 * parametres.axle_width_ * state.r));
}

// Implementación de la función getFy()
void CalculForces::getFy(const double Fz, const State& state, const Parametres& parametres, const Input& input, Fy& fy) {
    Alpha alpha;
    getAlpha(state, parametres, input, alpha);
    const double B = parametres.B;
    const double C = parametres.C;
    const double D = parametres.D;
    const double E = parametres.E;

    const double mu_yEsquerreFront = D * std::sin(C * std::atan(B * (1.0 - E) * alpha.alphaEsquerreFront + E * std::atan(B * alpha.alphaEsquerreFront)));
    const double mu_yDretaFront = D * std::sin(C * std::atan(B * (1.0 - E) * alpha.alphaDretaFront + E * std::atan(B * alpha.alphaDretaFront)));
    const double mu_yEsquerreRear = D * std::sin(C * std::atan(B * (1.0 - E) * alpha.alphaEsquerreRear + E * std::atan(B * alpha.alphaEsquerreRear)));
    const double mu_yDretaRear = D * std::sin(C * std::atan(B * (1.0 - E) * alpha.alphaDretaRear + E * std::atan(B * alpha.alphaDretaRear)));

    fy.FyEsquerreFront = Fz * mu_yEsquerreFront;
    fy.FyDretaFront = Fz * mu_yDretaFront;
    fy.FyEsquerreRear = Fz * mu_yEsquerreRear;
    fy.FyDretaRear = Fz * mu_yDretaRear;
    fy.FyFrontTotal = fy.FyEsquerreFront + fy.FyDretaFront;
    fy.FyRearTotal = fy.FyEsquerreRear + fy.FyDretaRear;
}