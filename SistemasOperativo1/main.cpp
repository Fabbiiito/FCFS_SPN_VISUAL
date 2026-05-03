#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "Header.h"
#include "MenuLateral.h"
#include "Contenido.h"
#include "Global.h"
#include "VNoExpulsivas.h"
#include "VFCFS.h"
#include <string>
#include "VExpulsivas.h"
#include "Datos/ArchivoTXT.h"
#include "Datos/Procesos.h"
#include "Algoritmos/RR.h"
#include "Algoritmos/SRT.h"
#include "Datos/Procesos.h"
int idHover = 0;
int algoritmoActual = 0;
HWND hSeparador;
HBRUSH hBrushGreen = CreateSolidBrush(RGB(0,102,51));
HBRUSH hBrushWhite = (HBRUSH)GetStockObject(WHITE_BRUSH);

void LimpiarPantalla()
{
    // OCULTAR TODO EL CONTENIDO
    OcultarMenuPrincipal();
    OcultarVNoExpulsivas();

    OcultarVExpulsivas();

    ShowWindow(hBoxArchivo, SW_HIDE);
    ShowWindow(hBoxTabla, SW_HIDE);
    ShowWindow(hBoxGantt, SW_HIDE);
    ShowWindow(hBoxResultados, SW_HIDE);

    ShowWindow(hBtnFCFS, SW_HIDE);
    ShowWindow(hBtnSPN, SW_HIDE);
    ShowWindow(hDropZone, SW_HIDE);


    // OCULTAR BOTÓN VOLVER
    ShowWindow(hBtnVolver, SW_HIDE);

    // RESTAURAR HEADER / FOOTER
    SetWindowText(hHeader, "SO POLITICAS");
    SetWindowText(hFooter, "Copyright Fabricio Mayta");
}
void ActualizarUI(HWND hDlg, int width, int height)
{
    if (vistaActual == 1)
    {
        ShowWindow(hBtnVolver, SW_SHOW);

        MoveWindow(
            hBtnVolver,
            (width / 2) - 50,
            height - 32,
            100,
            25,
            TRUE
        );
    }
    else
    {
        ShowWindow(hBtnVolver, SW_HIDE);
    }
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg)
    {
        case WM_INITDIALOG:
    CreateHeader(hDlg);
    CreateMenuLateral(hDlg);
    CreateContenido(hDlg);
    CreateVNoExpulsivas(hDlg);
    CreateVFCFS(hDlg);
    CreateVExpulsivas(hDlg);

    DragAcceptFiles(hDlg, TRUE);
    hSeparador = CreateWindowEx(
        0,
        "STATIC",
        "",
        WS_CHILD | WS_VISIBLE | SS_BLACKRECT,
        180, 35, 2, 400,
        hDlg,
        NULL,
        NULL,
        NULL
    );

    hBtnVolver = CreateWindow(
        "BUTTON",
        "Volver",
        WS_CHILD,
        0, 0, 0, 0,
        hDlg,
        (HMENU)5000,
        NULL,
        NULL
    );

    ShowWindow(hBtnVolver, SW_HIDE);

    return TRUE;

        case WM_MOUSEMOVE:
            {
                TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, hDlg, 0 };
                TrackMouseEvent(&tme);

                POINT pt;
                GetCursorPos(&pt);
                HWND hChild = WindowFromPoint(pt);

                if (hChild && GetParent(hChild) == hDlg)
                {
                    int id = GetDlgCtrlID(hChild);
                    if (id >= 1101 && id <= 1299)
                    {
                        if (idHover != id)
                        {
                            idHover = id;
                            InvalidateRect(hDlg, NULL, FALSE);
                        }
                    }
                    else if (idHover != 0)
                    {
                        idHover = 0;
                        InvalidateRect(hDlg, NULL, FALSE);
                    }
                }
                return TRUE;
            }
