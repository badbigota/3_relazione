#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "statistica.h"

using namespace std;

int main()
{
    string file = "comp_1ac.txt";
    ifstream fin(file);
    double b_an, si_an, a_in, si_in;
    vector<double> b_ang;
    vector<double> s_b;
    vector<double> a_int;
    vector<double> s_a;
    while (fin >> b_an >> si_an >> a_in >> si_in)
    {
        b_ang.push_back(b_an);
        s_b.push_back(si_an);
        a_int.push_back(a_in);
        s_a.push_back(si_in);
    }
    cout << "Media ponderata coeff angolare:\t" << media_ponderata(b_ang, s_b) << " +/- " << errore_media_ponderata(s_b) << endl;
    cout << "Media ponderata intercetta:\t" << media_ponderata(a_int, s_a) << " +/- " << errore_media_ponderata(s_a) << endl;

    cout << "Compatibilità fra i 2 K all e acc:\t" << comp_3(b_ang[0], b_ang[1], s_b[0], s_b[1]) << endl;
    cout << "Compatibilità fra err sistematici all e acc:\t" << comp_3(a_int[0], a_int[1], s_a[0], s_a[1]) << endl;
}