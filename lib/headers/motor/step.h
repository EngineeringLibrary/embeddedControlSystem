#ifndef STEP_H
#define STEP_H
#include "motor.h"

//Numa possível implementação concreta do motor de passo, seria interessante ter a opção de saber os limites do sistema a ser controlado posições máxima e mínima, velocidade máxima e mínima. Ter uma opção de saturação seria interessante também, para a variável a ser controlada;

class step :  public Motor
{
public:
    step ();
    ~step();
    gpio_num_t  getStepPin(){return this->step_pin;}
    gpio_num_t  getDirectionPin(){return this->direction_pin;}
    void setPosition(double position);

private:
    static void	TaskFunc(void*);//seria bom lembrar que quando a task for executada ela deve ser deletada ao final. Caso isso não seja feito, ela ficará reiniciando.

    gpio_num_t step_pin, direction_pin;
    TaskHandle_t	m_pTask;
    
};

#endif // STEP_H
