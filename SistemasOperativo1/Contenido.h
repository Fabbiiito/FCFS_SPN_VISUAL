#pragma once
#include <windows.h>

void CreateContenido(HWND hDlg);

extern HWND hBasics;
extern HWND hStackArray;
extern HWND hStackLinked;
extern HWND hNoExpulsivas;
extern HWND hFCFS;
extern HWND hSPN;

extern HWND hExpulsivas;
extern HWND hSRT;
extern HWND hRR;

void OcultarMenuPrincipal();
void MostrarMenuPrincipal();
