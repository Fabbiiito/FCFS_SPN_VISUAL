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



    HWND hExpulsivas;
    HWND hSRT;
    HWND hRR;


    int vistaActual = 0;
    HWND hBtnVolver = NULL;
   void CreateContenido(HWND hDlg)
{
    // =========================
    // NO EXPULSIVAS
    // =========================
    hNoExpulsivas = CreateWindowEx(
        0, "STATIC", "• planificación No Expulsivas",
        WS_CHILD | WS_VISIBLE | SS_NOTIFY,
        200, 190, 350, 20,
        hDlg, (HMENU)(INT_PTR)IDC_LINK_no_expulsivas,
        NULL, NULL
    );

    hFCFS = CreateWindowEx(
        0, "STATIC", "o FCFS",
        WS_CHILD | WS_VISIBLE,
        220, 220, 300, 20,
        hDlg, NULL, NULL, NULL
    );

    hSPN = CreateWindowEx(
        0, "STATIC", "o SPN",
        WS_CHILD | WS_VISIBLE,
        220, 245, 300, 20,
        hDlg, NULL, NULL, NULL
    );

    // =========================
    // EXPULSIVAS
    // =========================
    hExpulsivas = CreateWindowEx(
        0, "STATIC", "• planificación Expulsivas",
        WS_CHILD | WS_VISIBLE | SS_NOTIFY,
        200, 290, 350, 20,
        hDlg, (HMENU)(INT_PTR)IDC_LINK_expulsivas,
        NULL, NULL
    );

    hSRT = CreateWindowEx(
        0, "STATIC", "o SRT (Shortest Remaining Time)",
        WS_CHILD | WS_VISIBLE,
        220, 320, 300, 20,
        hDlg, NULL, NULL, NULL
    );

    hRR = CreateWindowEx(
        0, "STATIC", "o RR (Round Robin)",
        WS_CHILD | WS_VISIBLE,
        220, 345, 300, 20,
        hDlg, NULL, NULL, NULL
    );

    // =========================
    // BASICS
    // =========================
    hBasics = CreateWindowEx(
        0, "STATIC", "• texto 2",
        WS_CHILD | WS_VISIBLE,
        200, 395, 300, 20,
        hDlg, (HMENU)(INT_PTR)2001,
        NULL, NULL
    );

    hStackArray = CreateWindowEx(
        0, "STATIC", "o subtexto1",
        WS_CHILD | WS_VISIBLE | SS_NOTIFY,
        220, 425, 350, 20,
        hDlg, (HMENU)(INT_PTR)IDC_LINK_STACK_ARRAY,
        NULL, NULL
    );

    hStackLinked = CreateWindowEx(
        0, "STATIC", "o subtexto2",
        WS_CHILD | WS_VISIBLE | SS_NOTIFY,
        220, 450, 350, 20,
        hDlg, (HMENU)(INT_PTR)IDC_LINK_STACK_LINKED,
        NULL, NULL
    );
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

        ShowWindow(hExpulsivas, SW_HIDE);
ShowWindow(hSRT, SW_HIDE);
ShowWindow(hRR, SW_HIDE);


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

        ShowWindow(hExpulsivas, SW_SHOW);
ShowWindow(hSRT, SW_SHOW);
ShowWindow(hRR, SW_SHOW);

    }
