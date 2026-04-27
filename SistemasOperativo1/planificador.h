#ifndef PLANIFICADOR_H_INCLUDED
#define PLANIFICADOR_H_INCLUDED

#pragma once
#include "proceso.h"

// datos globales
extern Proceso copia[100];
extern int nProcesos;

// funciones de cálculo
void calcularFCFS();
void calcularSPN();

#endif // PLANIFICADOR_H_INCLUDED
