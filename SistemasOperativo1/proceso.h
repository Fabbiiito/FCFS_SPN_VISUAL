#ifndef PROCESO_H_INCLUDED
#define PROCESO_H_INCLUDED
#pragma once

struct Proceso{
    char nombre;
    int llegada;
    int ejecucion;
    int inicio;
    int fin;
    int espera;
    int retorno;
    bool terminado;
};


#endif // PROCESO_H_INCLUDED
