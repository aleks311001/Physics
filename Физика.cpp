
#include "TXLib.h"

struct CBall
    {
    double x = 3, y = 0;
    double vx = 0, vy = 0;
    double dt = 0, rad = 10;
    double massa = 0;
    };

const double XWindow = GetSystemMetrics (SM_CXSCREEN), YWindow = GetSystemMetrics (SM_CYSCREEN);
void kolba ();
bool peregorodka ();
bool peregorodka2();
void Manometr (double davlenie, double x, double y, COLORREF Color, const char * tekst);
void Molek (CBall* Mol, double x);
void kart (CBall *Mol, HDC Molek);
//void podschet (int k, CBall Mol[], HDC Molekula, int *davlen);
void Dlya_Graf ();
void Izmenenie (CBall *Mol1, CBall *Mol2);
void fizic ();
void DvigStena (int *X_Sten);
void Grafik (double t_s, double davlen_s, double t_n, double davlen_n);
void Vipusk (int* i, int maxsI, CBall Mol[]);
void vzriv (int Zoom, double x, double y);
void text (const char * text, double x, double y, COLORREF Color, int vis, int shir);
void Mass2 (CBall Mol[], int K);
void Mass1 (CBall Mol[], int N);
void Clear (double x1, double y1, double x2, double y2);
double mod (double x);
double Dist (double x1, double y1, double x2, double y2);
double temp (double x1, double y1);
void Ottalkiv (int n, CBall Mol[], double * temper);
double pX (CBall Mol);
double pY (CBall Mol);
void osi ();

int main()
    {
    _txWindowStyle &= ~ WS_CAPTION;

    txCreateWindow (XWindow, YWindow);

    fizic ();

    return 0;
    }

