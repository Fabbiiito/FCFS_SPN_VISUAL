#include <windows.h>
#include <commctrl.h>
#include <fstream>
#include "resource.h"
#include "proceso.h"
#include "planificador.h"
extern INT_PTR CALLBACK GanttProc(HWND,UINT,WPARAM,LPARAM);
using namespace std;
using namespace std;
float promEspera = 0;
float promRetorno = 0;
// ------------------ ESTRUCTURA ------------------

Proceso procesos[100];
Proceso copia[100];
int nProcesos=0;

int algoritmo=1; // 1 FCFS, 2 SPN barraProceso

// offsets para arrastrar
int offsetX=0, offsetY=0;
bool dragging=false;
POINT last;

// ------------------ ARCHIVO ------------------

void cargarArchivo()
{
    ifstream f("procesos.txt");
    nProcesos=0;

    while(f >> procesos[nProcesos].nombre
        >> procesos[nProcesos].llegada
        >> procesos[nProcesos].ejecucion)
    {
        nProcesos++;
    }
    f.close();
}

void copiar()
{
    for(int i=0;i<nProcesos;i++)
        copia[i]=procesos[i];
}

// ------------------ FCFS ------------------

void calcularFCFS()
{

    copiar();
    int tiempo=0;

    for(int i=0;i<nProcesos;i++)
    {
        if(tiempo < copia[i].llegada)
            tiempo = copia[i].llegada;

        copia[i].inicio = tiempo;
        copia[i].fin = tiempo + copia[i].ejecucion;
        copia[i].espera = copia[i].inicio - copia[i].llegada;
        copia[i].retorno = copia[i].fin - copia[i].llegada;

        tiempo = copia[i].fin;
    }
    promEspera = 0;
promRetorno = 0;

for(int i=0;i<nProcesos;i++){
    promEspera += copia[i].espera;
    promRetorno += copia[i].retorno;
}

promEspera /= nProcesos;
promRetorno /= nProcesos;
}

// ------------------ SPN ------------------

void calcularSPN()
{
    copiar();

    for(int i=0;i<nProcesos;i++)
        copia[i].terminado=false;

    int completados=0, tiempo=0;

    while(completados<nProcesos)
    {
        int menor=-1;

        for(int i=0;i<nProcesos;i++)
        {
            if(!copia[i].terminado && copia[i].llegada<=tiempo)
            {
                if(menor==-1 ||
                   copia[i].ejecucion < copia[menor].ejecucion)
                    menor=i;
            }
        }

        if(menor==-1){
            tiempo++;
            continue;
        }

        copia[menor].inicio=tiempo;
        copia[menor].fin=tiempo+copia[menor].ejecucion;
        copia[menor].espera=copia[menor].inicio-copia[menor].llegada;
        copia[menor].retorno=copia[menor].fin-copia[menor].llegada;

        tiempo=copia[menor].fin;
        copia[menor].terminado=true;
        completados++;
    }
    promEspera = 0;
promRetorno = 0;

for(int i=0;i<nProcesos;i++){
    promEspera += copia[i].espera;
    promRetorno += copia[i].retorno;
}

promEspera /= nProcesos;
promRetorno /= nProcesos;
}

// ------------------ DIBUJAR TABLA ------------------

