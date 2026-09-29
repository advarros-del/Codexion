#include "codexion.h"
void    cosa(t_coder *coder)
{
    int id;

    id = take_id(coder);
    if (id - 1 < 0)
    {   
        if (take_dong(coder->r_don, coder) == 2)
            go_to_sleep(coder, 0, coder->r_don);
        if (take_dong(coder->l_don, coder) == 2)
            go_to_sleep(coder, 1,coder->l_don);
    }
    else
    {   
        if (take_dong(coder->l_don, coder) == 2)
            go_to_sleep(coder, 0, coder->l_don);
        if (take_dong(coder->r_don, coder) == 2)
            go_to_sleep(coder, 2, coder->r_don);
    }

}

void go_to_sleep(t_coder *coder, int dongle, t_dongle *x_dongle) //1 derecha 2 izquierda 0 nada
{
    if (coder->ndong != 0) 
    {
        if (dongle == 1)
            is_available(coder->r_don);
        else
            is_available(coder->l_don);
    }
    coder->ndong = 0;
    while(1)
    {
        requesting(x_dongle, coder);
        pthread_cond_wait(&x_dongle->cond, &x_dongle->mutex);
        if (fifo_thing(x_dongle, coder->shared->data->coders, coder->id) == coder->id)
            break;
        if (is_sim(coder->shared) == 1)
        {
            pthread_mutex_unlock(&x_dongle->mutex);
            return (1);
        }
    }
}