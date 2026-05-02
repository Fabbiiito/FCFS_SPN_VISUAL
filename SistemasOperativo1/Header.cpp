#include "Header.h"
#include "resource.h"

HWND hHeader;
HWND hFooter;

void CreateHeader(HWND hDlg)
{
    hHeader = CreateWindowEx(
        0,"STATIC","so politicas ",
        WS_CHILD|WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE,
        0,0,900,35,
        hDlg,(HMENU)(INT_PTR)IDC_HEADER_TEXT,NULL,NULL);

    hFooter = CreateWindowEx(
        0,"STATIC","Copyright Fabricio Mayta",
        WS_CHILD|WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE,
        0,465,900,35,
        hDlg,(HMENU)(INT_PTR)IDC_FOOTER_TEXT,NULL,NULL);
}
