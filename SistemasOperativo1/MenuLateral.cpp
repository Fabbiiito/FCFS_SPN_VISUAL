#include "MenuLateral.h"
#include "resource.h"

HWND h1 = NULL;
HWND h2 = NULL;
HWND h3 = NULL;

void CreateMenuLateral(HWND hDlg)
{
    h1 = CreateWindowEx(
        0,"STATIC","texto1",
        WS_CHILD|WS_VISIBLE|SS_NOTIFY,
        20,80,120,20,
        hDlg,(HMENU)(INT_PTR)IDC_LINK_1,NULL,NULL);

    h2 = CreateWindowEx(
        0,"STATIC","texto2",
        WS_CHILD|WS_VISIBLE|SS_NOTIFY,
        20,105,120,20,
        hDlg,(HMENU)(INT_PTR)IDC_LINK_2,NULL,NULL);

    h3 = CreateWindowEx(
        0,"STATIC","texto3",
        WS_CHILD|WS_VISIBLE|SS_NOTIFY,
        20,130,120,20,
        hDlg,(HMENU)(INT_PTR)IDC_LINK_3,NULL,NULL);
}
