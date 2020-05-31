// Krystian Bartosik
// bk44266@zut.edu.pl
//   FEDCBA
#define _USE_MATH_DEFINES 
#include <iostream>
#include <string>
#include <fstream>
#include "math.h"
#include <complex>
#include <cstddef>
#include <bitset>

using namespace std;

string S2BS(const char* s, string Endian)
{
    string result = "";

    if (Endian == "BigEndian")
    {
        for (int j = 0; j < strlen(s); j++)
        {
            result = result + bitset<8>(s[j]).to_string();
        }
    }
    // "test" = 01110100011001010111001101110100

    if (Endian == "LittleEndian")
    {
        for (int j = 0; j < strlen(s); j++)
        {
            result = bitset<8>(s[j]).to_string() + result;
        }
    }

    //cout << result << endl;
    return result;
}

char SB2S(string S)
{
    int X = stoi(S, nullptr, 2);
    return X;
}

int** G_Multiply(int** D) // Przyjmuje 4bitową tablicę
{
    int G[7][4] = { {1,1,0,1},{1,0,1,1},{1,0,0,0},{0,1,1,1},{0,1,0,0},{0,0,1,0},{0,0,0,1} };
    int** C = new int* [8];
    int BitKontrolny = 0;

    for (int j = 0; j < 8; j++)
        C[j] = new int[1];

    for (int j = 0; j < 8; j++)
    {
        C[j][0] = 0;
    }

    for (int j = 0; j < 7; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            //cout << C[j][0] << "+(" << D[0][i]
            C[j][0] = C[j][0] + (D[i][0] * G[j][i]);
        }
        C[j][0] = C[j][0] % 2;
        BitKontrolny = BitKontrolny + C[j][0];
    }

    C[7][0] = BitKontrolny % 2;

    return C;
}

string Hamming_Nadajnik(string S)
{
    cout << "[NAD] Dane do wyslania:  " << S << endl;
    int** C = new int* [4];

    for (int j = 0; j < 4; j++)
    {
        C[j] = new int[1];
    }

    for (int j = 0; j < 4; j++)
    {
        C[j][0] = (int)S[j] - '0'; // Konwersja na liczbę
    }

    int** Result = G_Multiply(C);
    string X = "00000000";

    for (int j = 0; j < 8; j++)
    {
        X[j] = Result[j][0] + 48; // Konwersja na znak
    }
    cout << "[NAD] Nadana wiadomosc:  " << X << endl;
    return X;
}

void Zegar(double Czestotliwosc, double Probkowanie)
{
    fstream File1;
    File1.open("C:/Users/Qrystian/Desktop/results1.txt", ios::out);

    int Check = 0;
    int Bit = 1;
    for (double j = 0; j < Czestotliwosc; j = j + Probkowanie)
    {
        File1 << j << " " << Bit << endl;

        if (Check == 500)
        {
            Check = 0;
            if (Bit == 0)
                Bit = 1;
            else
                Bit = 0;
        }

        Check++;
    }

    File1.close();
}

double TTL(char t)
{
    double tt = (double)t - '0'; // Konwersja na liczbę

    if (tt == 0)
    {
        return 0.0;
    }

    if (tt == 1)
    {
        return 1.0;
    }
}

double zA(double A1, double A2, double f, double Fi, char T, double t)
{
    double tt = (double)T - '0'; // Konwersja na liczbę
    if (tt == 0)
    {
        return A1 * sin(2.0 * M_PI * f * t + Fi);
    }

    if (tt == 1)
    {
        return A2 * sin(2.0 * M_PI * f * t + Fi);
    }
}

double zF(double A, long double f0, double f1, double Fi, char T, double t)
{
    double tt = (double)T - '0'; // Konwersja na liczbę
    if (tt == 0)
    {
        return A * sin(2.0 * M_PI * f0 * t + Fi);
    }

    if (tt == 1)
    {
        return A * sin(2.0 * M_PI * f1 * t + Fi);
    }
}

double zP(double A, double f, double Fi1, double Fi2, char T, double t)
{
    double tt = (double)T - '0'; // Konwersja na liczbę
    if (tt == 0)
    {
        return A * sin(2.0 * M_PI * f * t + Fi1);
    }

    if (tt == 1)
    {
        return A * sin(2.0 * M_PI * f * t + Fi2);
    }
}

