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

vector<double> media_vett = {0, 0, 0, 0};
vector<double> sigma_media_vett = {0, 0, 0, 0};

//calcolo delta forza in newton
double delta_forza_N_400 = kgpeso_to_newton(((400 - 200) * 4) / 1000);
double delta_forza_N_1000 = kgpeso_to_newton(((1000 - 200) * 4) / 1000);

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

    for (auto d : campioni)
    {
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


    vector <double> k;
    vector <double> sigma_k;

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

    cout << valore_k << endl;

    double sigma_valore_k = errore_media_ponderata(sigma_k);

    cout << sigma_valore_k << endl;

    return 0;
}

double kgpeso_to_newton(double kg_peso)
{
    return 9.806 * kg_peso;
}