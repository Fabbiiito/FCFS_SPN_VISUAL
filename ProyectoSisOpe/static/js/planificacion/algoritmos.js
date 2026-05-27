// Motor Matemático del Simulador de Planificación - 4 Algoritmos Oficiales
const MotorPlanificacion = {
    
    // 1. Algoritmo FCFS (First Come, First Served)
    calcularFCFS: function(procesos) {
        let tiempo = 0;
        let rutaEjecucion = [];
        let copia = procesos.map(p => ({ ...p, restante: p.burst }));
        
        // Se despacha estrictamente en orden de llegada
        let colaDespacho = [...copia].sort((a, b) => a.llegada - b.llegada);
        
        while (colaDespacho.some(p => p.restante > 0)) {
            let disponibles = colaDespacho.filter(p => p.llegada <= tiempo && p.restante > 0);
            
            if (disponibles.length > 0) {
                let p = disponibles[0];
                rutaEjecucion.push(p.id);
                p.restante--;
            } else {
                rutaEjecucion.push("IDLE");
            }
            tiempo++;
        }
        return rutaEjecucion;
    },

    // 2. Algoritmo RR (Round Robin) - Usa el Quantum dinámico que viene del input
    calcularRR: function(procesos, quantum = 2) {
        let tiempo = 0;
        let rutaEjecucion = [];
        let copia = procesos.map(p => ({ ...p, restante: p.burst }));
        
        let colaReady = [];
        let procesoActivo = null;
        let qCount = 0;

        while (copia.some(p => p.restante > 0) || colaReady.length > 0 || procesoActivo) {
            // Entran los procesos que llegan en este segundo exacto
            copia.forEach(p => {
                if (p.llegada === tiempo && p !== procesoActivo && !colaReady.includes(p)) {
                    colaReady.push(p);
                }
            });

            if (!procesoActivo && colaReady.length > 0) {
                procesoActivo = colaReady.shift();
                qCount = 0;
            }

            if (procesoActivo) {
                rutaEjecucion.push(procesoActivo.id);
                procesoActivo.restante--;
                qCount++;

                // Arribos intermedios para el siguiente instante de tiempo
                copia.forEach(p => {
                    if (p.llegada === tiempo + 1 && p !== procesoActivo && !colaReady.includes(p) && p.restante > 0) {
                        colaReady.push(p);
                    }
                });

                if (procesoActivo.restante === 0) {
                    procesoActivo = null;
                } else if (qCount === quantum) {
                    colaReady.push(procesoActivo);
                    procesoActivo = null;
                }
            } else {
                rutaEjecucion.push("IDLE");
            }
            tiempo++;
        }
        return rutaEjecucion;
    },

    // 3. Algoritmo SPN (Shortest Process Next) - No apropiativo (No hay desalojo)
    calcularSPN: function(procesos) {
        let tiempo = 0;
        let rutaEjecucion = [];
        let copia = procesos.map(p => ({ ...p, restante: p.burst }));
        let procesoActivo = null;

        while (copia.some(p => p.restante > 0) || procesoActivo) {
            // Si no hay un proceso ejecutándose, buscamos el más corto de los que ya llegaron
            if (!procesoActivo) {
                let disponibles = copia.filter(p => p.llegada <= tiempo && p.restante > 0);
                if (disponibles.length > 0) {
                    // Ordena por ráfaga total (burst) para elegir el más corto
                    disponibles.sort((a, b) => a.burst - b.burst);
                    procesoActivo = disponibles[0];
                }
            }

            if (procesoActivo) {
                rutaEjecucion.push(procesoActivo.id);
                procesoActivo.restante--;

                // Si terminó su ráfaga, liberamos la CPU para el próximo ciclo
                if (procesoActivo.restante === 0) {
                    procesoActivo = null;
                }
            } else {
                rutaEjecucion.push("IDLE");
            }
            tiempo++;
        }
        return rutaEjecucion;
    },

    // 4. Algoritmo SRT (Shortest Remaining Time) - Apropiativo (Con desalojo)
    calcularSRT: function(procesos) {
        let tiempo = 0;
        let rutaEjecucion = [];
        let copia = procesos.map(p => ({ ...p, restante: p.burst }));

        while (copia.some(p => p.restante > 0)) {
            let disponibles = copia.filter(p => p.llegada <= tiempo && p.restante > 0);
            
            if (disponibles.length > 0) {
                // Evalúa segundo a segundo quién tiene el menor tiempo RESTANTE
                disponibles.sort((a, b) => a.restante - b.restante);
                let p = disponibles[0];
                rutaEjecucion.push(p.id);
                p.restante--;
            } else {
                rutaEjecucion.push("IDLE");
            }
            tiempo++;
        }
        return rutaEjecucion;
    }
};