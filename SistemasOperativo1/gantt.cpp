#include <windows.h>
#include "resource.h"
#include "proceso.h"
#include "planificador.h"
int offX=0, offY=0;
bool drag=false;
extern POINT last;   // ✅
void barraProceso(HDC hdc, int x, int y, int alto, Proceso p, int escala)
{
    int xi = x + p.inicio * escala;
    int xf = x + p.fin * escala;

    Rectangle(hdc, xi, y, xf, y + alto);

    char t[2] = { p.nombre, 0 };
    DrawText(hdc, t, -1,
             new RECT{ xi, y, xf, y + alto },
             DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
void dibujarGanttCompleto(HDC hdc)
{
    int x = 80;
    int y = 80;
    int alto = 30;
    int escala = 25;

    TextOut(hdc, x, y-40, "FCFS", 4);

    // ---- FCFS
    calcularFCFS();
    for(int i=0;i<nProcesos;i++)
    {
        barraProceso(hdc, x, y + i*alto, alto, copia[i], escala);
    }

    // ---- SPN (debajo)
    y += nProcesos*alto + 120;
    TextOut(hdc, x, y-40, "SPN", 3);

    calcularSPN();
    for(int i=0;i<nProcesos;i++)
    {
        barraProceso(hdc, x, y + i*alto, alto, copia[i], escala);
    }
}

INT_PTR CALLBACK GanttProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd,&ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));

            dibujarGanttCompleto(hdc);

            EndPaint(hwnd,&ps);
            return TRUE;
        }

        case WM_CLOSE:
            EndDialog(hwnd,0);
            return TRUE;
    }
    return FALSE;
}