case WM_DROPFILES:
{
    HDROP hDrop = (HDROP)wp;

    char ruta[MAX_PATH];
    DragQueryFile(hDrop, 0, ruta, MAX_PATH);

    // 🔥 VALIDACIÓN AQUÍ

    std::string path = ruta;

    if (path.substr(path.find_last_of(".") + 1) != "txt")
    {
        MessageBox(hDlg, "Solo archivos .txt", "Error", MB_OK);
        DragFinish(hDrop);
        return TRUE;
    }

    // 🔥 SI ES VÁLIDO
    CargarArchivoTXT(ruta);

    DragFinish(hDrop);

    MessageBox(hDlg, "Archivo cargado correctamente", "OK", MB_OK);

    return TRUE;
}
        case WM_MOUSELEAVE:
            idHover = 0;
            InvalidateRect(hDlg, NULL, FALSE);
            return TRUE;

        case WM_CTLCOLORSTATIC:
            {
                HDC hdc = (HDC)wp;
                int id = GetDlgCtrlID((HWND)lp);

                if (id == IDC_HEADER_TEXT || id == IDC_FOOTER_TEXT)
                {
                    SetTextColor(hdc, RGB(255,255,0));
                    SetBkColor(hdc, RGB(0,102,51));
                    return (INT_PTR)hBrushGreen;
                }

                if (id >= 1101 && id <= 1299)
                {
                    SetBkMode(hdc, TRANSPARENT);
                    if (id == idHover)
                        SetTextColor(hdc, RGB(255,255,0));
                    else
                        SetTextColor(hdc, RGB(0,102,51));
                    return (INT_PTR)hBrushWhite;
                }

                SetBkMode(hdc, TRANSPARENT);
                return (INT_PTR)hBrushWhite;
            }

     case WM_COMMAND:
{
    int id = LOWORD(wp);

    // 1. Manejo de Links de texto (ID 1101 a 1299)
   if (id == IDC_LINK_no_expulsivas)
{
    vistaActual = 1;

    OcultarMenuPrincipal();
    ShowWindow(hBoxArchivo, SW_SHOW);
    SetWindowText(hHeader, "NO EXPULSIVAS");
    SetWindowText(hFooter, "VOLVER");

    ShowWindow(hBtnVolver, SW_SHOW);

    MostrarVNoExpulsivas();

    UpdateWindow(hDlg);
}

    // 2. Botón Volver (ID 5000)

    /*
    if (id == 5000)
    {
        vistaActual = 0;
        SetWindowText(hHeader, "SO POLITICAS");
        SetWindowText(hFooter, "Copyright Fabricio Mayta");

        ShowWindow(hBtnVolver, SW_HIDE); // NO USES DestroyWindow, solo ocúltalo
        MostrarMenuPrincipal();

        InvalidateRect(hDlg, NULL, TRUE); // Forzar repintado
        return TRUE;
    }
*/
/*
if (id == 5000)
{
    vistaActual = 0;

    SetWindowText(hHeader, "SO POLITICAS");
    SetWindowText(hFooter, "Copyright Fabricio Mayta");

    ShowWindow(hBtnVolver, SW_HIDE);

    OcultarVNoExpulsivas();

    MostrarMenuPrincipal();

    InvalidateRect(hDlg, NULL, TRUE);

    return TRUE;
}
*/if (id == 5000)
{
    vistaActual = 0;

    LimpiarPantalla();      // 🔥 borra TODO visual

    MostrarMenuPrincipal(); // 🔵 SOLO menú principal

    return TRUE;
}
if (id == 6001) // botón FCFS
{
     ShowWindow(hBoxArchivo, SW_SHOW);
    ShowWindow(hBoxTabla, SW_SHOW);
    ShowWindow(hBoxGantt, SW_SHOW);
    ShowWindow(hBoxResultados, SW_SHOW);

    return TRUE;
}
if (id == IDC_LINK_expulsivas)
{
    vistaActual = 1;

    OcultarMenuPrincipal();

    SetWindowText(hHeader, "EXPULSIVAS");
    SetWindowText(hFooter, "VOLVER");

    ShowWindow(hBtnVolver, SW_SHOW);

    MostrarVExpulsivas(); // 🔥 ESTA ES LA CLAVE

    UpdateWindow(hDlg);
}
if (id == 7002) // RR
{
    if (listaProcesos.empty())
    {
        MessageBox(hDlg, "Primero carga un archivo TXT", "Error", MB_OK);
        return TRUE;
    }

    EjecutarRR();       // 🔥 procesa
    MostrarFCFS();      // 🔥 muestra panel

    MessageBox(hDlg, "RR procesado correctamente", "OK", MB_OK);

    return TRUE;
}
if (id == 7001) // SRT
{
    if (listaProcesos.empty())
    {
        MessageBox(hDlg, "Primero carga un archivo TXT", "Error", MB_OK);
        return TRUE;
    }

    EjecutarSRT();
    MostrarFCFS();

    MessageBox(hDlg, "SRT procesado correctamente", "OK", MB_OK);

    return TRUE;
}

    return TRUE;
}
        case WM_SIZE:
            {
                int width = LOWORD(lp);
                int height = HIWORD(lp);

                int menuWidth = 180;

                // =========================
                // HEADER
                // =========================

                MoveWindow(
                    hHeader,
                    0,
                    0,
                    width,
                    35,
                    TRUE);

                // =========================
                // FOOTER
                // =========================

                MoveWindow(
                    hFooter,
                    0,
                    height - 35,
                    width,
                    35,
                    TRUE);
            // =========================
                // LINEA DIVISORA hFooter
                // =========================

                MoveWindow(
                    hSeparador,
                    menuWidth,
                    35,
                    2,
                    height - 70,
                    TRUE);

                // =========================
                // MENU IZQUIERDO
                // =========================

                MoveWindow(
                    h1,
                    20,
                    80,
                    menuWidth - 40,
                    30,
                    TRUE);

                MoveWindow(
                    h2,
                    20,
                    115,
                    menuWidth - 40,
                    30,
                    TRUE);

                MoveWindow(
                    h3,
                    20,
                    150,
                    menuWidth - 40,
                    30,
                    TRUE);
int panelX = menuWidth + 30;
int categoryX = panelX;
int itemX = panelX + 25;
int panelWidth = width - panelX - 20;

// =========================
// LAYOUT VERTICAL DINÁMICO
// =========================

int y = 90;

// =========================
// NO EXPULSIVAS
// =========================

MoveWindow(hNoExpulsivas, categoryX, y, panelWidth, 30, TRUE);
y += 35;

MoveWindow(hFCFS, itemX, y, panelWidth, 25, TRUE);
y += 30;

MoveWindow(hSPN, itemX, y, panelWidth, 25, TRUE);
y += 40;

// =========================
// EXPULSIVAS
// =========================

MoveWindow(hExpulsivas, categoryX, y, panelWidth, 30, TRUE);
y += 35;

MoveWindow(hSRT, itemX, y, panelWidth, 25, TRUE);
y += 30;

MoveWindow(hRR, itemX, y, panelWidth, 25, TRUE);
y += 40;

// =========================
// OTRO BLOQUE
// =========================

MoveWindow(hBasics, categoryX, y, panelWidth, 30, TRUE);
y += 35;

MoveWindow(hStackArray, itemX, y, panelWidth, 25, TRUE);
y += 30;

MoveWindow(hStackLinked, itemX, y, panelWidth, 25, TRUE);

                    // =========================
                    // REDRAW
                    // =========================

                    RedrawWindow(
                        hDlg,
                        NULL,
                        NULL,
                        RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN
                    );
MoveWindow(
    hBtnVolver,
    (width / 2) - 50,
    height - 32,   // dentro del footer
    100,
    25,
    TRUE
);

                return TRUE;
            }

            case WM_CLOSE:
                EndDialog(hDlg, 0);
                return TRUE;
        }
        return FALSE;
    }

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN_DIALOG), NULL, DialogProc);
}
