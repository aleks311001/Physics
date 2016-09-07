#include "TXLib.h"

double XWindow = GetSystemMetrics (SM_CXSCREEN), YWindow = GetSystemMetrics (SM_CYSCREEN);


void Koleso (int x, int y, int x_l, int y_l);

int main ()
    {
    _txWindowStyle &= ~ WS_CAPTION;
    txCreateWindow (XWindow, YWindow);

    for (int i = 0; ; i+= 50)
        {
        txBegin ();
        Koleso (YWindow / 2, YWindow / 2, (YWindow / 2 - 150) * sin (i*txPI/180) + YWindow / 2, (YWindow / 2 - 150) * cos (i*txPI/180) + YWindow / 2);
        txEnd ();
        }
    }

void Koleso (int x, int y, int x_l, int y_l)
    {
    txSetColor (RGB (0, 200, 0));
    txSetFillColor (RGB (0, 200, 0));
    txCircle (x, y, YWindow / 2 - 50);

    txSetColor (RGB (0, 255, 0));
    txSetFillColor (RGB (0, 255, 0));
    txCircle (x, y, YWindow / 2 - 150);

    double Ugol = acos ((x_l - x)/(YWindow / 2 - 50));

    for (int i = 0; i < 36; i++)
        {
        txSetColor (TX_BLACK);
        txLine ((YWindow / 2 - 50)  * cos ((Ugol - 10 * i)*txPI/180) + x, (YWindow / 2 - 50)  * sin ((Ugol - 10 * i)*txPI/180) + y,
                (YWindow / 2 - 150) * cos ((Ugol - 10 * i)*txPI/180) + x, (YWindow / 2 - 150) * sin ((Ugol - 10 * i)*txPI/180) + y);
        }
    }