string Hamming_Odbiornik(string S)
{
    cout << endl;
    cout << "[ODB] Odebrana wiadomosc:" << S << endl;
    int H[3][7] = { {0,0,0,1,1,1,1},{0,1,1,0,0,1,1},{1,0,1,0,1,0,1} };
    int** C = new int* [8];
    int** Answer = new int* [3];
    int BitParzystosci = 0;

    for (int j = 0; j < 8; j++)
    {
        C[j] = new int[1];
        C[j][0] = (int)S[j] - '0';
        BitParzystosci = BitParzystosci + C[j][0];
    }

    BitParzystosci = BitParzystosci % 2;
    cout << "[ODB] Bit parzystosci:   " << BitParzystosci << endl;

    for (int j = 0; j < 3; j++)
    {
        Answer[j] = new int[1];
        Answer[j][0] = 0;
    }

    cout << "[ODB] Odkodowana macierz:";

    for (int j = 0; j < 3; j++)
    {
        for (int i = 0; i < 7; i++)
        {
            Answer[j][0] = Answer[j][0] + (C[i][0] * H[j][i]);
        }
        Answer[j][0] = Answer[j][0] % 2;
        cout << Answer[j][0];
    }
    cout << endl;

    bool Poprawnosc;
    for (int j = 0; j < 3; j++)
    {
        if (Answer[j][0] == 0)
        {
            Poprawnosc = true;
            continue;
        }
        else
        {
            Poprawnosc = false;
            break;
        }
    }

    if ((Poprawnosc == true) && (BitParzystosci == 0))
    {
        cout << "[ODB] Dane poprawne!" << endl;
        string X = "0000";

        X[0] = C[2][0] + 48;
        X[1] = C[4][0] + 48;
        X[2] = C[5][0] + 48;
        X[3] = C[6][0] + 48;

        cout << "[ODB] Odebrane dane:     " << X << endl << endl;
        return X;
    }

    if ((Poprawnosc == false) && (BitParzystosci == 1))
    {
        cout << "[ODB] Pojedynczy blad!" << endl;
        int Pozycja = 0;
        for (int j = 0; j < 3; j++)
        {
            Pozycja = Pozycja + (Answer[2 - j][0] * pow(2, j));
        }

        cout << "[ODB] Blad na pozycji:   " << Pozycja - 1 << endl;
        if (C[Pozycja - 1][0] == 1)
            C[Pozycja - 1][0] = 0;
        else
            C[Pozycja - 1][0] = 1;

        cout << "[ODB] Blad naprawiony!" << endl;

        string X = "0000";

        X[0] = C[2][0] + 48;
        X[1] = C[4][0] + 48;
        X[2] = C[5][0] + 48;
        X[3] = C[6][0] + 48;

        cout << "[ODB] Odebrane dane:     " << X << endl << endl;
        return X;
    }

    if ((Poprawnosc == false) && (BitParzystosci == 0))
    {
        cout << "[ODB] Podwojny blad!" << endl;
        cout << "[ODB] Pakiet odrzucony!" << endl << endl;
        return "Pakiet uszkodzony";
    }
}

