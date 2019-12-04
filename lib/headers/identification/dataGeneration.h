#ifndef DATAGENERATION_H
#define DATAGENERATION_H

#ifndef modeltest
    #include "headers/primitive/matrix.h"
#else
    #include "../../Impressora3d/headers/primitive/matrix.h"
    #include "../../Impressora3d/headers/motor/motor.h"
#endif

//A ideia eh implementar apenas algoritmos recursivos de modelagem e otimizacao

namespace IdentificationHandler {
    //Geração de Sinal de excitação para obtenção de modelo fenomenológico
    template <typename Type>
    LinAlg::Matrix<Type> prs(gpio_num_t input, gpio_num_t output);
    template <typename Type>
    LinAlg::Matrix<Type> prbs(gpio_num_t input, gpio_num_t output);
    template <typename Type>
    LinAlg::Matrix<Type> relayExcitation(gpio_num_t input, gpio_num_t output);
    template <typename Type>
    LinAlg::Matrix<Type> sin(gpio_num_t input, gpio_num_t output);
    template <typename Type>
    LinAlg::Matrix<Type> square(gpio_num_t input, gpio_num_t output);

    //Extração de características do sistema
    template <typename Type>
    LinAlg::Matrix<Type> covariance();
    template <typename Type>
    LinAlg::Matrix<Type> correlation();
    template <typename Type>
    LinAlg::Matrix<Type> crosscorrelation();
    template <typename Type>
    LinAlg::Matrix<Type> variance();
    template <typename Type>
    LinAlg::Matrix<Type> standardDeviation();

    template <typename Type>
    LinAlg::Matrix<Type> smallestTimeConstant(gpio_num_t input, gpio_num_t output);
    template <typename Type>
    LinAlg::Matrix<Type> heuristicSampleTimeSelection(gpio_num_t input, gpio_num_t output);
}

#ifndef modeltest
    #include "src/model/model.hpp"
#else
    #include "../../Impressora3d/src/model/model.hpp"
#endif

#endif // MODEL_H
