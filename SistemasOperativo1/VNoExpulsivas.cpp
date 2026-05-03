#include "VNoExpulsivas.h"

HWND hBtnFCFS = NULL;
HWND hBtnSPN  = NULL;

void CreateVNoExpulsivas(HWND hDlg)
{
    hBtnFCFS = CreateWindow(
        "BUTTON",
        "FCFS",
        WS_CHILD,
        20, 80, 140, 35,
        hDlg,
        (HMENU)6001,
        NULL,
        NULL
    );

    hBtnSPN = CreateWindow(
        "BUTTON",
        "SPN",
        WS_CHILD,
        20, 125, 140, 35,
        hDlg,
        (HMENU)6002,
        NULL,
        NULL
    );

    ShowWindow(hBtnFCFS, SW_HIDE);
    ShowWindow(hBtnSPN, SW_HIDE);
}

void MostrarVNoExpulsivas()
{
    ShowWindow(hBtnFCFS, SW_SHOW);
    ShowWindow(hBtnSPN, SW_SHOW);
}

void OcultarVNoExpulsivas()
{
    ShowWindow(hBtnFCFS, SW_HIDE);
    ShowWindow(hBtnSPN, SW_HIDE);
}
