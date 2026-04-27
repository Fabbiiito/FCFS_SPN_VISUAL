#ifndef ALGORITMOS_H_INCLUDED
#define ALGORITMOS_H_INCLUDED


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

extern Proceso procesos[100];
extern int nProcesos;

void cargarArchivo(const char* nombre);
void calcularFCFS();
void calcularSPN();

#endif // ALGORITMOS_H_INCLUDED
