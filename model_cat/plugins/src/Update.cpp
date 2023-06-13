
void Update::Update(CalculForces::State& state, CalculForce::Input& input, const Parametres& parametres)
{
    CalculForces::getNormalForce(const Parametres& parametres);
    CalculForces::getFx(const Input& input, const Parametres& parametres);
    CalculForces::getAlpha(const State& state, const Parametres& parametres, const Input& input, Alpha& alpha);
    CalculForces::getFy(const double Fz, const State& state, const Parametres& parametres, const Input& input, Fy& fy);
    CalculEstat::DerivadesEstat calcularDerivadesEstat(const CalculForces::State& state, const CalculForces::Input& input, const Parametres& parametres, const CalculForces::Fy& fy, const double Fx);
    CalculForces::State calcularSeguentDinamica(const CalculForces::State& state, const CalculEstat::DerivadesEstat& derivades, const Parametres& parametres);
    CalculForces::State calculCorreccioEstat(const CalculForces::State& seguentdinamica, const Parametres& parametres, double Fx, const CalculForces::State& state, const CalculForces::Input& input);
};