void fizic ()
    {
    //txSetColor (TX_WHITE);
    //txClear ();
    int i = 0, k = 0;
    CBall  Mol [3200] = {};
    CBall Mol2 [3200] = {};

    txTextCursor (false);

    double time = 0;

    int davlenK = 0, davlenK_s = 0;
    int davlenN = 0, davlenN_s = 0;
    int K = 0, N = 0;
    int t = 76;
    double X_Sten = XWindow / 2, X_Sten_S = XWindow / 2;
    double temper = 0;
    double temper_s = 0;
    int Zoom = 0;
    int interv = 0;
    int l1 = 0, l2 = 0;

    Dlya_Graf ();

    HDC Molekula  = txLoadImage ("Image//Молекула гелия.bmp");
    HDC Molekula2 = txLoadImage ("Image//Молекула 3.bmp");

    Mass1 (Mol , N);
    Mass2 (Mol2, K);

    osi ();

    text ("Для помощи нажмите I", XWindow / 2 + 30, YWindow - 40, TX_WHITE, 30, 8);

    while (!GetAsyncKeyState (VK_ESCAPE))
        {
        txBegin ();

        kolba ();

        //txSetColor (TX_WHITE);
        //Clear (XWindow - 510, YWindow - 90, XWindow - 120, YWindow - 10);

        txSetColor (TX_WHITE, 7);
        txLine (X_Sten, 250, X_Sten, YWindow);
        if (GetAsyncKeyState (VK_RIGHT)) X_Sten ++;
        if (GetAsyncKeyState (VK_LEFT))  X_Sten --;
        else if (X_Sten < 200 && i + k > 150) X_Sten += 0.25;
        if (X_Sten > XWindow / 2) X_Sten = XWindow / 2;
        if (X_Sten < 125) X_Sten = 125;

        Ottalkiv (i, Mol, &temper);
        Ottalkiv (k, Mol2, &temper);

        for (K = 0; K < k; K++)
            {
            Molek (&Mol2 [K], X_Sten);
            kart (&Mol2 [K], Molekula2);
            if (Mol2[K].y - Mol2[K].rad == 250)davlenK += 2;
            temper += temp (Mol2 [K].vx, Mol2 [K].vy);
            if (GetAsyncKeyState ('G')) Mol2 [K].vy -= 0.05;
            }

        for (N = 0; N < i; N++)
            {
            Molek (&Mol [N], X_Sten);
            kart (&Mol [N], Molekula);
            if (Mol[N].y - Mol[N].rad == 250) davlenN ++;
            temper += temp (Mol [N].vx, Mol [N].vy);
            if (GetAsyncKeyState ('G')) Mol [N].vy -= 0.05;
            }

        if (t > 75)
            {
            Manometr (davlenK, XWindow / 2 - 120, 110, TX_BLACK, "Давление оранжевых:");
            Manometr (davlenN, XWindow / 2 - 360, 110, TX_BLACK, "Давление зеленых:");
            Grafik (X_Sten_S + XWindow / 2 - 50/*XWindow / 2 + time + 2*/, temper_s / 50,
                    X_Sten +   XWindow / 2 - 50/*XWindow / 2 + time + 2 + 0.05 * 75*/, temper / 50);
            t = 0;
            davlenK_s = davlenK;
            davlenN_s = davlenN;
            temper_s = temper;
            X_Sten_S = X_Sten;
            davlenK = 0; davlenN = 0;
            }

        if (temper > 70000)
            {
            while (Zoom < 2500)
                {
                vzriv (Zoom, X_Sten / 2, YWindow / 2);
                Zoom += 10;
                txSleep (0);
                }
            text ("Максимальная температура превышена :(", XWindow / 2 - 500, YWindow / 2, RGB(0, 255, 0), 50, 25);
            //txMessageBox ("Вы взорвались!", ":(  :(", 2);
            /*if (txMessageBox ("Вы взорвались!", ":(  :(", 2) == IDABORT) _txExit = true;*/ break;
            //if (txMessageBox ("Вы взорвались!", ":(  :(", 2) == IDABORT)
            }

        Manometr (i + k, 120, 110, TX_BLACK, "Количество:");

        if (temper > 50000)
            {
            text ("!!!Температура очень высокая!!!", XWindow - 500, YWindow - 40, RGB(255, 0, 0), 25, 12);
            Manometr (temper, 360, 110, RGB ((temper - 50000) * 255 / 20000, 0, 0), "Температура:");
            }

        else
            {
            Manometr (temper, 360, 110, TX_BLACK, "Температура:");
            }

        if (i == 0) if (peregorodka() == true) i += 1;

        if (i > 0 && i < 3200) if (peregorodka() == true && l1 > 2)
            {
            i += 1;
            l1 = 0;
            }

        if (k == 0) if (peregorodka2() == true) k += 1;

        if (k > 0 && k < 3200) if (l2 > 4 && peregorodka2() == true)
            {
            k += 1;
            l2 = 0;
            }

        if (GetAsyncKeyState ('R'))
            {
            i = 0, k = 0;
            Mass1 (Mol, N);
            Mass2 (Mol2, K);
            }

        if (GetAsyncKeyState ('W') && i > 0)
            {
            i--;
            Mol [i].x = 0;
            Mol [i].y = 325;
            }

        if (GetAsyncKeyState ('X') && k > 0)
            {
            k--;
            Mol [k].x = 0;
            Mol [k].y = YWindow - 75;
            }

        if (GetAsyncKeyState ('I'))
            {
            text ("Нажмите A что-бы добавить молекулу 1 вида",     30, YWindow / 2 - 90, TX_WHITE, 30, 8);
            text ("Нажмите D что-бы добавить молекулу 2 вида",     30, YWindow / 2 - 55, TX_WHITE, 30, 8);
            text ("Нажмите W что-бы удалить  молекулу 1 вида",     30, YWindow / 2 - 20, TX_WHITE, 30, 8);
            text ("Нажмите X что-бы удалить  молекулу 2 вида",     30, YWindow / 2 + 20, TX_WHITE, 30, 8);
            text ("Нажмите R что-бы удалить  молекулы всех видов", 30, YWindow / 2 + 55, TX_WHITE, 30, 8);
            text ("Движение перегородки стрелками",                30, YWindow / 2 + 90, TX_WHITE, 30, 8);
            text ("Нажмите G и включится гравитация",              30, YWindow / 2 + 125, TX_GREEN, 30, 8);
            }

        l1 ++;
        l2 ++;
        time += 0.05;
        t++;
        interv++;
        temper = 0;
        txEnd ();
        }

    txDeleteDC (Molekula);
    txDeleteDC (Molekula2);
    }

void kolba ()
    {
    txSetFillColor (TX_BLACK);
    txSetColor (TX_BLACK);
    POINT kolb [4] = {{0, YWindow}, {0, 250}, {XWindow/2, 250}, {XWindow/2, YWindow}};
    txPolygon (kolb, 4);

    txSetColor (TX_WHITE/*RGB(0, 0, 255)*/, 7);
    txLine (XWindow/2, 250,     XWindow/2, YWindow);
    txLine (XWindow/2, YWindow, 0,         YWindow);
    txLine (0,         YWindow, 0,         YWindow - 50);
    txLine (0,         YWindow - 100, 0,   350);
    txLine (0,         300,     0,         250);
    txSetColor (TX_WHITE, 10);
    txLine (0,         250,     XWindow/2 - 3, 250);

    txSetColor (RGB(150, 150, 150), 10);
    if (peregorodka() == false) txLine (0, 300, 0, 350);

    if (peregorodka2() == false) txLine (0, YWindow - 100, 0, YWindow - 50);

    // Manometr
    }