int main()
{
    const char* Napis = "a";
    string S = S2BS(Napis, "BigEndian");
    cout << "Napis: " << Napis << endl << "Ciag binarny: " << S << endl;
    
    string SSS = "0000000000000000";

    int Poz = 0;
    for (int k = 0; k < S.length(); k++)
    {
        if (k % 4 == 0)
        {
            string SS = S.substr(Poz, 4);   
            SS = Hamming_Nadajnik(SS);
            cout << endl;
            
            for (int a = 0; a < 8; a++)
            {
                if (Poz == 0)       
                    SSS[a] = SS[a];
                else
                    SSS[a + 8] = SS[a];
            }

            Poz = Poz + 4;
        }
    }

    cout << "Napis po kodowaniu: " << SSS << endl;
    S = SSS;

    fstream File2;
    fstream File3;
    File3.open("C:/Users/Qrystian/Desktop/results3.txt", ios::out);
    File2.open("C:/Users/Qrystian/Desktop/results2.txt", ios::out);

    Zegar(S.length(), 0.001);
    double Suma = 0;
    int Checker = 0;

    string SS = "0000000000000000";
    int Sj = 0;

    for (double t = 0; t < S.length(); t = t + 0.001)
    {
        File2 << t << " " << TTL(S[floor(t)]) << endl;
        
        /*
        // ASK
        File3 << t << " " << zA(0.0, 1.0, ((double)S.length()/0.001) * pow(1000, -1), 2 * M_PI, S[floor(t)], t) << endl;
        double Y = zA(0.0, 1.0, ((double)S.length() / 0.01) * pow(1000, -1), 2 * M_PI, S[floor(t)], t) * zA(1.0, 1.0, ((double)S.length() / 0.01) * pow(1000, -1), 2 * M_PI, S[floor(t)], t) * 0.01;

        if (Checker == 999) // Co 1000 próbek = co jeden bit
        {
            for (double j = 0.0 + t; j < 1 + t; j = j + 0.01)
            {
                if (Suma > 0.3)   // h=0.3, odzyskanie sygnału
                {
                    SS[Sj] = '1';
                    Sj++;
                    break;
                }                
                else
                {
                    SS[Sj] = '0';
                    Sj++;
                    break;
                }                   
            }
            Suma = 0;
            Checker = 0;
        }
        else
        {
            Suma = Suma + Y; // Sumowanie kolejnych próbek = drugi krok całkowania
            Checker++;
        }
        */

        // PSK
        /*
        File3 << t << " " << zP(1.0, ((double)S.length() / 0.01) * pow(1000, -1), 0.0, M_PI, S[floor(t)], t) << endl;
        double Y = zP(1.0, ((double)S.length() / 0.01) * pow(1000, -1), 0.0, M_PI, S[floor(t)], t) * zP(1.0, ((double)S.length() / 0.01) * pow(1000, -1), M_PI, M_PI, S[floor(t)], t) * 0.01;
        
        if (Checker == 999)
        {
            for (double j = 0.0 + t; j < 1 + t; j = j + 0.01)
            {
                if (Suma > 0)   // h=0, odzyskanie sygnału
                {
                    SS[Sj] = '1';
                    Sj++;
                    break;
                }
                else
                {
                    SS[Sj] = '0';
                    Sj++;
                    break;
                }
            }
            Suma = 0;
            Checker = 0;
        }
        else
        {
            Suma = Suma + Y;
            Checker++;
        }
        */

        
        // FSK
        File3 << t << " " << zF(1.0, 1.0, 5.0, 2 * M_PI, S[floor(t)], t) << endl;
        double x1 = zF(1.0, 1.0, 5.0, 2 * M_PI, S[floor(t)], t) * zF(1.0, 1.0, 1.0, 2 * M_PI, S[floor(t)], t);
        double x2 = zF(1.0, 1.0, 5.0, 2 * M_PI, S[floor(t)], t) * zF(1.0, 5.0, 5.0, 2 * M_PI, S[floor(t)], t);
        double Y = (x2 * 0.01) - (x1 * 0.01);

        if (Checker == 999)
        {
            for (double j = 0.0 + t; j < 1 + t; j = j + 0.01)
            {
                if (Suma > 0)   // h=0, odzyskanie sygnału
                {
                    SS[Sj] = '1';
                    Sj++;
                    break;
                }
                else
                {
                    SS[Sj] = '0';
                    Sj++;
                    break;
                }

            }
            Suma = 0;
            Checker = 0;
        }
        else
        {
            Suma = Suma + Y;
            Checker++;
        }
        
    }

    cout << "Sygnal po demodulacji: " << SS << endl;

    string S1 = SS.substr(0, 8);
    string S2 = SS.substr(8, 8);
    string SS1 = "";
    string SS2 = "";

    SS1 = Hamming_Odbiornik(S1);
    SS2 = Hamming_Odbiornik(S2);

    string Sx = "";
    Sx.append(SS1);
    Sx.append(SS2);

    cout << "Odebrany napis: " << Sx << endl;

    cout << "Odebrana wiadomosc: " << SB2S(Sx);
}
