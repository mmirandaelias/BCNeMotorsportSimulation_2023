#ifndef CALCULESTAT_HPP
#define CALCULESTAT_HPP

#include "CalculForces.hpp"

class CalculEstat {
public:
    struct DerivadesEstat{
        double x_dot;
        double y_dot;
        double yaw_dot;
        double v_x_dot;
        double v_y_dot;
        double a_x_dot;
        double a_y_dot;
        double r_dot;
    };

    DerivadesEstat calcularDerivadesEstat(const CalculForces::State& state, const CalculForces::Input& input, const Parametres& parametres, const CalculForces::Fy& fy, const double Fx);
    CalculForces::State calcularSeguentDinamica(const CalculForces::State& state, const CalculEstat::DerivadesEstat& derivades, const Parametres& parametres);
    CalculForces::State calculCorreccioEstat(const CalculForces::State& seguentdinamica, const Parametres& parametres, double Fx, const CalculForces::State& state, const CalculForces::Input& input);
};

#endif  // CALCULESTAT_HPP