#include "codexion.h"

void go_to_sleep(t_coder *coder, int dongle, t_dongle *x_dongle) //1 derecha 2 izquierda 0 nada
{
    free_dongle(coder, dongle)
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
    compiling(coder)
}

void    free_dongle(t_coder *coder, int ndong)
{
    if (ndong == 0)
        is_available(coder->r_don);
    else if (ndong == 1)
        is_available(coder->l_don);
    else if (ndong == 2)
        {
            is_available(coder->l_don);
            is_available(coder->r_don);
        }
    coder->ndong = 0;
}