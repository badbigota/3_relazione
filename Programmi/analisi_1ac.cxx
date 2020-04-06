#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include "statistica1.h"
using namespace std;

double kgpeso_to_newton(double kg_peso)
{
    return 9.806 * kg_peso;
}

int main()
{
    string dir_ac, sub_dir, alto_basso;
    double t;
    string numero;

    vector<double> delta_forza_newton; //Unico vettore che serve per entrambe le esperienze
    for (double i = 200; i < 1300; i = i + 100)
    {
        delta_forza_newton.push_back(kgpeso_to_newton((i - 200) * 4 / 1000)); //Delta_F per l'asse x
    }

    dir_ac = "1ac"; //Legge automaticamente la prima acquisizione

    vector<double> media_1_acq, dstd_1_acq;
    cout << "Vuoi legge i file di Allungamento o Accorciamento? (all / acc): ";
    cin >> sub_dir;
    ofstream excelout("../operatorimisure.txt", ios::app);
    for (int i = 200; i < 1300; i = i + 100) //legge tutti i file 200->1200
    {
        vector<double> dati_file; // creo un vettore dove immagazzinare i 3 dati per ogni foglio e faccio fare la media con libreria
        numero = to_string(i);    //trasformo (i) in una stringa chiamata numero
        ifstream fin("../" + dir_ac + "/" + sub_dir + "/" + numero + ".txt");
        if (!fin) //controllo lettura
        {
            cout << "Errore di lettura" << endl;
            return 1;
        }
        while (fin >> t)
        {
            dati_file.push_back(t * 10.0); //Vengono convertiti tutti in micron. I dati sono letti come 1e-5m
        }
        media_1_acq.push_back(media(dati_file));
        dstd_1_acq.push_back(dstd_media(dati_file));
        excelout << dati_file[0]<<"\t"<<dati_file[1]<<"\t"<<dati_file[2]<<"\t"<<media(dati_file) << "\t" << dstd(dati_file) << "\t" << dstd_media(dati_file) << endl;
    }

    for (int i = 0; i < dstd_1_acq.size(); i++)
    {
        if (dstd_1_acq[i] < sigma_dist_uni(10, 10))
        {
            dstd_1_acq[i] = sigma_dist_uni(10, 10);
        }
    }

    cout << "POST\tERR_NS\tDIST_UNI" << endl;
    for (int i = 0; i < dstd_1_acq.size(); i++)
    {
        cout << sigma_y_posteriori(delta_forza_newton, media_1_acq) << "\t" << dstd_1_acq[i] << endl;
    }
    vector<double> errori_y(dstd_1_acq.size(), sigma_y_posteriori(delta_forza_newton, media_1_acq));

    //Output dati grafico
    ofstream fout("../Grafici/1ac_all_acc/grafico_" + sub_dir + ".txt");
    ofstream dout("comp_1ac_correz_dist_uni.txt", ios::app);
    ofstream sout("comp_1ac_sigma_post.txt", ios::app);

    if (!fout)
    {
        cout << "Errore scrittura";
        return 1;
    }
    fout << "#DeltaForza[N]\tDelta_x[1e-6m]\tDstd(y_post)[1e-6m]" << endl; //Stampa descrizione colonne per rendere migliore la lettura del file all'utilizzatore finale
    for (int i = 0; i < media_1_acq.size(); i++)                           //usiamo sigma post perchè più gramde di dev std (anche dopo confronto con dist uni)
    {
        fout << delta_forza_newton[i] << "\t" << media_1_acq[i] << "\t" << sigma_y_posteriori(delta_forza_newton, media_1_acq) << endl;
    }
    //Primo approccio calcolo di K (tramite interpolazione)
    cout << "Err.Piccoli (sottostima) Coeff ang:\t" << b_angolare(delta_forza_newton, media_1_acq, dstd_1_acq) << " +/- " << sigma_b(delta_forza_newton, media_1_acq, dstd_1_acq) << endl;
    cout << "Sig. Post. (funzioni senza err) Coeff ang:\t" << b_angolare(delta_forza_newton, media_1_acq) << " +/- " << sigma_b(delta_forza_newton, media_1_acq) << endl;

    cout << "Err.Piccoli (sottosti Intercetta:\t" << a_intercetta(delta_forza_newton, media_1_acq, dstd_1_acq) << " +/- " << sigma_a(delta_forza_newton, media_1_acq, dstd_1_acq) << endl;
    cout << "Sig. Post. (funzioni  Intercetta:\t" << a_intercetta(delta_forza_newton, media_1_acq) << " +/- " << sigma_a(delta_forza_newton, media_1_acq) << endl;
    cout<<"COmpatibilità intercetta con origine"<<comp_3(a_intercetta(delta_forza_newton, media_1_acq), 0, sigma_a(delta_forza_newton, media_1_acq), sigma_y_posteriori(delta_forza_newton, media_1_acq))<<endl;

    //Stampa K quando sosttostimato e quando non sottostimato con la sigma posteriori
    dout << b_angolare(delta_forza_newton, media_1_acq, dstd_1_acq) << "\t" << sigma_b(delta_forza_newton, media_1_acq, dstd_1_acq) << "\t" << a_intercetta(delta_forza_newton, media_1_acq, dstd_1_acq) << "\t" << sigma_a(delta_forza_newton, media_1_acq, dstd_1_acq) << endl;
    sout << b_angolare(delta_forza_newton, media_1_acq) << "\t" << sigma_b(delta_forza_newton, media_1_acq) << "\t" << a_intercetta(delta_forza_newton, media_1_acq) << "\t" << sigma_a(delta_forza_newton, media_1_acq) << endl;

    //Secondo approccio sbagliato (campione K consecutivi)
    //vector<double> delta_allungamento;
    //vector<double> k_consecutivi;
    //vector<double> k_non_consecutivi;
    //for (int i = 1; i < media_1_acq.size(); i++) //Calcola ogni allungamento di tratto in tratto
    //{
    //    delta_allungamento.push_back(media_1_acq[i] - media_1_acq[i - 1]);
    //}
    //for (int i = 0; i < delta_allungamento.size(); i++) //Calcola tutti i K
    //{
    //    k_consecutivi.push_back(delta_allungamento[i] / kgpeso_to_newton(0.4));
    //}
    //cout << "K consec:\t" << media(k_consecutivi) << "+/-" << dstd_media(k_consecutivi) << endl;
    //
    ////Terzo approccio corretto (campione K non consecutivi)
    //for (int i = 0; i < k_consecutivi.size(); i = i + 2) //Prende i K in posizioni pari
    //{
    //    k_non_consecutivi.push_back(k_consecutivi[i]);
    //}
    //cout << "K NON consec:\t" << media(k_non_consecutivi) << "+/-" << dstd_media(k_non_consecutivi) << endl;

    return 0;
}
