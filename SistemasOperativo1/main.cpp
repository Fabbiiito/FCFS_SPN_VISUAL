#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "Header.h"
#include "MenuLateral.h"
#include "Contenido.h"

int idHover = 0;
HWND hSeparador;
HBRUSH hBrushGreen = CreateSolidBrush(RGB(0,102,51));
HBRUSH hBrushWhite = (HBRUSH)GetStockObject(WHITE_BRUSH);

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg)
    {
        case WM_INITDIALOG:
            CreateHeader(hDlg);
            CreateMenuLateral(hDlg);
            CreateContenido(hDlg);
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
                if (id >= 1101 && id <= 1299)
                {
                    char txt[128];
                    GetWindowText((HWND)lp, txt, 128);
                    MessageBox(hDlg, txt, "Click", MB_OK);
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
                // LINEA DIVISORA
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

                // =========================
                // CONTENIDO DERECHO
                // =========================

                int panelX = menuWidth + 30;

                // COLUMNAS
                int categoryX = panelX;
                int itemX     = panelX + 25;

                int panelWidth = width - panelX - 20;

                // =========================
                // primer bloque
                // =========================

                MoveWindow(
                    hNoExpulsivas,
                    categoryX,
                    90,
                    panelWidth,
                    30,
                    TRUE);

                MoveWindow(
                    hFCFS,
                    itemX,
                    125,
                    panelWidth,
                    25,
                    TRUE);


                MoveWindow(
                    hSPN,
                    itemX,
                    150,
                    panelWidth,
                    25,
                    TRUE);

                // =========================
                // segundo bloque
                // =========================

                MoveWindow(
                    hBasics,
                    categoryX,
                    220,
                    panelWidth,
                    30,
                    TRUE);


                MoveWindow(
                    hStackArray,
                    itemX,
                    255,
                    panelWidth,
                    30,
                    TRUE);


                MoveWindow(
                    hStackLinked,
                    itemX,
                    290,
                    panelWidth,
                    30,
                    TRUE);

                    // =========================
                    // REDRAW
                    // =========================

                    RedrawWindow(
                        hDlg,
                        NULL,
                        NULL,
                        RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN
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