void Manometr (double davlenie, double x, double y, COLORREF Color, const char * tekst)
    {
    txSetFillColor (TX_WHITE);
    txSetColor (TX_WHITE, 7);
    txRectangle (x + 25, y + 140, x - 25, y + 90);
    txSetFillColor (Color);
    txCircle (x, y, 95);

    txSetColor (TX_WHITE);

    char text [10] = "";
    sprintf (text, "%lg", davlenie);
    txSetTextAlign (TA_CENTER);
    txSelectFont ("Arial", 60, 17, 1000, false, false, false, 0);
    txTextOut (x, y - 12, text);
    txSelectFont ("Arial", 30, 7,  800, false, false, false, 0);
    txTextOut (x, y - 45, tekst);
    }

bool peregorodka ()
    {
    if (GetAsyncKeyState ('A')) return true;
        else                    return false;
    }

bool peregorodka2()
    {
    if (GetAsyncKeyState ('D')) return true;
        else                           return false;
    }

void Molek (CBall* Mol, double x)
    {
    Mol->x += Mol->vx * Mol->dt;
    Mol->y += Mol->vy * Mol->dt;

    if (Mol->x + Mol->rad > x)
        {
        Mol->x = x - Mol->rad;
        Mol->vx = -Mol->vx;
        if (GetAsyncKeyState (VK_LEFT) && x > 126 && x < XWindow / 2 - 100)
            {
            Mol->vx += 0.4;
            }
        if (GetAsyncKeyState (VK_RIGHT) && x < XWindow / 2 - 100 && x > 126)
            {
            Mol->vx -= 0.4;
            }
        }
    if (Mol->x - Mol->rad < 0)
        {
        Mol->x = Mol->rad;
        Mol->vx = -Mol->vx;
        //Mol->vy = 2 * Mol->vy;
        }
    if (Mol->y + Mol->rad > YWindow)
        {
        Mol->y = YWindow - Mol->rad;
        Mol->vy = -Mol->vy;
        }
    if (Mol->y - Mol->rad < 250)
        {
        Mol->y = 250 + Mol->rad;
        Mol->vy = -Mol->vy;
        }

    //if (Mol->vx > 0.2) Mol->vx -= 0.002;
    //if (Mol->vy > 0.2) Mol->vy -= 0.002;
    }

void kart (CBall* Mol, HDC Molek)
    {
    txSetColor (RGB (random (25, 225), random (25, 225), random (25, 225)));
    txSetFillColor (RGB (random (25, 225), random (25, 225), random (25, 225)));
    //txCircle (Mol->x, Mol->y, Mol->rad);
    txTransparentBlt (txDC (), Mol->x - Mol->rad, Mol->y - Mol->rad, Mol->rad * 2, Mol->rad * 2, Molek, 0, 0, TX_WHITE);
    }

void osi ()
    {
    txSetColor (TX_WHITE, 3);
    txLine (XWindow / 2,      YWindow - 50, XWindow - 10,     YWindow - 50);
    txLine (XWindow / 2 + 15, 10,            XWindow / 2 + 15, YWindow - 35);
    txLine (XWindow - 10,     YWindow - 50, XWindow - 40,     YWindow - 65);
    txLine (XWindow - 10,     YWindow - 50, XWindow - 40,     YWindow - 35);
    txLine (XWindow / 2 + 15, 10,            XWindow / 2 + 30, 40);
    txLine (XWindow / 2 + 15, 10,            XWindow / 2,      40);
    text ("Объем", XWindow - 100, YWindow - 40, TX_WHITE, 30, 8);
    txSetColor (TX_WHITE);
    txSelectFont ("Comic Sans MS", 30, 8, 0, false, false, false);
    txTextOut (XWindow / 2 + 35, 8, "Температура");
    }

void Dlya_Graf ()
    {
    txSetFillColor (TX_BLACK);
    POINT graf [4] = {{XWindow/2, YWindow}, {XWindow/2, 0}, {XWindow, 0}, {XWindow, YWindow}};
    txPolygon (graf, 4);
    txSetColor (TX_WHITE/*RGB(0, 0, 255)*/, 7);
    txLine (XWindow,   0,     XWindow,   YWindow);
    txLine (XWindow,   YWindow, XWindow/2, YWindow);
    txLine (XWindow/2, YWindow, XWindow/2, 0);
    txLine (XWindow/2, 0,     XWindow,   0);
    }

void Grafik (double t_s, double davlen_s, double t_n, double davlen_n)
    {
    txSetColor (TX_ORANGE, 3);
    txLine (t_s, -davlen_s + YWindow - 50, t_n, -davlen_n + YWindow - 50);
    }

