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
  campioni[0].gp = 400.0;
  campioni[1].gp = 400.0;
  campioni[2].gp = 1000.0;
  campioni[3].gp = 1000.0;
  double sigma_delta_forza = sigma_dist_uni(((300.0 * 100.0) / (11.0 * 1000.0)) * (4.0 * 9.806 / 1000.0), 1.0);

  for (auto &d : campioni)
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
    double delta_forza = kgpeso_to_newton((d.gp - 200.0) * 4 / 1000);
    d.media = media_ponderata(d.media_2_acq, d.dstd_2_acq);
    d.dstd_media = errore_media_ponderata(d.dstd_2_acq);
    d.kappa = d.media / delta_forza;
    //cout << "->" << d.media << "+/-" << d.dstd_media << endl;
    //d.sigma_kappa = sqrt(pow(1 / delta_forza, 2) * (pow(d.dstd_media, 2)) + pow((d.media / pow(delta_forza, 2)), 2) * pow(sigma_delta_forza, 2));//senza opportuna propagazione
    d.sigma_kappa = sqrt(pow(1 / delta_forza, 2) * (pow(d.dstd_media, 2) + pow(sigma_dist_uni(10, 10), 2)) + 2 * pow(d.media * sigma_delta_forza / pow(delta_forza, 2), 2));
    if (d.nomefile.find("alto") != std::string::npos)
    {
      k_alto.push_back(d.kappa);
      sigma_k_alto.push_back(d.sigma_kappa);
      cout << "Alto"
           << "\t" << d.kappa << "\t" << d.sigma_kappa << "\t" << d.nomefile << endl;
    }
    else if (d.nomefile.find("basso") != std::string::npos)
    {
      k_basso.push_back(d.kappa);
      sigma_k_basso.push_back(d.sigma_kappa);
      cout << "Basso"
           << "\t" << d.kappa << "\t" << d.sigma_kappa << "\t" << d.nomefile << endl;
    }
    //cout << d.gp << "\t" << d.nomefile << "\t" << d.kappa << "\t" << d.sigma_kappa << endl;
  }

  //ERRORE DE SALVATOR
  double delta_1000 = abs(campioni[3].media - campioni[2].media);
  double delta_400 = abs(campioni[0].media - campioni[1].media);
  double delta_f = kgpeso_to_newton((1000.0 - 400.0) * 4 / 1000.0);
  double errore_desalva = (delta_1000 - delta_400) / delta_f;
  double num = campioni[3].media - campioni[1].media + campioni[2].media - campioni[0].media;
  double sigma_errore_desalva = sqrt(pow(1 / delta_f, 2) * (pow(campioni[0].dstd_media, 2) + pow(campioni[1].dstd_media, 2) + pow(campioni[2].dstd_media, 2) + pow(campioni[3].dstd_media, 2)) + 2 * pow(num * sigma_delta_forza / pow(delta_f, 2), 2));
  cout << "Errore DeSalvador: " << errore_desalva << "+/-" << sigma_errore_desalva << "micron/Newton" << endl;

  //for(auto d:campioni)
  //{
  //    if(d.nomefile == "../2ac/400/400_basso.txt"){
  //        for(auto c : d.media_2_acq){
  //            cout<<c<<endl;
  //        }
  //        for (auto i : d.dstd_2_acq){
  //            cout<<i<<endl;
  //        }
  //    }
  //    if(d.nomefile == "../2ac/400/400_alto.txt"){
  //        for(auto c : d.media_2_acq){
  //            cout<<c<<endl;
  //        }
  //        for (auto i : d.dstd_2_acq){
  //            cout<<i<<endl;
  //        }
  //
  //    }
  //    if(d.nomefile == "../2ac/1000/1000_basso.txt"){
  //         for(auto c : d.media_2_acq){
  //            cout<<c<<endl;
  //        }
  //        for (auto i : d.dstd_2_acq){
  //            cout<<i<<endl;
  //        }
  //    }
  //    if(d.nomefile == "../2ac/1000/1000_alto.txt"){
  //          for(auto c : d.media_2_acq){
  //            cout<<c<<endl;
  //        }
  //        for (auto i : d.dstd_2_acq){
  //            cout<<i<<endl;
  //        }
  //    }
  //
  //
  //}

  vector<double> k;
  vector<double> sigma_k;

  //media ponderata valori di k alto e errore con errore media ponderata
  double media_ponderata_k_alto = media_ponderata(k_alto, sigma_k_alto); //valori k
  k.push_back(media_ponderata_k_alto);
  double sigma_media_ponderata_k_alto = errore_media_ponderata(sigma_k_alto); //errori k
  sigma_k.push_back(sigma_media_ponderata_k_alto);
  double compatib = abs(k_alto[0] - k_alto[1]) / sqrt(pow(sigma_k_alto[0], 2) + pow(sigma_k_alto[1], 2));
  cout << "K_alto: " << compatib << "\t" << media_ponderata_k_alto << "\t" << sigma_media_ponderata_k_alto << endl;
  //cout << media_ponderata_k_alto << " +/-" << sigma_media_ponderata_k_alto << endl;

  //media ponderata valori di k basso e errore con errore media ponderata
  double media_ponderata_k_basso = media_ponderata(k_basso, sigma_k_basso);
  k.push_back(media_ponderata_k_basso);
  double sigma_media_ponderata_k_basso = errore_media_ponderata(sigma_k_basso);
  sigma_k.push_back(sigma_media_ponderata_k_basso);
  double compatib2 = abs(k_basso[0] - k_basso[1]) / sqrt(pow(sigma_k_basso[0], 2) + pow(sigma_k_basso[1], 2));
  cout << "K_basso: " << compatib2 << "\t" << media_ponderata_k_basso << "\t" << sigma_media_ponderata_k_basso << endl;
  //cout << media_ponderata_k_basso << " +/- " << sigma_media_ponderata_k_basso << endl;

  //media ponderata delle medie ponderate
  double valore_k = media_ponderata(k, sigma_k);
  double sigma_valore_k = errore_media_ponderata(sigma_k);

  cout << "K ponderato: " << valore_k << " +/- " << sigma_valore_k << " micron/Newton" << endl;
  cout << "Compatibilità K all e K acc tramite i singoli K: " << comp_3(media_ponderata_k_alto, media_ponderata_k_basso, sigma_media_ponderata_k_alto, sigma_media_ponderata_k_basso) << endl;

  //SECONDO METODO CALCOLO DI K con 2 chi quadri fra tutti gli 8 punti
  cout << "SECONDO METODO CON CHI " << endl;
  vector<double> chi_basso; //ho messo nel vettore 8 punti di 400basso
  vector<double> chi_alto;  //unifico 2 vettori per alto
  vector<double> sigma_chi_basso;
  vector<double> sigma_chi_alto;
  vector<double> delta_forza_newton; //Unico vettore che serve per entrambe le esperienze

  for (int i = 0; i < campioni[0].media_2_acq.size(); i++)
  {
    delta_forza_newton.push_back(kgpeso_to_newton((200.0) * 4 / 1000));
  }
  for (int i = 0; i < campioni[2].media_2_acq.size(); i++) //ACTUNG il vettore forza ha 17 elementi, cosi' come i dati dall'alto di campioni [2]
  {
    delta_forza_newton.push_back(kgpeso_to_newton((800.0) * 4 / 1000));
  }

  for (int i = 0; i < campioni[0].media_2_acq.size(); i++) //tutto per 400
  {
    chi_basso.push_back(campioni[0].media_2_acq[i]);
    chi_alto.push_back(campioni[1].media_2_acq[i]);
    sigma_chi_basso.push_back(campioni[0].dstd_2_acq[i]);
    sigma_chi_alto.push_back(campioni[1].dstd_2_acq[i]);
  }

  for (int i = 0; i < campioni[2].media_2_acq.size(); i++) //tutto per i 1000
  {
    chi_basso.push_back(campioni[2].media_2_acq[i]);
    chi_alto.push_back(campioni[3].media_2_acq[i]);
    sigma_chi_basso.push_back(campioni[2].dstd_2_acq[i]);
    sigma_chi_alto.push_back(campioni[3].dstd_2_acq[i]);
  }

  cout << "Angolare Basso:\t" << b_angolare(delta_forza_newton, chi_basso) << " +/- " << sigma_b(delta_forza_newton, chi_basso) << endl;
  cout << "Intercetta Basso:\t" << a_intercetta(delta_forza_newton, chi_basso) << " +/- " << sigma_a(delta_forza_newton, chi_basso) << endl;
  cout << "Angolare Alto:\t" << b_angolare(delta_forza_newton, chi_alto) << " +/- " << sigma_b(delta_forza_newton, chi_alto) << endl;
  cout << "Intercetta Alto:\t" << a_intercetta(delta_forza_newton, chi_alto) << " +/- " << sigma_a(delta_forza_newton, chi_alto) << endl;
  vector<double> chi;
  vector<double> intercetta;
  chi.push_back(b_angolare(delta_forza_newton, chi_basso));
  chi.push_back(b_angolare(delta_forza_newton, chi_alto));
  intercetta.push_back(a_intercetta(delta_forza_newton, chi_alto));
  intercetta.push_back(a_intercetta(delta_forza_newton, chi_basso));

  vector<double> sigma_chi;
  vector<double> sigma_intercetta;

  sigma_chi.push_back(sigma_b(delta_forza_newton, chi_basso));
  sigma_chi.push_back(sigma_b(delta_forza_newton, chi_alto));
  sigma_intercetta.push_back(sigma_b(delta_forza_newton, chi_alto));
  sigma_intercetta.push_back(sigma_b(delta_forza_newton, chi_basso));

  cout << "Compatibi K alto e k basso tramite chi quadro: " << comp_3(chi[0], chi[1], sigma_chi[0], sigma_chi[1]) << endl;
  cout << "Comp intercette: " << comp_3(a_intercetta(delta_forza_newton, chi_basso), a_intercetta(delta_forza_newton, chi_alto), sigma_a(delta_forza_newton, chi_basso), sigma_a(delta_forza_newton, chi_alto)) << endl;
  //Media ponderata tra k basso e k alto
  cout << "K con chi quadro tramite intepolazione 8 punti media pon k alto e K basso):\t" << media_ponderata(chi, sigma_chi) << " +/- " << errore_media_ponderata(sigma_chi) << endl;
  cout << "Intercetta pond: " << media_ponderata(intercetta, sigma_intercetta) << "+/-" << errore_media_ponderata(sigma_intercetta) << endl;

  return 0;
}

double kgpeso_to_newton(double kg_peso)
{
  return 9.806 * kg_peso;
}
