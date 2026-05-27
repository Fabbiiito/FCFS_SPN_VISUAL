document.addEventListener("DOMContentLoaded", () => {
    // Variables de Control Globales del Simulador
    let tiempoActual = 0;
    let timerSimulacion = null;
    let procesosOriginales = [];
    let historialSimulacion = []; // Almacena el resultado del motor matemático
    
    // Valores de configuración del sistema (Valores iniciales por defecto)
    let tipoParticionActiva = "Fijas";
    let estrategiaActiva = "PrimerAjuste";
    let ramTotalGlobal = 1024;
    let soReservadoGlobal = 128;
    let listaParticionesFijasGlobal = [4, 10, 16];

    const paletaColores = ["#4cd3e6", "#a29bfe", "#ff7675", "#55efc4", "#ffeaa7", "#fd79a8", "#74b9ff"];
    let mapaColores = {};

    // Elementos del DOM Generales (localizados dentro del panel de memoria para evitar conflictos)
    const panelMem = document.getElementById('panel-memoria');
    const btnConfiguracion = (panelMem && panelMem.querySelector('#btnConfiguracion')) || document.getElementById('btnConfiguracion');
    const modalConfig = (panelMem && panelMem.querySelector('#modalConfig')) || document.getElementById('modalConfig');
    const btnGuardarConfig = (panelMem && panelMem.querySelector('#btnGuardarConfig')) || document.getElementById('btnGuardarConfig');
    const configActualDisplay = (panelMem && panelMem.querySelector('#configActualDisplay')) || document.getElementById('configActualDisplay');
    const archivoDisplay = (panelMem && panelMem.querySelector('#archivoCargadoDisplay')) || document.getElementById('archivoCargadoDisplay');
    const btnEjecutar = (panelMem && panelMem.querySelector('#btnEjecutar')) || document.getElementById('btnEjecutar');
    const btnReset = (panelMem && panelMem.querySelector('#btnReset')) || document.getElementById('btnReset');
    const speedControl = (panelMem && panelMem.querySelector('#speedControl')) || document.getElementById('speedControl');
    const systemTimeDisplay = (panelMem && panelMem.querySelector('#systemTime')) || document.getElementById('systemTime');
    const ramPilaContainer = (panelMem && panelMem.querySelector('#ramPilaContainer')) || document.getElementById('ramPilaContainer');
    const tablaBody = (panelMem && panelMem.querySelector('#tablaMemoriaBody')) || document.getElementById('tablaMemoriaBody');

    // Métricas del DOM
    const metricProcesos = document.getElementById('metricProcesos');
    const metricUsoMemoria = document.getElementById('metricUsoMemoria');
    const metricFragExterna = document.getElementById('metricFragExterna');
    const metricFragInterna = document.getElementById('metricFragInterna');

    // Elementos del DOM del Modal Reestructurado
    const toggleFijas = document.getElementById('toggleParticionesFijas');
    const toggleLabelText = document.getElementById('toggleLabelText');
    const subventanaFijas = document.getElementById('subventanaFijas');
    const listaParticiones = document.getElementById('listaParticionesDinamicas');
    const btnAnadirParticion = document.getElementById('btnAnadirParticion');

    let contadorParticiones = 0;
    const particionesPredeterminadas = [4, 10, 16];

    // ==========================================
    // LÓGICA DEL MODAL DINÁMICO DE CONFIGURACIÓN
    // ==========================================
    
    function inicializarEventosModalAvanzado() {
        // 1. Escuchar el cambio del interruptor de particiones fijas
        toggleFijas.addEventListener('change', () => {
            if (toggleFijas.checked) {
                toggleLabelText.textContent = "Usar particiones fijas (activado)";
                subventanaFijas.style.display = "block"; // Despliega la subventana
                
                // Si la lista está vacía al activar, cargamos los valores por defecto
                if (listaParticiones.children.length === 0) {
                    contadorParticiones = 0;
                    particionesPredeterminadas.forEach(tam => crearFilaParticion(tam));
                }
            } else {
                toggleLabelText.textContent = "Usar particiones fijas (desactivado)";
                subventanaFijas.style.display = "none";  // Oculta la subventana
            }
        });

        // 2. Escuchar el botón para añadir nuevas particiones
        btnAnadirParticion.addEventListener('click', () => {
            crearFilaParticion(32); // Añade una por defecto de 32 KB para editar
        });
    }

    // Función encargada de renderizar la fila de la partición con su botón X para borrar
    function crearFilaParticion(tamanioValor) {
        contadorParticiones++;
        const idActual = contadorParticiones;

        const row = document.createElement('div');
        row.className = "particion-row";
        row.id = `particion-row-${idActual}`;

        row.innerHTML = `
            <span class="particion-label">P${idActual}</span>
            <input type="number" class="particion-input-box" value="${tamanioValor}" min="1">
            <span class="particion-unit">KB</span>
            <button type="button" class="btn-eliminar-particion" title="Eliminar partición">✕</button>
        `;

        // Programar el comportamiento del botón eliminar (X)
        row.querySelector('.btn-eliminar-particion').addEventListener('click', () => {
            row.remove();
            renombrarParticionesSecuenciales();
        });

        listaParticiones.appendChild(row);
    }

    // Función auxiliar para re-numerar las etiquetas (P1, P2, P3...) si se borra una intermedia
    function renombrarParticionesSecuenciales() {
        const filas = listaParticiones.querySelectorAll('.particion-row');
        contadorParticiones = 0;
        filas.forEach((fila) => {
            contadorParticiones++;
            fila.querySelector('.particion-label').textContent = `P${contadorParticiones}`;
        });
    }

    // Abrir Modal
    btnConfiguracion.addEventListener('click', () => {
        modalConfig.style.display = "flex";
    });

    // 🚀 CORREGIDO: EVENTO DEL BOTÓN CONFIRMAR (PROCESAMIENTO DE DATOS)
    btnGuardarConfig.addEventListener('click', () => {
        const selectAjuste = document.getElementById('selectAjuste');

        // Captura y actualización de variables globales desde el nuevo diseño
        ramTotalGlobal = parseInt(document.getElementById('inputRamTotal').value) || 1024;
        soReservadoGlobal = parseInt(document.getElementById('inputSoReservado').value) || 128;
        estrategiaActiva = selectAjuste.value;
        
        // Determinar tipo de partición según el estado del Toggle Switch
        if (toggleFijas.checked) {
            tipoParticionActiva = "Fijas";
            
            // Recolectar dinámicamente los tamaños de la subventana
            listaParticionesFijasGlobal = [];
            const inputs = listaParticiones.querySelectorAll('.particion-input-box');
            inputs.forEach(inp => {
                let val = parseInt(inp.value);
                if (!isNaN(val) && val > 0) {
                    listaParticionesFijasGlobal.push(val);
                }
            });
        } else {
            tipoParticionActiva = "Variables";
            listaParticionesFijasGlobal = [];
        }

        // Mostrar logs de depuración en consola
        console.log("--- CONFIGURACIÓN ACTUALIZADA CON ÉXITO ---");
        console.log("Estrategia:", estrategiaActiva);
        console.log("RAM Total:", ramTotalGlobal, "KB");
        console.log("SO Reservado:", soReservadoGlobal, "KB");
        console.log("Tipo Partición:", tipoParticionActiva);
        console.log("Lista Particiones:", listaParticionesFijasGlobal);

        // Actualizar UI y limpiar estados anteriores
        actualizarTextoConfig();
        modalConfig.style.display = "none";
        
        clearInterval(timerSimulacion);
        timerSimulacion = null;
        tiempoActual = 0;
        limpiarInterfazGrafica();
    });

    // ==========================================
    // LÓGICA DE CONTROL DEL SIMULADOR Y MOTOR
    // ==========================================

    function inicializarSimulador() {
        // No cargar archivos por defecto al iniciar. La lista de procesos
        // permanecerá vacía hasta que el usuario seleccione un archivo desde
        // el historial o suba uno manualmente.
        if (!procesosOriginales) procesosOriginales = [];
        // Mostrar que no hay archivo activo por defecto
        if (!archivoDisplay.textContent || archivoDisplay.textContent.trim() === '') {
            archivoDisplay.textContent = "Archivo: Ninguno";
        }

        mapaColores = {};
        procesosOriginales.forEach((p, idx) => {
            mapaColores[p.id] = paletaColores[idx % paletaColores.length];
        });

        actualizarTextoConfig();
        limpiarInterfazGrafica();
    }

    function cargarArchivoMemoriaDesdeTexto(texto, nombreArchivo) {
        const lineas = texto.split(/\r?\n/);
        const procesos = [];

        lineas.forEach(linea => {
            const datos = linea.trim().split(/[\s,;]+/);
            if (datos.length >= 4 && datos[0] !== "") {
                const id = datos[0];
                const duracion = parseFloat(datos[2]);
                const tamanio = parseInt(datos[3], 10);

                if (!Number.isFinite(duracion) || duracion <= 0 || Number.isNaN(tamanio) || tamanio <= 0) {
                    return;
                }

                procesos.push({ id, llegada: 0, duracion, tamanio });
            }
        });

        if (procesos.length === 0) {
            alert('El archivo de memoria debe tener al menos cuatro columnas: ID, llegada, duración y memoria.');
            return false;
        }

        procesosOriginales = procesos;
        archivoDisplay.textContent = `Archivo: ${nombreArchivo || 'memoria.txt'}`;
        clearInterval(timerSimulacion);
        timerSimulacion = null;
        tiempoActual = 0;
        inicializarSimulador();
        renderizarTablaCarga();
        return true;
    }

    window.cargarArchivoMemoriaDesdeTexto = cargarArchivoMemoriaDesdeTexto;

    function limpiarInterfazGrafica() {
        systemTimeDisplay.textContent = "0";
        tablaBody.innerHTML = "";
        
        // Calcular porcentajes visuales adaptables dinámicamente según la RAM y SO configurados
        let porcentajeSO = (soReservadoGlobal / ramTotalGlobal) * 100;
        let porcentajeLibre = 100 - porcentajeSO;

        ramPilaContainer.innerHTML = `
            <div class="ram-block block-so" style="height: ${porcentajeSO}%;">SISTEMA OPERATIVO (SO)<br>[${soReservadoGlobal}K]</div>
            <div class="ram-block block-libre" style="height: ${porcentajeLibre}%;">ESPACIO TOTAL DISPONIBLE RAM<br>[${ramTotalGlobal - soReservadoGlobal}K]</div>
        `;

        metricProcesos.textContent = "0";
        metricUsoMemoria.textContent = `${Math.round(porcentajeSO)}%`;
        metricFragExterna.textContent = "0 KB";
        metricFragInterna.textContent = "0 KB";
    }

    function renderizarTablaCarga() {
        tablaBody.innerHTML = "";
        procesosOriginales.forEach(p => {
            const tr = document.createElement('tr');
            const tiempoDecimal = (parseFloat(p.duracion) / 10);
            const tiempoText = Number.isFinite(tiempoDecimal) ? tiempoDecimal.toFixed(1) : '-';
            tr.innerHTML = `
                <td><strong>${p.id}</strong></td>
                <td>${tiempoText} s</td>
                <td>-</td>
                <td>${p.tamanio} KB</td>
                <td>-</td>
            `;
            tablaBody.appendChild(tr);
        });
    }

    function actualizarTextoConfig() {
        configActualDisplay.textContent = `Configuración: ${tipoParticionActiva} - ${estrategiaActiva}`;
    }

    // Bucle de renderizado segundo a segundo controlado por slider de velocidad
    function ejecutarPasoMemoria() {
        if (tiempoActual >= historialSimulacion.length) {
            clearInterval(timerSimulacion);
            return;
        }

        let snapshot = historialSimulacion[tiempoActual];

        // 1. Renderizar Tabla Global
        tablaBody.innerHTML = "";
        snapshot.estadoProcesos.forEach(p => {
            if (tiempoActual >= p.llegada) {
                const tr = document.createElement('tr');
                let badgeClass = "status-espera";
                if (p.estado === "Proceso") badgeClass = "status-ejecucion";
                if (p.estado === "Terminado") badgeClass = "status-terminado";

                tr.innerHTML = `
                    <td><strong>${p.id}</strong></td>
                    <td>${p.tiempoRestante} s</td>
                    <td>${p.direccionInicio === -1 ? "-" : p.direccionInicio}</td>
                    <td>${p.tamanio} KB</td>
                    <td><span class="status-badge ${badgeClass}">${p.estado}</span></td>
                `;
                tablaBody.appendChild(tr);
            }
        });

        // 2. Renderizar la Pila de RAM Vertical dinámicamente usando la RAM total global
        ramPilaContainer.innerHTML = "";
        
        // Calcular altura relativa usando la suma real de bloques del snapshot
        const totalBlocks = snapshot.mapaRAM.reduce((acc, b) => acc + (b.tamanio || 0), 0);

        // Bloque del Sistema Operativo adaptado (altura relativa al total snapshot)
        const blockSO = document.createElement('div');
        blockSO.className = "ram-block block-so";
        let pctSO = (soReservadoGlobal / (soReservadoGlobal + totalBlocks)) * 100;
        blockSO.style.height = `${pctSO}%`;
        blockSO.innerHTML = `SISTEMA OPERATIVO (SO)<br>[${soReservadoGlobal}K]`;
        ramPilaContainer.appendChild(blockSO);

        // Renderizar dinámicamente la memoria restante dividida entre los bloques calculados
        snapshot.mapaRAM.forEach(bloque => {
            const divB = document.createElement('div');
            let porcentajeAlto = (bloque.tamanio / (soReservadoGlobal + totalBlocks)) * 100;
            divB.style.height = `${porcentajeAlto}%`;

            if (bloque.tipo === "PROCESO") {
                divB.className = "ram-block";
                divB.style.backgroundColor = mapaColores[bloque.id];
                let fragText = (bloque.tamanio > bloque.realSize) ? `<br>Frag. Int: ${bloque.tamanio - bloque.realSize}K` : "";
                divB.innerHTML = `<strong>${bloque.id}</strong><br>[${bloque.realSize}K]${fragText}`;
            } else {
                divB.className = "ram-block block-libre";
                divB.innerHTML = `LIBRE<br>[${bloque.tamanio}K]`;
            }
            ramPilaContainer.appendChild(divB);
        });

        // 3. Actualizar tarjetas de Métricas en Tiempo Real
        let asignados = snapshot.estadoProcesos.filter(p => p.estado === "Proceso").length;
        metricProcesos.textContent = asignados;

        let ocupadoProcesos = snapshot.mapaRAM.filter(b => b.tipo === "PROCESO").reduce((acc, b) => acc + b.tamanio, 0);
        let usoTotalPct = Math.round(((soReservadoGlobal + ocupadoProcesos) / (soReservadoGlobal + totalBlocks)) * 100);
        metricUsoMemoria.textContent = `${usoTotalPct}%`;

        metricFragExterna.textContent = `${snapshot.fragExterna} KB`;
        metricFragInterna.textContent = `${snapshot.fragInterna} KB`;

        systemTimeDisplay.textContent = tiempoActual;
        tiempoActual++;
    }

    function iniciarIntervaloSimulacion() {
        clearInterval(timerSimulacion);
        let velocidadMS = 1600 - parseInt(speedControl.value);
        timerSimulacion = setInterval(ejecutarPasoMemoria, velocidadMS);
    }

    speedControl.addEventListener('input', () => {
        if (timerSimulacion !== null && tiempoActual < historialSimulacion.length) {
            iniciarIntervaloSimulacion();
        }
    });

    btnEjecutar.addEventListener('click', () => {
        clearInterval(timerSimulacion);
        tiempoActual = 0;
        
        // Pasa las variables globales al Motor de Memoria para que procese con los inputs modificados
        historialSimulacion = MotorMemoria.generarMapaSimulacion(
            procesosOriginales, 
            tipoParticionActiva, 
            estrategiaActiva, 
            ramTotalGlobal, 
            soReservadoGlobal, 
            listaParticionesFijasGlobal
        );
        
        iniciarIntervaloSimulacion();
    });

    btnReset.addEventListener('click', () => {
        clearInterval(timerSimulacion);
        timerSimulacion = null;
        tiempoActual = 0;
        inicializarSimulador();
    });

    // Encendido Inicial del Sistema
    inicializarEventosModalAvanzado();
    inicializarSimulador();
});