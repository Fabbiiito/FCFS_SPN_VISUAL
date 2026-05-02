#include "VistaSPN.h"

void DibujarVistaSPN(HDC hdc)
{
    RECT r = {220, 100, 900, 500};
    DrawText(hdc, "ACA VA SPN", -1, &r,
             DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
