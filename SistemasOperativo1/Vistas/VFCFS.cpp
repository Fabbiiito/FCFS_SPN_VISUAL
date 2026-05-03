#include <windows.h>

HWND hBoxArchivo;
HWND hBoxTabla;
HWND hBoxGantt;
HWND hBoxResultados;

void MostrarVFCFS();
void OcultarVFCFS();

void CreateVFCFS(HWND hDlg)
{
    int x = 220;        // inicio contenido derecho
    int w = 650;        // ancho total panel derecho

    int top = 20;

    // ───────── RESULTADOS ─────────
    hBoxResultados = CreateWindow(
        "STATIC",
        "PANEL DE RESULTADOS",
        WS_CHILD | WS_VISIBLE | SS_CENTER | SS_SUNKEN,
        x, top + 100, w, 80,
        hDlg, NULL, NULL, NULL
    );

    // ───────── ARCHIVO TXT ─────────
    hBoxArchivo = CreateWindow(
        "STATIC",
        "PANEL ARCHIVO TXT",
        WS_CHILD | WS_VISIBLE | SS_CENTER | SS_SUNKEN,
        x, top + 190, w, 70,
        hDlg, NULL, NULL, NULL
    );

    // ───────── CONTENEDOR TABLA ─────────
    hBoxTabla = CreateWindow(
        "STATIC",
        "TABLA DE PROCESOS",
        WS_CHILD | WS_VISIBLE | SS_CENTER | SS_SUNKEN,
        x, top + 270, (w / 2) - 5, 250,
        hDlg, NULL, NULL, NULL
    );

    // ───────── CONTENEDOR GANTT ─────────
    hBoxGantt = CreateWindow(
        "STATIC",
        "DIAGRAMA DE GANTT",
        WS_CHILD | WS_VISIBLE | SS_CENTER | SS_SUNKEN,
        x + (w / 2) + 5, top + 270, (w / 2) - 5, 250,
        hDlg, NULL, NULL, NULL
    );

    // OCULTAR AL INICIO (IMPORTANTE)
    ShowWindow(hBoxArchivo, SW_HIDE);
    ShowWindow(hBoxTabla, SW_HIDE);
    ShowWindow(hBoxGantt, SW_HIDE);
    ShowWindow(hBoxResultados, SW_HIDE);
}
void MostrarFCFS()
{
    ShowWindow(hBoxArchivo, SW_SHOW);
    ShowWindow(hBoxTabla, SW_SHOW);
    ShowWindow(hBoxGantt, SW_SHOW);
    ShowWindow(hBoxResultados, SW_SHOW);
}