void celda(HDC hdc,int x,int y,int w,int h,const char* txt)
{
    Rectangle(hdc,x,y,x+w,y+h);

    SetBkMode(hdc,TRANSPARENT);
    DrawText(hdc,txt,-1,
             new RECT{ x, y, x+w, y+h },
             DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void dibujarTabla(HDC hdc)
{
    int x = 40 + offsetX;
    int y = 100 + offsetY;

    int w = 90;   // ancho celda
    int h = 30;   // alto celda

    // ---- Encabezados
    celda(hdc,x,y,w,h,"Proceso");
    celda(hdc,x+w,y,w,h,"TLL");
    celda(hdc,x+2*w,y,w,h,"TEJ");
    celda(hdc,x+3*w,y,w,h,"Inicio");
    celda(hdc,x+4*w,y,w,h,"Fin");
    celda(hdc,x+5*w,y,w,h,"Espera");
    celda(hdc,x+6*w,y,w,h,"Retorno");

    y+=h;

    // ---- Filas
    for(int i=0;i<nProcesos;i++)
    {
        char b1[30],b2[10],b3[10],b4[10],b5[10],b6[10],b7[10];

        wsprintf(b1,"%s",copia[i].nombre);
        wsprintf(b2,"%d",copia[i].llegada);
        wsprintf(b3,"%d",copia[i].ejecucion);
        wsprintf(b4,"%d",copia[i].inicio);
        wsprintf(b5,"%d",copia[i].fin);
        wsprintf(b6,"%d",copia[i].espera);
        wsprintf(b7,"%d",copia[i].retorno);

        celda(hdc,x,y,w,h,b1);
        celda(hdc,x+w,y,w,h,b2);
        celda(hdc,x+2*w,y,w,h,b3);
        celda(hdc,x+3*w,y,w,h,b4);
        celda(hdc,x+4*w,y,w,h,b5);
        celda(hdc,x+5*w,y,w,h,b6);
        celda(hdc,x+6*w,y,w,h,b7);

        y+=h;
    }
}

// ------------------ WINPROC ------------------

INT_PTR  CALLBACK MainProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hPromedios;
HWND hTabla;
    switch(msg)
    {
        case WM_INITDIALOG:
            cargarArchivo();
            calcularFCFS();
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_BTN_FCFS:
            algoritmo = 1;
            calcularFCFS();
            InvalidateRect(hwnd,NULL,TRUE);
        break;
        case IDC_BTN_GANTT:
    DialogBox(GetModuleHandle(NULL),
              MAKEINTRESOURCE(IDD_GANTT),
              hwnd,
              GanttProc);
break;

        case IDC_BTN_SPN:
            algoritmo = 2;
            calcularSPN();
            InvalidateRect(hwnd,NULL,TRUE);
        break;
            }
        return TRUE;

        case WM_LBUTTONDOWN:
            dragging=true;
            last.x=LOWORD(lParam);
            last.y=HIWORD(lParam);
        return TRUE;

        case WM_MOUSEMOVE:
            if(dragging){
                int mx=LOWORD(lParam);
                int my=HIWORD(lParam);
                offsetX += mx-last.x;
                offsetY += my-last.y;
                last.x=mx;
                last.y=my;
                InvalidateRect(hwnd,NULL,TRUE);
            }
        return TRUE;

        case WM_LBUTTONUP:
            dragging=false;
        return TRUE;
        /*
case WM_ERASEBKGND:
{
    HDC hdc = (HDC)wParam;

    char num[50];
    char txt[100];

    sprintf(num,"%.2f",promEspera);
    wsprintf(txt,"Promedio Espera: %s",num);
    TextOut(hdc,360,20,txt,lstrlen(txt));

    sprintf(num,"%.2f",promRetorno);
    wsprintf(txt,"Promedio Retorno: %s",num);
    TextOut(hdc,360,45,txt,lstrlen(txt));

    dibujarTabla(hdc);

    return TRUE;
}*
case WM_PAINT:
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd,&ps);

    char txt[100];

    wsprintf(txt,"Promedio Espera: %.2f",promEspera);
    TextOut(hdc,300,20,txt,lstrlen(txt));

    wsprintf(txt,"Promedio Retorno: %.2f",promRetorno);
    TextOut(hdc,300,45,txt,lstrlen(txt));

    dibujarTabla(hdc);

    EndPaint(hwnd,&ps);
    return TRUE;wsprintf
}*/
case WM_ERASEBKGND:
{
    HDC hdc = (HDC)wParam;

    char num[50];
    char txt[100];

    sprintf(num,"%.2f",promEspera);
    wsprintf(txt,"Promedio Espera: %s",num);
    TextOut(hdc,360,20,txt,lstrlen(txt));

    sprintf(num,"%.2f",promRetorno);
    wsprintf(txt,"Promedio Retorno: %s",num);
    TextOut(hdc,360,45,txt,lstrlen(txt));

    return TRUE;
}case WM_PAINT:
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd,&ps);

    // 🚨 LE DECIMOS QUE NO BORRE NADA
    SetBkMode(hdc, TRANSPARENT);

    dibujarTabla(hdc);

    EndPaint(hwnd,&ps);
    return TRUE;
}

        case WM_CLOSE:
            EndDialog(hwnd,0);
        return TRUE;
    }
    return FALSE;
}

// ------------------ WINMAIN ------------------

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int)
{
    DialogBox(hInst,
              MAKEINTRESOURCE(IDD_MAIN),
              NULL,
              MainProc);
    return 0;
}
