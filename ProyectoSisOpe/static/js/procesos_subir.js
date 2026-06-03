document.addEventListener("DOMContentLoaded", () => {
    const dropZone = document.getElementById('dropZone');
    const fileInput = document.getElementById('fileInput');
    const previewContainer = document.getElementById('previewContainer');
    const previewTbody = document.getElementById('previewTbody');
    const uploadStatus = document.getElementById('uploadStatus');
    const btnGuardarArchivo = document.getElementById('btnGuardarArchivo');
    let currentFile = null;

    // Hacer que la zona reaccione al clic
    dropZone.addEventListener('click', () => fileInput.click());

    // Eventos para arrastrar archivos (Drag & Drop)
    dropZone.addEventListener('dragover', (e) => {
        e.preventDefault();
        dropZone.classList.add('drop-zone-active');
    });
    //recorre cada elemento.
    ['dragleave', 'drop'].forEach(eventName => {
        dropZone.addEventListener(eventName, () => dropZone.classList.remove('drop-zone-active'));
    });
    //Se ejecuta cuando el usuario suelta un archivo.
    dropZone.addEventListener('drop', (e) => {
        e.preventDefault();
        const files = e.dataTransfer.files;
        if (files.length > 0) {
            currentFile = files[0];
            procesarArchivo(currentFile);
        }
    });

    // Se ejecuta cuando el usuario selecciona un archivo.
    fileInput.addEventListener('change', (e) => {
        if (e.target.files.length > 0) {
            currentFile = e.target.files[0];
            procesarArchivo(currentFile);
        }
    });

    // Función principal para leer y parsear el archivo
    function procesarArchivo(file) {
        const reader = new FileReader();//Crear lector de archivos
        
        reader.onload = function(e) {//Cuando termine de leer
            const contenido = e.target.result;
            const lineas = contenido.split('\n');
            
            // Limpiamos la tabla anterior
            previewTbody.innerHTML = '';
            let filasValidas = 0;
            //Recorrer cada línea
            lineas.forEach(linea => {
                // Limpiar espacios vacíos y separar por comas, punto y coma, o pestañas
                const datos = linea.trim().split(/[\s,;]+/);
                
                // Validamos que la línea tenga las 4 entradas que pides
                if (datos.length >= 4 && datos[0] !== "") {
                    const nombre = datos[0];
                    const tLlegada = datos[1];
                    const tEjecucion = datos[2];
                    const memoria = datos[3];

                    // Crear fila HTML
                    const tr = document.createElement('tr');
                    tr.innerHTML = `
                        <td><strong>${nombre}</strong></td>
                        <td>${tLlegada} s</td>
                        <td>${tEjecucion} s</td>
                        <td>${memoria} KB</td>
                    `;
                    previewTbody.appendChild(tr);
                    filasValidas++;
                }
            });

            // Si leyó datos correctos, muestra el contenedor del rectángulo de la tabla
            if (filasValidas > 0) {
                previewContainer.style.display = 'block';
                uploadStatus.textContent = `Archivo listo: ${file.name}`;
            } else {
                alert('el archivo no tiene el formato correcto de 4 columnas (Nombre Llegada Ejecucion Memoria).');
                previewContainer.style.display = 'none';
                uploadStatus.textContent = '';
            }
        };

        reader.readAsText(file);
    }

    btnGuardarArchivo.addEventListener('click', () => {
        if (!currentFile) {
            alert('Primero debes seleccionar o arrastrar un archivo válido.');
            return;
        }

        const formData = new FormData();
        formData.append('archivo', currentFile);
        uploadStatus.textContent = 'Guardando archivo...';

        fetch('/api/guardar', {
            method: 'POST',
            body: formData
        })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    uploadStatus.textContent = `Guardado correctamente: ${data.nombre}`;
                    if (typeof cargarHistorial === 'function') {
                        cargarHistorial();
                    }
                } else {
                    uploadStatus.textContent = data.error || 'Error al guardar el archivo.';
                }
            })
            .catch(() => {
                uploadStatus.textContent = 'Error de conexión al guardar el archivo.';
            });
    });
});