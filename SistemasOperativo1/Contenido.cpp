#include "Contenido.h"
#include "resource.h"
#include "Global.h"
#include "MenuLateral.h"
HWND hBasics;
HWND hStackArray;
HWND hStackLinked;

HWND hNoExpulsivas;
HWND hFCFS;
HWND hSPN;
int vistaActual = 0;
HWND hBtnVolver = NULL;
void CreateContenido(HWND hDlg)
{


    hBasics = CreateWindowEx(
        0,
        "STATIC",
        "• texto 2",
        WS_CHILD | WS_VISIBLE,
        200,90,300,20,
        hDlg,
        (HMENU)(INT_PTR)2001,
        NULL,
        NULL);

    hStackArray = CreateWindowEx(
        0,
        "STATIC",
        "o subtexto1",
        WS_CHILD | WS_VISIBLE | SS_NOTIFY,
        220,115,350,20,
        hDlg,
        (HMENU)(INT_PTR)IDC_LINK_STACK_ARRAY,
        NULL,
        NULL);

    hStackLinked = CreateWindowEx(
        0,
        "STATIC",
        "o subtexto2",
        WS_CHILD | WS_VISIBLE | SS_NOTIFY,
        220,140,350,20,
        hDlg,
        (HMENU)(INT_PTR)IDC_LINK_STACK_LINKED,
        NULL,
        NULL);
//primer panel
     hNoExpulsivas = CreateWindowEx(
        0,
        "STATIC",
        "• planificación No Expulsivas",
        WS_CHILD | WS_VISIBLE | SS_NOTIFY,
        200,190,350,20,
        hDlg,
        (HMENU)(INT_PTR) IDC_LINK_no_expulsivas ,
        NULL,
        NULL);

    hFCFS = CreateWindowEx(
        0,
        "STATIC",
        "o FCFS ",
        WS_CHILD | WS_VISIBLE,
        205,215,300,20,
        hDlg,
        NULL,
        NULL,
        NULL);

    hSPN = CreateWindowEx(
        0,
        "STATIC",
        "o SPN",
        WS_CHILD | WS_VISIBLE,
        205,240,300,20,
        hDlg,
        NULL,
        NULL,
        NULL);
}
void OcultarMenuPrincipal()
{
    ShowWindow(hBasics, SW_HIDE);
    ShowWindow(hStackArray, SW_HIDE);
    ShowWindow(hStackLinked, SW_HIDE);

    ShowWindow(hNoExpulsivas, SW_HIDE);
    ShowWindow(hFCFS, SW_HIDE);
    ShowWindow(hSPN, SW_HIDE);

    ShowWindow(h1, SW_HIDE);
    ShowWindow(h2, SW_HIDE);
    ShowWindow(h3, SW_HIDE);
}
void MostrarMenuPrincipal()
{
    ShowWindow(hBasics, SW_SHOW);
    ShowWindow(hStackArray, SW_SHOW);
    ShowWindow(hStackLinked, SW_SHOW);

    ShowWindow(hNoExpulsivas, SW_SHOW);
    ShowWindow(hFCFS, SW_SHOW);
    ShowWindow(hSPN, SW_SHOW);

    ShowWindow(h1, SW_SHOW);
    ShowWindow(h2, SW_SHOW);
    ShowWindow(h3, SW_SHOW);
}
