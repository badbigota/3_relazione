#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

#include "statistica.h"

using namespace std;

int main()
{
    string dir_ac, sub_dir, alto_basso;
    double t;
    vector<double> media_1_acq, dstd_1_acq;
    string numero;
    double media_vett;
    vector<double> dati_file; // creo un vettore dove immagazzinare i 3 dati per ogni foglio e faccio fare la media con libreria
    vector<double> dinamometro;
    for (int i = 0; i < 1600; i = i + 100)
    {
        dinamometro.push_back(i * 4);
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
                dati_file.push_back(t);
            }
            media_1_acq.push_back(media(dati_file));
            dstd_1_acq.push_back(dstd_media(dati_file));
        }
        ofstream fout("grafico.txt");

        for (int i = 0; i < media_1_acq.size(); i++)
        {
            cout << dinamometro[i] << "\t" << media_1_acq[i] << "\t" << dstd_1_acq[i] << endl;
            fout << "#Forza[g_peso]\tDelta_x[1e-6m]\tDstd[1e-6m]" << endl; //Stampa descrizione colonne per rendere migliore la lettura del file all'utilizzatore finale
            fout << dinamometro[i] << "\t" << media_1_acq[i] << "\t" << dstd_1_acq[i] << endl;
        }
        cout << "Coeff angolare: " << b_angolare(dinamometro, media_1_acq) << "+-" << sigma_b(dinamometro, media_1_acq) << endl;
        cout << "Intercetta: " << a_intercetta(dinamometro, media_1_acq) << "+-" << sigma_a(dinamometro, media_1_acq) << endl;
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
