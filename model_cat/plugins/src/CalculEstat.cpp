
#include "CalculEstat.hpp"

CalculEstat::DerivadesEstat calcularDerivadesEstat(const CalculForces::State& state, const CalculForces::Input& input, const Parametres& parametres, const CalculForces::Fy& fy, const double Fx){
    CalculEstat::DerivadesEstat derivades;
    // Calcul de les derivades de posició i orientació.
    derivades.x_dot = cos(state.yaw) * state.v_x - sin(state.yaw) * state.v_y;
    derivades.y_dot = sin(state.yaw) * state.v_x + cos(state.yaw) * state.v_y;
    derivades.yaw_dot = state.r;

    // Calcul de les derivades de velocitat.
    derivades.v_x_dot = state.r * state.v_y + (1 / parametres.massa) * (Fx - fy.FyFrontTotal * sin(input.steering_angle));
    derivades.v_y_dot = -state.r * state.v_x + (1 / parametres.Iz) * (fy.FyFrontTotal * cos(input.steering_angle) + fy.FyRearTotal);

    // Calcul de les derivades d'acceleració.
    derivades.a_x_dot =0;
    derivades.a_y_dot =0;
    derivades.r_dot = (cos(input.steering_angle)*fy.FyFrontTotal*parametres.lever_arm_length_+sin(input.steering_angle)*(fy.FyDretaFront-fy.FyEsquerreFront)*0.5*parametres.bf-fy.FyRearTotal*parametres.lever_arm_length_)/parametres.Iz;
    return derivades;
}

CalculForces::State calcularSeguentDinamica(const CalculForces::State& state, const CalculEstat::DerivadesEstat& derivades, const Parametres& parametres){
    CalculForces::State seguentdinamica;
    seguentdinamica.x = state.x + derivades.x_dot * parametres.dt;
    seguentdinamica.y = state.y + derivades.y_dot * parametres.dt;
    seguentdinamica.yaw = state.yaw + derivades.yaw_dot * parametres.dt;
    seguentdinamica.v_x = state.v_x + derivades.v_x_dot * parametres.dt;
    seguentdinamica.v_y = state.v_y + derivades.v_y_dot * parametres.dt;
    seguentdinamica.a_x = state.a_x;
    seguentdinamica.a_y = state.a_y;
    seguentdinamica.r = state.r + derivades.r_dot * parametres.dt;
    return seguentdinamica;
}

CalculForces::State calculCorreccioEstat(const CalculForces::State& seguentdinamica, const Parametres& parametres, double Fx, const CalculForces::State& state, const CalculForces::Input& input){
    CalculForces::State NouEstat;
    //Correcció de la velocitat longitudinal (vx):
        //Suavitat
        const double v = std::hypot(seguentdinamica.v_x, seguentdinamica.v_y);
        const double v_suau = 0.5 * (v - 1.5);
        const double suavitat = std::fmax(std::fmin(1.0,v_suau),0.0);
        //Correció de l'acceleració longitudinal (v_x_dot):
        const double v_x_dot = Fx / (parametres.Iz + parametres.massalong_transmissio);
        //Correcció total vx:
    NouEstat.v_x = suavitat * seguentdinamica.v_x + (1.0-suavitat)*(state.v_x + v_x_dot * parametres.dt);
    //Correcció de la velocitat lateral (vy):
        const double v_y = std::tan(input.steering_angle) * NouEstat.v_x * parametres.distCG_EixR / parametres.longEixF;
    NouEstat.v_y = suavitat * seguentdinamica.v_y + (1.0-suavitat) * v_y;
    //Correcció de la velocitat angular (r):
        const double r = std::tan(input.steering_angle) * NouEstat.v_x / parametres.longEixF;
    NouEstat.r = suavitat * seguentdinamica.r + (1.0-suavitat) * r; 
    //Els altres estats no es corregeixen:
    NouEstat.x = seguentdinamica.x;
    NouEstat.y = seguentdinamica.y;
    NouEstat.yaw = seguentdinamica.yaw;
    NouEstat.a_x = seguentdinamica.a_x;
    NouEstat.a_y = seguentdinamica.a_y;
    return NouEstat;
}





