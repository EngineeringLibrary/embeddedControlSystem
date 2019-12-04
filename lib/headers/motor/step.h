#ifndef STEP_H
#define STEP_H
#include "motor.h"

//Numa poss�vel implementa��o concreta do motor de passo, seria interessante ter a op��o de saber os limites do sistema a ser controlado posi��es m�xima e m�nima, velocidade m�xima e m�nima. Ter uma op��o de satura��o seria interessante tamb�m, para a vari�vel a ser controlada;

class step :  public Motor
{
public:
    step ();
    ~step();
    gpio_num_t  getStepPin(){return this->step_pin;}
    gpio_num_t  getDirectionPin(){return this->direction_pin;}
    void setPosition(double position);

private:
    static void	TaskFunc(void*);//seria bom lembrar que quando a task for executada ela deve ser deletada ao final. Caso isso n�o seja feito, ela ficar� reiniciando.

    gpio_num_t step_pin, direction_pin;
    TaskHandle_t	m_pTask;
    
};

#endif // STEP_H
