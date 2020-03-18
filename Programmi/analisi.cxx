#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include "statistica.h"
using namespace std;

double kgpeso_to_newton(double kg_peso);

int main()
{
    string dir_ac, sub_dir, alto_basso;
    double t;
    vector<double> media_1_acq, dstd_1_acq;
    string numero;
    double media_vett;
    vector<double> dati_file; // creo un vettore dove immagazzinare i 3 dati per ogni foglio e faccio fare la media con libreria
                              // vector<double> forza_newton;
    vector<double> delta_forza_newton;
    for (int i = 200; i < 1300; i = i + 100)
    {
        //forza_newton.push_back(kgpeso_to_newton(i*4/1000)); //Automaticamente converte in Newton i grammi peso
        delta_forza_newton.push_back(kgpeso_to_newton((i - 200) * 4 / 1000)); //Delta_F per l'asse x
    }

    cout << "Inserire le cartella che vuoi leggere (1ac / 2ac): ";
    cin >> dir_ac;
    if (dir_ac == "1ac") //legge 1 acquisizione
    {

        cout << "Vuoi legge i file di Allungamento o Accorciamento? (all / acc): ";
        cin >> sub_dir;

        for (int i = 200; i < 1300; i = i + 100) //legge tutti i file 200->1200
        {
            numero = to_string(i); //trasformo (i) in una stringa chiamata numero
            ifstream fin("../" + dir_ac + "/" + sub_dir + "/" + numero + ".txt");
            if (!fin) //controllo lettura
            {
                cout << "Errore di lettura" << endl;
                return 1;
            }
            while (fin >> t)
            {
                dati_file.push_back(t * 10); //Vengono convertiti tutti in micron. I dati sono presi in 1e-5m
            }
            media_1_acq.push_back(media(dati_file));
            dstd_1_acq.push_back(dstd_media(dati_file));
        }
        for (auto d : dstd_1_acq) //Se l'errore è più piccolo della distrib triangolare, sostituire la dev std con quella della triangolare. Previene avere errori nulli
        {
            if (d < sigma_dist_tri(10, 10))
            {
                d = sigma_dist_tri(10, 10);
            }
        }

        //Output dati grafico
        ofstream fout("grafico_" + sub_dir + ".txt");
        if (!fout)
        {
            cout << "Errore scrittura";
            return 1;
        }
        for (int i = 0; i < media_1_acq.size(); i++)
        {
            cout << delta_forza_newton[i] << "\t" << media_1_acq[i] << "\t" << dstd_1_acq[i] << endl;
            fout << "#DeltaForza[N]\tDelta_x[1e-6m]\tDstd[1e-6m]" << endl; //Stampa descrizione colonne per rendere migliore la lettura del file all'utilizzatore finale
            fout << delta_forza_newton[i] << "\t" << media_1_acq[i] << "\t" << dstd_1_acq[i] << endl;
        }

        //Primo approccio calcolo di K (tramite interpolazione)
        cout << "Coeff ang:\t" << b_angolare(delta_forza_newton, media_1_acq) << "+/-" << sigma_b(delta_forza_newton, media_1_acq) << endl;
        cout << "Intercetta:\t" << a_intercetta(delta_forza_newton, media_1_acq) << "+/-" << sigma_a(delta_forza_newton, media_1_acq) << endl;
    }

    if (dir_ac == "2ac") //legge 2 acquisizione QUasi sicuramente si può muovere in un altro file
    {
        vector<double> temp;
        vector<double> media_2_acq, dstd_2_acq;
        cout << "Vuoi legge i file di 400 o 1000? (400/1000): ";
        cin >> sub_dir;

        cout << "Vuoi leggere i file dall'alto o dal basso? (alto / basso): ";
        cin >> alto_basso;

        ifstream fin("../" + dir_ac + "/" + sub_dir + "/" + sub_dir + "_" + alto_basso + ".txt");

        if (!fin)
        {
            cout << "Errore di lettura" << endl;
            return 1;
        }

        while (fin >> t)
        {
            temp.push_back(t);
        }

        for (int i = 0; i <= temp.size() - 3; i = i + 3)
        {
            media_2_acq.push_back(media(temp, i, i + 3));
            dstd_2_acq.push_back(dstd_media(temp, i, i + 3));
        }

        for (auto c : media_2_acq)
        {
            cout << c << endl;
        }
    }

    return 0;
}

double kgpeso_to_newton(double kg_peso)
{
    return 9.806 * kg_peso;
}