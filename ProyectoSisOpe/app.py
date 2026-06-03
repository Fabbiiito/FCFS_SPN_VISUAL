import os
from datetime import datetime
from flask import Flask, render_template, request, jsonify
from werkzeug.utils import secure_filename

app = Flask(__name__)

UPLOAD_FOLDER = os.path.join(app.root_path, 'uploads')
os.makedirs(UPLOAD_FOLDER, exist_ok=True)

@app.route('/')
def index():
    return render_template('base.html')

@app.route('/api/guardar', methods=['POST'])
def api_guardar():
    if 'archivo' not in request.files:
        return jsonify(success=False, error='No se recibió ningún archivo.'), 400

    archivo = request.files['archivo']
    if archivo.filename == '':
        return jsonify(success=False, error='No se seleccionó ningún archivo.'), 400

    filename = secure_filename(archivo.filename)
    if filename == '':
        return jsonify(success=False, error='Nombre de archivo inválido.'), 400

    save_path = os.path.join(UPLOAD_FOLDER, filename)
    #¿Ya existe ese archivo?
    if os.path.exists(save_path):
        nombre_base, extension = os.path.splitext(filename)
        timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
        filename = f"{nombre_base}_{timestamp}{extension}"
        save_path = os.path.join(UPLOAD_FOLDER, filename)

    archivo.save(save_path)
    return jsonify(success=True, nombre=filename)

#sirve para leer un archivo 
# que ya está guardado en la carpeta uploads y devolver su contenido al navegador.
@app.route('/api/archivo')
def api_archivo():
    nombre = request.args.get('nombre', '')
    if not nombre:
        return jsonify(success=False, error='Nombre de archivo requerido.'), 400

    archivo_seguro = os.path.basename(nombre)
    path = os.path.join(UPLOAD_FOLDER, archivo_seguro)
    if not os.path.isfile(path):
        return jsonify(success=False, error='Archivo no encontrado.'), 404

    try:
        with open(path, 'r', encoding='utf-8') as f:
            contenido = f.read()
    except Exception as e:
        return jsonify(success=False, error=f'No se pudo leer el archivo: {str(e)}'), 500

    return jsonify(success=True, nombre=archivo_seguro, texto=contenido)
#sirve para obtener la lista
# de todos los archivos guardados en la carpeta uploads y enviarla al frontend.
@app.route('/api/historial')
def api_historial():
    archivos_guardados = []
    try:
        for filename in sorted(os.listdir(UPLOAD_FOLDER), reverse=True):
            path = os.path.join(UPLOAD_FOLDER, filename)
            if os.path.isfile(path):
                fecha_formato = datetime.fromtimestamp(os.path.getmtime(path)).strftime('%d/%m/%Y %H:%M')
                archivos_guardados.append({
                    'nombre': filename,
                    'fecha': fecha_formato
                })
    except Exception as e:
        return jsonify(success=False, error=str(e)), 500

    return jsonify(archivos=archivos_guardados)

if __name__ == '__main__':
    app.run(debug=True)