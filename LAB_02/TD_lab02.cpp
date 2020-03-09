// Krystian Bartosik
// bk44266@zut.edu.pl
//   FEDCBA
#define _USE_MATH_DEFINES 
#include <iostream>
#include <fstream>

using namespace std;

double TonProsty(double A, double f, double Fi, double t)
{
    return A * sin(2 * M_PI * f * t + Fi);
}

int Kwantyzacja(double q, double Y)
{
    return Y / (2 / pow(2, q)); // '2' ponieważ amplituda "łączna" to 2 -> 1 w górę i 1 w dół (Amplituda / dokładność = rozdzielczość)
}

int main()
{
    fstream File;
    File.open("C:/Users/Qrystian/Desktop/results.txt", ios::out);

    for (double t = 0.0; t <= 6; t = t + 0.00005)
    {
        //File << t << " " << TonProsty(1.0, 6, 2 * M_PI, t) << endl;
        //File << t << " " << Kwantyzacja(16, TonProsty(1.0, 6, 2 * M_PI, t)) << endl;
        File << t << " " << Kwantyzacja(8, TonProsty(1.0, 6, 2 * M_PI, t)) << endl;
    }

    File.close();
}

