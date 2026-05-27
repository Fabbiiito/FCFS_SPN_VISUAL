const MotorMemoria = {
    // Simulación del comportamiento paso a paso de la RAM
    generarMapaSimulacion: function(procesosInput, tipoParticion, estrategia) {
        const TAMANIO_RAM = 1024; // 1024 KB totales
        const TAMANIO_SO = 256;   // 256 KB para el SO
        const RAM_DISPONIBLE = TAMANIO_RAM - TAMANIO_SO;

        let lineaTiempo = [];
        // Clonar procesos para no alterar originales
        let listaProcesos = procesosInput.map(p => ({
            id: p.id,
            duracion: p.duracion,
            tamanio: p.tamanio,
            llegada: p.llegada,
            tiempoRestante: p.duracion,
            direccionInicio: -1,
            estado: "Espera"
        }));

        // Determinamos la duración máxima de la simulación
        let tiempoMaximo = Math.max(...listaProcesos.map(p => p.llegada)) + 
                           listaProcesos.reduce((acc, p) => acc + p.duracion, 0) + 5;

        // Estructura de particiones iniciales si es FIJAS
        let particionesFijas = [
            { id: 1, inicio: 256, tamanio: 128, ocupadaPor: null },
            { id: 2, inicio: 384, tamanio: 128, ocupadaPor: null },
            { id: 3, inicio: 512, tamanio: 256, ocupadaPor: null },
            { id: 4, inicio: 768, tamanio: 256, ocupadaPor: null }
        ];

        // Particiones dinámicas si es VARIABLES (Inicia con un gran bloque vacío)
        let particionesVariables = [
            { inicio: 256, tamanio: RAM_DISPONIBLE, ocupadaPor: null }
        ];

        // Correr segundo a segundo
        for (let t = 0; t <= tiempoMaximo; t++) {
            
            // 1. Liberar memoria de procesos que ya terminaron en este segundo 't'
            if (tipoParticion === "Fijas") {
                particionesFijas.forEach(part => {
                    if (part.ocupadaPor) {
                        let pId = part.ocupadaPor;
                        let p = listaProcesos.find(proc => proc.id === pId);
                        if (p && p.tiempoRestante <= 0) {
                            part.ocupadaPor = null;
                        }
                    }
                });
            } else {
                particionesVariables.forEach(part => {
                    if (part.ocupadaPor) {
                        let pId = part.ocupadaPor;
                        let p = listaProcesos.find(proc => proc.id === pId);
                        if (p && p.tiempoRestante <= 0) {
                            part.ocupadaPor = null;
                        }
                    }
                });
                // Compactar/fusionar huecos libres contiguos en variables
                this.fusionarHuecosLibres(particionesVariables);
            }

            // 2. Intentar asignar procesos que ya llegaron y están en Espera
            listaProcesos.forEach(p => {
                if (t >= p.llegada && p.estado === "Espera") {
                    if (tipoParticion === "Fijas") {
                        this.asignarParticionFija(p, particionesFijas, estrategia);
                    } else {
                        this.asignarParticionVariable(p, particionesVariables, estrategia);
                    }
                }
            });

            // 3. Consumir tiempo de ejecución de los procesos que están corriendo (Asignados)
            listaProcesos.forEach(p => {
                if (p.estado === "Proceso" && p.tiempoRestante > 0) {
                    p.tiempoRestante--;
                    if (p.tiempoRestante === 0) {
                        p.estado = "Terminado";
                    }
                }
            });

            // 4. Capturar foto instantánea de la memoria en este segundo exacto
            let instantaneaRAM = [];
            let fragInternaSeg = 0;
            let fragExternaSeg = 0;

            if (tipoParticion === "Fijas") {
                particionesFijas.forEach(part => {
                    if (part.ocupadaPor) {
                        let proc = listaProcesos.find(proc => proc.id === part.ocupadaPor);
                        fragInternaSeg += (part.tamanio - proc.tamanio);
                        instantaneaRAM.push({ tipo: "PROCESO", id: proc.id, inicio: part.inicio, tamanio: part.tamanio, realSize: proc.tamanio });
                    } else {
                        instantaneaRAM.push({ tipo: "LIBRE", id: "LIBRE", inicio: part.inicio, tamanio: part.tamanio });
                    }
                });
            } else {
                particionesVariables.forEach(part => {
                    if (part.ocupadaPor) {
                        instantaneaRAM.push({ tipo: "PROCESO", id: part.ocupadaPor, inicio: part.inicio, tamanio: part.tamanio, realSize: part.tamanio });
                    } else {
                        fragExternaSeg += part.tamanio;
                        instantaneaRAM.push({ tipo: "LIBRE", id: "LIBRE", inicio: part.inicio, tamanio: part.tamanio });
                    }
                });
            }

            // Guardar datos del segundo actual para el renderizador dinámico
            lineaTiempo.push({
                tiempo: t,
                estadoProcesos: JSON.parse(JSON.stringify(listaProcesos)),
                mapaRAM: instantaneaRAM,
                fragInterna: fragInternaSeg,
                fragExterna: fragExternaSeg
            });

            // Si todos los procesos terminaron, rompemos el ciclo de tiempo
            if (listaProcesos.every(p => p.estado === "Terminado")) {
                break;
            }
        }

        return lineaTiempo;
    },

    asignarParticionFija: function(proceso, particiones, estrategia) {
        // Filtrar particiones vacías y que tengan el tamaño suficiente
        let candidatos = particiones.filter(p => p.ocupadaPor === null && p.tamanio >= proceso.tamanio);
        if (candidatos.length === 0) return; // Espera al próximo ciclo

        let elegida = null;
        if (estrategia === "PrimerAjuste") {
            elegida = candidatos[0];
        } else if (estrategia === "MejorAjuste") {
            candidatos.sort((a, b) => a.tamanio - b.tamanio);
            elegida = candidatos[0];
        } else if (estrategia === "PeorAjuste") {
            candidatos.sort((a, b) => b.tamanio - a.tamanio);
            elegida = candidatos[0];
        } else { // Default Primer Ajuste para Buddy System simplificado en fijas
            elegida = candidatos[0];
        }

        if (elegida) {
            elegida.ocupadaPor = proceso.id;
            proceso.direccionInicio = elegida.inicio + "K";
            proceso.estado = "Proceso";
        }
    },

    asignarParticionVariable: function(proceso, particiones, estrategia) {
        let candidatos = particiones.filter(p => p.ocupadaPor === null && p.tamanio >= proceso.tamanio);
        if (candidatos.length === 0) return;

        let elegida = null;
        if (estrategia === "PrimerAjuste") {
            elegida = candidatos[0];
        } else if (estrategia === "MejorAjuste") {
            candidatos.sort((a, b) => a.tamanio - b.tamanio);
            elegida = candidatos[0];
        } else if (estrategia === "PeorAjuste") {
            candidatos.sort((a, b) => b.tamanio - a.tamanio);
            elegida = candidatos[0];
        } else {
            elegida = candidatos[0];
        }

        if (elegida) {
            let index = particiones.indexOf(elegida);
            let sobrante = elegida.tamanio - proceso.tamanio;

            if (sobrante > 0) {
                // Se divide la partición: una para el proceso, otra libre para el resto
                let nuevaLibre = {
                    inicio: elegida.inicio + proceso.tamanio,
                    tamanio: sobrante,
                    ocupadaPor: null
                };
                elegida.tamanio = proceso.tamanio;
                particiones.splice(index + 1, 0, nuevaLibre);
            }

            elegida.ocupadaPor = proceso.id;
            proceso.direccionInicio = elegida.inicio + "K";
            proceso.estado = "Proceso";
        }
    },

    fusionarHuecosLibres: function(particiones) {
        for (let i = 0; i < particiones.length - 1; i++) {
            if (particiones[i].ocupadaPor === null && particiones[i+1].ocupadaPor === null) {
                particiones[i].tamanio += particiones[i+1].tamanio;
                particiones.splice(i + 1, 1);
                i--; // Re-evaluar posición tras eliminación
            }
        }
    }
};