void DvigStena (int *X_Sten)
    {
    if (GetAsyncKeyState (VK_RIGHT)) *X_Sten ++;
    if (GetAsyncKeyState (VK_LEFT))  *X_Sten --;
    txSetColor (RGB(0, 0, 255), 7);
    txLine (*X_Sten, 250, *X_Sten, YWindow);
    if (*X_Sten > XWindow / 2) *X_Sten = XWindow / 2;
    if (*X_Sten < 50) *X_Sten = 50;
    }

double temp (double x1, double y1)
    {
    return sqrt (x1 * x1 + y1 * y1);
    }

void vzriv (int Zoom, double x, double y)
    {
    POINT Zvezda [10] = {{x - 0.5 * Zoom, y - Zoom       }, {x,              y - 3 * Zoom}, {x + 0.5 * Zoom, y - Zoom}, {x + 2.5 * Zoom, y - Zoom    },
                         {x + 0.7 * Zoom, y + 0.25 * Zoom}, {x + 1.5 * Zoom, y + 2 * Zoom}, {x,              y + Zoom}, {x - 1.5 * Zoom, y + 2 * Zoom},
                         {x - 0.7 * Zoom, y + 0.25 * Zoom}, {x - 2.5 * Zoom, y - Zoom}};
    txSetFillColor (TX_RED);
    txPolygon (Zvezda, 10);
    }

void text (const char * text, double x, double y, COLORREF Color, int vis, int shir)
    {
    txSetColor (Color);
    txSelectFont ("Comic Sans MS", vis, shir, 0, false, false, false, 0);
    txTextOut (x, y, text);
    }

void Mass1 (CBall Mol[], int N)
    {
    for (N = 0; N < 3200; N++)
        {
        Mol [N].x = 0;
        Mol [N].y = 325;
        Mol [N].vx = random (2, 5);
        Mol [N].vy = random (-2, 2);
        Mol [N].dt = 2;
        Mol [N].massa = random (0.95, 1.05);
        }
    }

void Mass2 (CBall Mol[], int K)
    {
    for (K = 0; K < 3200; K++)
        {
        Mol [K].x = 0;
        Mol [K].y = YWindow - 75;
        Mol [K].vx = random (1, 3);
        Mol [K].vy = random (-1, 1);
        Mol [K].dt = 2;
        Mol [K].rad = 15;
        Mol [K].massa = random (1.45, 1.55);
        }
    }

void Clear (double x1, double y1, double x2, double y2)
    {
    POINT CLear [4] {{x1, y1}, {x2, y1}, {x2, y2}, {x1, y2}};
    txPolygon (CLear, 4);
    }

double Dist (double x1, double y1, double x2, double y2)
    {
    return sqrt ((x2- x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }

void Ottalkiv (int n, CBall Mol[], double * temper)
    {
    for (int i = 0; i < n; i++)
        {
        for (int k = 0; k < n; k++)
            {
            if (mod (Mol [i].x - Mol [k].x) <= Mol [i].rad + Mol [k].rad + 3 && mod (Mol [i].y - Mol [k].y) <= Mol [i].rad + Mol [k].rad + 3)
                {
                if (Dist (Mol [k].x, Mol [k].y, Mol [i].x, Mol [i].y) <= Mol [i].rad + Mol [k].rad + 3)
                    {
                    Izmenenie (&Mol[k], &Mol[i]);
                    *temper ++;
                    }
                }
            }
        }
    }

void Izmenenie (CBall *Mol1, CBall *Mol2)
    {
    double vx = Mol1->vx, vy = Mol1->vy;
    Mol1->vx = Mol2->vx;//pX (*Mol2) / Mol1->massa;
    Mol1->vy = Mol2->vy;//pY (*Mol2) / Mol1->massa;
    Mol2->vx = vx;//pX (*Mol1) / Mol2->massa;
    Mol2->vy = vy;//pY (*Mol1) / Mol2->massa;
    Mol2->x -= Mol2->vx * 1.5;
    Mol1->x -= Mol1->vx * 1.5;
    Mol2->y -= Mol2->vy * 1.5;
    Mol1->y -= Mol1->vy * 1.5;
    }

/*void podschet (int k, CBall Mol[], HDC Molekula, int *davlen)
    {
    int K = 0;
    while (K < k)
        {
        Molek2 (&Mol [K]);
        kart (&Mol [K], Molekula);
        if (Mol[K].y - Mol[K].rad == 250)*davlen ++;
        K++;
        }
    }

void Vipusk (int* i, int maxsI, CBall Mol[])
    {
    if (*i == 0) if (peregorodka() == true) *i ++;
    if (*i > 0 && *i < maxsI) if (Mol [*i - 1].x > 20 && peregorodka() == true) *i ++;
    }
      */

double pX (CBall Mol)
    {
    return Mol.vx * Mol.massa;
    }

double pY (CBall Mol)
    {
    return Mol.vy * Mol.massa;
    }

double mod (double x)
    {
    if (x > 0) return x;
    else       return -x;
    }
