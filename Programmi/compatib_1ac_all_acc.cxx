#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "statistica.h"

using namespace std;

int main()
{
    string file1 = "comp_1ac_correz_dist_uni.txt";
    string file2 = "comp_1ac_sigma_post.txt";
    ifstream fin1(file1);
    ifstream fin2(file2);
    double b_an1, si_an1, a_in1, si_in1;
    double b_an2, si_an2, a_in2, si_in2;
    vector<double> b_ang1;
    vector<double> b_ang2;
    vector<double> s_b1;
    vector<double> s_b2;
    vector<double> a_int1;
    vector<double> a_int2;
    vector<double> s_a1;
    vector<double> s_a2;
    while (fin1 >> b_an1 >> si_an1 >> a_in1 >> si_in1)
    {
        b_ang1.push_back(b_an1);
        s_b1.push_back(si_an1);
        a_int1.push_back(a_in1);
        s_a1.push_back(si_in1);
    }
    while (fin2 >> b_an2 >> si_an2 >> a_in2 >> si_in2)
    {
        b_ang2.push_back(b_an2);
        s_b2.push_back(si_an2);
        a_int2.push_back(a_in2);
        s_a2.push_back(si_in2);
    }
    cout << "CON CORREZIONI DI DIST UNI SU DSTD DAI DATI LETTI" << endl;
    cout << "Media ponderata coeff angolare:\t" << media_ponderata(b_ang1, s_b1) << " +/- " << errore_media_ponderata(s_b1) << endl;
    cout << "Media ponderata intercetta:\t" << media_ponderata(a_int1, s_a1) << " +/- " << errore_media_ponderata(s_a1) << endl;
    cout << "Compatibilità fra i 2 K all e acc:\t" << comp_3(b_ang1[0], b_ang1[1], s_b1[0], s_b1[1]) << endl;
    cout << "Compatibilità fra err sistematici all e acc:\t" << comp_3(a_int1[0], a_int1[1], s_a1[0], s_a1[1]) << endl;

    cout << "CON SIGMA A POSTERIORI" << endl;
    cout << "Media ponderata coeff angolare:\t" << media_ponderata(b_ang2, s_b2) << " +/- " << errore_media_ponderata(s_b2) << endl;
    cout << "Media ponderata intercetta:\t" << media_ponderata(a_int2, s_a2) << " +/- " << errore_media_ponderata(s_a2) << endl;
    cout << "Compatibilità fra i 2 K all e acc:\t" << comp_3(b_ang2[0], b_ang2[1], s_b2[0], s_b2[1]) << endl;
    cout << "Compatibilità fra err sistematici all e acc:\t" << comp_3(a_int2[0], a_int2[1], s_a2[0], s_a2[1]) << endl;
}