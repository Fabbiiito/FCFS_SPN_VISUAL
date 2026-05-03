#include "VExpulsivas.h"

HWND hBtnSRT = NULL;
HWND hBtnRR  = NULL;
HWND hEditQuantum = NULL;
HWND hDropZone = NULL;
void CreateVExpulsivas(HWND hDlg)
{
    // BOTÓN SRT
    hBtnSRT = CreateWindow(
        "BUTTON",
        "SRT",
        WS_CHILD,
        20, 80, 140, 35,
        hDlg,
        (HMENU)7001,
        NULL,
        NULL
    );

    // BOTÓN RR (más pequeño)
    hBtnRR = CreateWindow(
        "BUTTON",
        "RR",
        WS_CHILD,
        20, 125, 100, 35,
        hDlg,
        (HMENU)7002,
        NULL,
        NULL
    );

    // CAMPO QUANTUM (al costado de RR)
    hEditQuantum = CreateWindow(
        "EDIT",
        "1", // 🔥 valor por defecto
        WS_CHILD | WS_BORDER | ES_CENTER,
        130, 125, 40, 35, // al costado de RR
        hDlg,
        (HMENU)7003,
        NULL,
        NULL
    );
    hDropZone = CreateWindow(
    "STATIC",
    "Arrastra tu archivo TXT aqui",
    WS_CHILD | WS_VISIBLE | SS_CENTER | WS_BORDER,
    20, 700, 150, 50,
    hDlg,
    (HMENU)8001,
    NULL,
    NULL
);
ShowWindow(hDropZone, SW_HIDE);

    // OCULTAR TODO AL INICIO
    ShowWindow(hBtnSRT, SW_HIDE);
    ShowWindow(hBtnRR, SW_HIDE);
    ShowWindow(hEditQuantum, SW_HIDE);
}
void MostrarVExpulsivas()
{
    ShowWindow(hBtnSRT, SW_SHOW);
    ShowWindow(hBtnRR, SW_SHOW);
    ShowWindow(hEditQuantum, SW_SHOW);
    ShowWindow(hDropZone, SW_SHOW);
}

void OcultarVExpulsivas()
{
    ShowWindow(hBtnSRT, SW_HIDE);
    ShowWindow(hBtnRR, SW_HIDE);
    ShowWindow(hEditQuantum, SW_HIDE);
}
