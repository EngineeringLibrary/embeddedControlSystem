#ifndef RECURSIVELEASTSQUARE_H
#define RECURSIVELEASTSQUARE_H

#ifndef modeltest
    #include "headers/optimizer/optmizer.h"
#else
    #include "../lib/headers/optimizer/optmizer.h"
#endif

namespace OptimizationHandler {
    template <class Type>
    class RecursiveLeastSquare: public Optmizer<Type>
    {
    public:
        RecursiveLeastSquare(ModelHandler::Model<Type> *model,
                             Type p0 = 1e5,
                             Type lambda = 1);

        void optimize(LinAlg::Matrix<Type> input, LinAlg::Matrix<Type> output);
        std::string print(){std::string str = "O modelo otimizado eh: \n"; str += this->model->print();}

    private:
        bool firstTimeFlag;
        Type p0, lambda;
        LinAlg::Matrix<Type> P, K, E, Input, Output;
    };
}

#ifndef modeltest
    #include "src/optimizer/linear_optimizer/unconstrained_optimizer/recursiveLeastSquare.hpp"
#else
    #include "../lib/src/optimizer/linear_optimizer/unconstrained_optimizer/recursiveleastsquare.hpp"
#endif

#endif // RECURSIVELEASTSQUARE_H
