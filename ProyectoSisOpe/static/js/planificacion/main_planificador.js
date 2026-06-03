document.addEventListener("DOMContentLoaded", () => {
    let tiempoActual = 0;
    let timerSimulacion = null;
    let ejecutando = false;
    let algoritmoSeleccionado = "FCFS"; //Algoritmo que se utilizará inicialmente.
    let procesosOriginales = [];
    let mapaColores = {};
    let secuenciaEjecucion = [];
    
    // Almacenes para métricas individuales en tiempo real
    let tiemposInicio = {};
    let tiemposFinal = {};

    const paletaColores = ["#4cd3e6", "#a29bfe", "#ff7675", "#55efc4", "#ffeaa7", "#fd79a8", "#74b9ff"];

    // Busca el botón correcto únicamente dentro de la sección de planificación, ignorando por
    //  completo todo lo demás que haya en la página para no equivocarse.
    const panelPlan = document.getElementById('panel-planificacion');
    const btnReset = (panelPlan && panelPlan.querySelector('#btnReset')) || document.getElementById('btnReset');
    const btnEjecutar = (panelPlan && panelPlan.querySelector('#btnEjecutar')) || document.getElementById('btnEjecutar');
    const btnSeleccionarArchivo = (panelPlan && panelPlan.querySelector('#btnSeleccionarArchivo')) || document.getElementById('btnSeleccionarArchivo');
    const planificadorFileInput = (panelPlan && panelPlan.querySelector('#planificadorFileInput')) || document.getElementById('planificadorFileInput');
    const archivoActualDisplay = (panelPlan && panelPlan.querySelector('#archivoActualDisplay')) || document.getElementById('archivoActualDisplay');
    const quantumInput = (panelPlan && panelPlan.querySelector('#quantumInput')) || document.getElementById('quantumInput');
    const speedRange = (panelPlan && panelPlan.querySelector('#speedRange')) || document.getElementById('speedRange');
    const speedLabel = (panelPlan && panelPlan.querySelector('#speedLabel')) || document.getElementById('speedLabel');
    const systemTimeDisplay = (panelPlan && panelPlan.querySelector('#systemTime')) || document.getElementById('systemTime');
    const ganttTimeline = (panelPlan && panelPlan.querySelector('#ganttTimeline')) || document.getElementById('ganttTimeline');
    const ganttZoom = (panelPlan && panelPlan.querySelector('#ganttZoom')) || document.getElementById('ganttZoom');
    const tablaBody = (panelPlan && panelPlan.querySelector('#tablaProcesosBody')) || document.getElementById('tablaProcesosBody');
    //Velocidad inicial del simulador (700ms por paso) y archivo activo en "Ninguno" para que el usuario sepa que debe cargar uno.
    let velocidadIntervalo = parseInt(speedRange.value, 10) || 700;
    let archivoActivo = "Ninguno";

    // Inicializador en Standby (Dejo todo en blanco como pediste)
    function cargarProcesosDefault() {
        procesosOriginales = [
            { id: "P1", llegada: 0, burst: 5 },
            { id: "P2", llegada: 2, burst: 3 },
            { id: "P3", llegada: 4, burst: 2 },
            { id: "P4", llegada: 6, burst: 4 }
        ];
    }

    function establecerArchivoActivo(nombre) {
        archivoActivo = nombre || "Ningunoss";
        archivoActualDisplay.textContent = `Archivo activo: ${archivoActivo}`;
    }

    function inicializarSimulador() {
        // No cargar procesos por defecto al iniciar la app. El usuario
        // debe seleccionar un archivo desde el historial o subir uno.
        mapaColores = {};
        procesosOriginales.forEach((p, index) => {
            mapaColores[p.id] = paletaColores[index % paletaColores.length];
        });
        mapaColores["IDLE"] = "#232a34";

        ganttTimeline.innerHTML = "";
        renderizarTablaVacia();
        resetearMetricasVisuales();
        establecerArchivoActivo(archivoActivo);
    }

    function cargarArchivoDesdeTexto(texto, nombreArchivo) {
        const lineas = texto.split(/\r?\n/);
        const procesos = [];

        lineas.forEach(linea => {
            const datos = linea.trim().split(/[\s,;]+/);
            if (datos.length >= 3 && datos[0] !== "") {
                const id = datos[0];
                const llegada = parseFloat(datos[1]);
                const burst = parseFloat(datos[2]);

                if (!Number.isFinite(llegada) || !Number.isFinite(burst) || llegada < 0 || burst <= 0) {
                    return;
                }

                procesos.push({ id, llegada, burst });
            }
        });

        if (procesos.length === 0) {
            alert('El archivo no tiene un formato válido. Asegúrate de tener al menos tres columnas: ID, llegada y duración.');
            return false;
        }

        procesosOriginales = procesos;
        tiemposInicio = {};
        tiemposFinal = {};
        secuenciaEjecucion = [];
        tiempoActual = 0;
        establecerArchivoActivo(nombreArchivo || 'Ninguno');
        inicializarSimulador();
        return true;
    }

    function procesarAlgoritmoCompleto() {
        tiemposInicio = {};
        tiemposFinal = {};
        const qVal = parseInt(quantumInput.value) || 2;

        if (algoritmoSeleccionado === "FCFS") {
            secuenciaEjecucion = MotorPlanificacion.calcularFCFS(procesosOriginales);
        } else if (algoritmoSeleccionado === "RR") {
            secuenciaEjecucion = MotorPlanificacion.calcularRR(procesosOriginales, qVal);
        } else if (algoritmoSeleccionado === "SPN") {
            secuenciaEjecucion = MotorPlanificacion.calcularSPN(procesosOriginales);
        } else if (algoritmoSeleccionado === "SRT") {
            secuenciaEjecucion = MotorPlanificacion.calcularSRT(procesosOriginales);
        }
    }

    function renderizarTablaVacia() {
        tablaBody.innerHTML = "";
        procesosOriginales.forEach(p => {
            const tr = document.createElement('tr');
            tr.id = `fila-${p.id}`;
            tr.innerHTML = `
                <td><strong>${p.id}</strong></td>
                <td>${p.llegada} s</td>
                <td>${p.burst} s</td>
                <td id="t-retorno-${p.id}">-</td>
                <td id="t-espera-${p.id}">-</td>
                <td id="t-inicio-${p.id}">-</td>
                <td id="t-final-${p.id}">-</td>
                <td><span class="status-badge status-espera" id="status-${p.id}">Espera</span></td>
            `;
            tablaBody.appendChild(tr);
        });
    }

    function ejecutarPaso() {
        if (tiempoActual >= secuenciaEjecucion.length) {
            pausarSimulacion();
            return;
        }

        const procesoActualId = secuenciaEjecucion[tiempoActual];
        
        if (procesoActualId !== "IDLE" && tiemposInicio[procesoActualId] === undefined) {
            tiemposInicio[procesoActualId] = tiempoActual;
            document.getElementById(`t-inicio-${procesoActualId}`).textContent = `${tiempoActual}s`;
        }

        const block = document.createElement('div');
        block.className = "gantt-block";
        block.style.width = `${ganttZoom.value}px`;
        block.style.backgroundColor = mapaColores[procesoActualId];
        block.innerHTML = procesoActualId !== "IDLE" ? `<span>${procesoActualId}</span>` : "";
        ganttTimeline.appendChild(block);

        procesosOriginales.forEach(p => {
            const badge = document.getElementById(`status-${p.id}`);
            if (!badge) return;

            const historicoBurst = secuenciaEjecucion.slice(0, tiempoActual + 1).filter(id => id === p.id).length;
            const procesoTerminó = historicoBurst >= p.burst;

            if (tiempoActual < p.llegada) {
                badge.className = "status-badge status-espera";
                badge.textContent = "Espera";
            } else if (procesoActualId === p.id) {
                if (procesoTerminó) {
                    badge.className = "status-badge status-terminado";
                    badge.textContent = "Terminado";
                    if (tiemposFinal[p.id] === undefined) {
                        tiemposFinal[p.id] = tiempoActual + 1;
                        const tRetorno = tiemposFinal[p.id] - p.llegada;
                        const tEspera = tRetorno - p.burst;
                        document.getElementById(`t-final-${p.id}`).textContent = `${tiemposFinal[p.id]}s`;
                        document.getElementById(`t-retorno-${p.id}`).textContent = `${tRetorno}s`;
                        document.getElementById(`t-espera-${p.id}`).textContent = `${tEspera}s`;
                    }
                } else {
                    badge.className = "status-badge status-ejecucion";
                    badge.textContent = "Running";
                }
            } else {
                if (procesoTerminó) {
                    badge.className = "status-badge status-terminado";
                    badge.textContent = "Terminado";
                    if (tiemposFinal[p.id] === undefined) {
                        tiemposFinal[p.id] = tiempoActual + 1;
                        const tRetorno = tiemposFinal[p.id] - p.llegada;
                        const tEspera = tRetorno - p.burst;
                        document.getElementById(`t-final-${p.id}`).textContent = `${tiemposFinal[p.id]}s`;
                        document.getElementById(`t-retorno-${p.id}`).textContent = `${tRetorno}s`;
                        document.getElementById(`t-espera-${p.id}`).textContent = `${tEspera}s`;
                    }
                } else {
                    badge.className = "status-badge status-listo";
                    badge.textContent = "Listo";
                }
            }
        });

        tiempoActual++;
        systemTimeDisplay.textContent = tiempoActual;
        actualizarMétricasGlobales();
    }

    function actualizarMétricasGlobales() {
        let ingresados = procesosOriginales.filter(p => p.llegada < tiempoActual).length;
        document.getElementById('metricCount').textContent = ingresados;
        
        if (tiempoActual > 0) {
            let slotsCpu = secuenciaEjecucion.slice(0, tiempoActual);
            let utiles = slotsCpu.filter(id => id !== "IDLE").length;
            let pct = Math.round((utiles / tiempoActual) * 100);
            document.getElementById('metricCpu').textContent = `${pct}%`;
        }

        // Promedios dinámicos en base a los procesos que ya finalizaron
        let terminados = procesosOriginales.filter(p => tiemposFinal[p.id] !== undefined);
        if (terminados.length > 0) {
            let sumRetorno = 0, sumEspera = 0;
            terminados.forEach(p => {
                let ret = tiemposFinal[p.id] - p.llegada;
                sumRetorno += ret;
                sumEspera += (ret - p.burst);
            });
            document.getElementById('metricRetorno').textContent = `${(sumRetorno / terminados.length).toFixed(2)}s`;
            document.getElementById('metricEspera').textContent = `${(sumEspera / terminados.length).toFixed(2)}s`;
        }
    }

    function resetearMetricasVisuales() {
        document.getElementById('metricCount').textContent = "0";
        document.getElementById('metricCpu').textContent = "0%";
        document.getElementById('metricRetorno').textContent = "0s";
        document.getElementById('metricEspera').textContent = "0s";
    }

    function iniciarSimulacion() {
        if (secuenciaEjecucion.length === 0) return;
        ejecutando = true;
        timerSimulacion = setInterval(ejecutarPaso, velocidadIntervalo);
    }

    function pausarSimulacion() {
        ejecutando = false;
        clearInterval(timerSimulacion);
    }

    // GATILLO PRINCIPAL: Al pulsar Ejecutar, recién se puebla la tabla y se arma la secuencia
    btnEjecutar.addEventListener('click', () => {
        pausarSimulacion();
        tiempoActual = 0;
        systemTimeDisplay.textContent = "0";
        ganttTimeline.innerHTML = "";
        
        procesarAlgoritmoCompleto();
        renderizarTablaVacia();
        resetearMetricasVisuales();
        iniciarSimulacion();
    });

    speedRange.addEventListener('input', () => {
        velocidadIntervalo = parseInt(speedRange.value, 10) || 700;
        if (velocidadIntervalo <= 300) {
            speedLabel.textContent = 'Muy rápido';
        } else if (velocidadIntervalo <= 600) {
            speedLabel.textContent = 'Rápido';
        } else if (velocidadIntervalo <= 900) {
            speedLabel.textContent = 'Normal';
        } else {
            speedLabel.textContent = 'Lento';
        }
        if (ejecutando) {
            clearInterval(timerSimulacion);
            timerSimulacion = setInterval(ejecutarPaso, velocidadIntervalo);
        }
    });

    btnSeleccionarArchivo.addEventListener('click', () => {
        planificadorFileInput.click();
    });

    planificadorFileInput.addEventListener('change', (e) => {
        const file = e.target.files[0];
        if (!file) return;

        const reader = new FileReader();
        reader.onload = (event) => {
            if (cargarArchivoDesdeTexto(event.target.result, file.name)) {
                establecerArchivoActivo(file.name);
            }
        };
        reader.readAsText(file);
    });

    btnReset.addEventListener('click', () => {
        pausarSimulacion();
        tiempoActual = 0;
        secuenciaEjecucion = [];
        systemTimeDisplay.textContent = "0";
        ganttTimeline.innerHTML = "";
        renderizarTablaVacia();
        resetearMetricasVisuales();
    });

    ganttZoom.addEventListener('input', () => {
        const bloques = document.querySelectorAll('.gantt-block');
        bloques.forEach(b => b.style.width = `${ganttZoom.value}px`);
    });

    // Control de Iluminación Selectiva de la botonera
    document.querySelectorAll('.btn-algo').forEach(btn => {
        btn.addEventListener('click', (e) => {
            // Evitar conflictos si se hace clic directamente en el input de quantum
            if (e.target.tagName === 'INPUT') return;

            document.querySelectorAll('.btn-algo').forEach(b => b.classList.remove('active'));
            
            const targetBtn = e.target.closest('.btn-algo');
            targetBtn.classList.add('active');
            algoritmoSeleccionado = targetBtn.getAttribute('data-algo');
            
            // Forzar Standby al cambiar de algoritmo
            pausarSimulacion();
            inicializarSimulador();
        });
    });

    window.cargarArchivoDesdeTexto = cargarArchivoDesdeTexto;
    inicializarSimulador();
});