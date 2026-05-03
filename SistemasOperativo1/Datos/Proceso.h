#pragma once

struct Proceso
{
    int id;
    int llegada;
    int rafaga;

    // para algoritmos
    int restante;
    int inicio;
    int fin;

    // métricas
    int espera;
    int retorno;
};
