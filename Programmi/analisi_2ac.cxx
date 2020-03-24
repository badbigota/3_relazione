#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include "statistica.h"
using namespace std;

double kgpeso_to_newton(double kg_peso);

struct container
{
    string nomefile = "";
    vector<double> temp;
    vector<double> media_2_acq;
    vector<double> dstd_2_acq;
    double media;
    double dstd_media;
    double kappa;
    double sigma_kappa;
    double gp = 0;
};
vector<container> campioni(4);

int main()
{
    double t;
    string numero;
    vector<double> k_alto;
    vector<double> k_basso;
    vector<double> sigma_k_alto;
    vector<double> sigma_k_basso;

    campioni[0].nomefile = "../2ac/400/400_basso.txt";
    campioni[1].nomefile = "../2ac/400/400_alto.txt";
    campioni[2].nomefile = "../2ac/1000/1000_basso.txt";
    campioni[3].nomefile = "../2ac/1000/1000_alto.txt";
    campioni[0].gp = 400;
    campioni[1].gp = 400;
    campioni[2].gp = 1000;
    campioni[3].gp = 1000;

    for (auto &d : campioni) ////
    {
        //PRIMO METODO CALCOLO DI K
        vector<double> temp;
        ifstream fin(d.nomefile);
        if (!fin)
        {
            cout << "Errore di lettura di:" << d.nomefile << endl;
            return 1;
        }
        while (fin >> t)
        {
            temp.push_back(t * 10);
        }
        //metto in un vettore le medie e in un altro vettore le deviazioni standard
        for (int i = 0; i <= temp.size() - 3; i = i + 3)
        {
            d.media_2_acq.push_back(media(temp, i, i + 3));
            d.dstd_2_acq.push_back(dstd_media(temp, i, i + 3));
        }
        //Se l'errore è più piccolo della dev stand , sostituire la dev std con quella dell unifotme.
        for (int i = 0; i < d.dstd_2_acq.size(); i++)
        {
            if (d.dstd_2_acq[i] < sigma_dist_uni(10, 10))
            {
                d.dstd_2_acq[i] = sigma_dist_uni(10, 10);
            }
        }
        double delta_forza = kgpeso_to_newton((d.gp - 200) * 4 / 1000);
        double sigma_delta_forza = sigma_dist_uni(((300 * 100) / (11 * 1000)) * (4 * 9.806 / 1000), 1);
        d.media = media(d.media_2_acq);
        d.dstd_media = dstd_media(d.media_2_acq);
        d.kappa = d.media / delta_forza;
        d.sigma_kappa = sqrt(pow(1 / delta_forza, 2) * (pow(d.dstd_media, 2)) + pow((d.media / pow(delta_forza, 2)), 2) * pow(sigma_delta_forza, 2));
        if (d.nomefile.find("alto") != std::string::npos)
        {
            k_alto.push_back(d.kappa);
            sigma_k_alto.push_back(d.sigma_kappa);
        }
        else if (d.nomefile.find("basso") != std::string::npos)
        {
            k_basso.push_back(d.kappa);
            sigma_k_basso.push_back(d.sigma_kappa);
        }
    }

    vector<double> k;
    vector<double> sigma_k;

    //media ponderata valori di k alto e errore con errore media ponderata
    double media_ponderata_k_alto = media_ponderata(k_alto, sigma_k_alto); //valori k
    k.push_back(media_ponderata_k_alto);

    double sigma_media_ponderata_k_alto = errore_media_ponderata(sigma_k_alto); //errori k
    sigma_k.push_back(sigma_media_ponderata_k_alto);

    //media ponderata valori di k basso e errore con errore media ponderata
    double media_ponderata_k_basso = media_ponderata(k_basso, sigma_k_basso);
    k.push_back(media_ponderata_k_basso);

    double sigma_media_ponderata_k_basso = errore_media_ponderata(sigma_k_basso);
    sigma_k.push_back(sigma_media_ponderata_k_basso);

    //media ponderata delle medie ponderate
    double valore_k = media_ponderata(k, sigma_k);
    double sigma_valore_k = errore_media_ponderata(sigma_k);

    cout << valore_k << " +/- " << sigma_valore_k << " micron/Newton" << endl;

    //SECONDO METODO CALCOLO DI K con 2 chi quadri fra tutti gli 8 punti

    vector<double> chi_basso; //ho messo nel vettore 8 punti di 400basso
    vector<double> chi_alto;
    //unifico 2 vettori per alto
    vector<double> sigma_chi_basso;
    vector<double> sigma_chi_alto;
    vector<double> delta_forza_newton; //Unico vettore che serve per entrambe le esperienze

    for (int i = 0; i < campioni[1].media_2_acq.size(); i++)
    {
        chi_alto.push_back(campioni[1].media_2_acq[i]); //400alto
        chi_alto.push_back(campioni[3].media_2_acq[i]); //1000alto
        sigma_chi_alto.push_back(campioni[1].dstd_2_acq[i]);
        sigma_chi_alto.push_back(campioni[3].dstd_2_acq[i]);
    }
    for (int i = 0; i < campioni[0].media_2_acq.size(); i++)
    {
        delta_forza_newton.push_back(kgpeso_to_newton((200) * 4 / 1000));
        delta_forza_newton.push_back(kgpeso_to_newton((800) * 4 / 1000));
    }
    delta_forza_newton.pop_back();

    for (int i = 0; i < campioni[0].media_2_acq.size(); i++)
    {
        chi_basso.push_back(campioni[0].media_2_acq[i]);      //ho messo nel vettore 400basso
        chi_basso.push_back(campioni[2].media_2_acq[i]);      //1000basso
        sigma_chi_basso.push_back(campioni[0].dstd_2_acq[i]); //la stessa cosa per gli errori
        sigma_chi_basso.push_back(campioni[2].dstd_2_acq[i]);
    }

    cout << "Angolare Basso:\t" << b_angolare(delta_forza_newton, chi_basso, sigma_chi_basso) << " +/- " << sigma_b(delta_forza_newton, chi_basso, sigma_chi_basso) << endl;
    cout << "Intercetta Basso:\t" << a_intercetta(delta_forza_newton, chi_basso, sigma_chi_basso) << " +/- " << sigma_a(delta_forza_newton, chi_basso, sigma_chi_basso) << endl;

    cout << "Angolare Alto:\t" << b_angolare(delta_forza_newton, chi_alto, sigma_chi_alto) << " +/- " << sigma_b(delta_forza_newton, chi_alto, sigma_chi_alto) << endl;
    cout << "Intercetta Alto:\t" << a_intercetta(delta_forza_newton, chi_alto, sigma_chi_alto) << " +/- " << sigma_a(delta_forza_newton, chi_alto, sigma_chi_alto) << endl;

    vector<double> chi;
    chi.push_back(b_angolare(delta_forza_newton, chi_basso, sigma_chi_basso));
    chi.push_back(b_angolare(delta_forza_newton, chi_alto, sigma_chi_alto));

    vector<double> sigma_chi;
    sigma_chi.push_back(sigma_b(delta_forza_newton, chi_basso, sigma_chi_basso));
    sigma_chi.push_back(sigma_b(delta_forza_newton, chi_alto, sigma_chi_alto));

    cout << "K con chi quadro tramite intepolazione 8 punti:\t" << media_ponderata(chi, sigma_chi) << " +/- " << errore_media_ponderata(sigma_chi) << endl;

    return 0;
}

double kgpeso_to_newton(double kg_peso)
{
    return 9.806 * kg_peso;
}