#include "RR.h"
#include "../Datos/Procesos.h"

void EjecutarRR()
{
    // 🔥 prueba simple
    for (auto& p : listaProcesos)
    {
        p.retorno = p.rafaga; // ejemplo
    }
}
