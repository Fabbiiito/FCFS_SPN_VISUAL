#include "Contenido.h"
#include "resource.h"

HWND hBasics;
HWND hStackArray;
HWND hStackLinked;

HWND hNoExpulsivas;
HWND hFCFS;
HWND hSPN;

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
