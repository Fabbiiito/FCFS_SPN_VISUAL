#include "ArchivoTXT.h"
#include "Procesos.h"
#include <fstream>
#include <sstream>

void CargarArchivoTXT(const char* ruta)
{
    listaProcesos.clear(); // 🔥 limpiar antes de cargar

    std::ifstream archivo(ruta);

    if (!archivo.is_open())
        return;

    std::string linea;
    int id = 1;

    while (getline(archivo, linea))
    {
        std::stringstream ss(linea);

        Proceso p;

        ss >> p.llegada >> p.rafaga;

        p.id = id++;
        p.restante = p.rafaga;
        p.inicio = -1;
        p.fin = 0;
        p.espera = 0;
        p.retorno = 0;

        listaProcesos.push_back(p);
    }

    archivo.close();
}
