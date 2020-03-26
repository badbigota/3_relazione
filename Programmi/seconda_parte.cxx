#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include "statistica2.h"
using namespace std;

struct estensimetri
{
	int n_est;
	double lunghezza;
	double diametro;
	double err_lunghezza = 2;
	; //in mm
	double err_diametro_perce = 0.01;
	double err_diametro;
	double sezione;
	double intercetta_all;
	double coeff_ango_all;
	double intercetta_acc;
	double coeff_ango_acc;
	double err_intercetta_all;
	double err_coeff_ango_all;
	double err_intercetta_acc;
	double err_coeff_ango_acc;
	double k_medio;
	double err_k_medio;
	vector<double> allungamento;  //Contengono i dati grezzi dell'estensimetro
	vector<double> accorciamento; //
};

int main()
{
	int count = 0;
	double v, x0_all, x0_acc;
	vector<estensimetri> est(9); //Crea vettore contente le strutture
	vector<double> delta_f;
	vector<double> errori;
	vector<double> lunghezza_l;
	vector<double> k_medio_l;
	vector<double> err_k_medio_l;
	vector<double> reciproco_sezione;
	vector<double> k_medio_s;
	vector<double> err_k_medio_s;
	vector<double> rapporto;
	vector<double> prodotto;
	vector<double> err_rapporto;
	vector<double> err_prodotto;
	vector<double> diametro_quadrato;

	ifstream fin("set_dati.txt");
	if (!fin)
	{
		cout << "Errore lettura da file";
		return 1;
	}
	for (int i = 0; i < 9; i++)
	{
		est[i].n_est = i + 1;
		fin >> est[i].lunghezza;
		fin >> est[i].diametro;
	}
	while (fin >> v)
	{
		if (count % 2 == 0)
		{
			est[(count / 2) % 9].allungamento.push_back(v * 1e+6); //conversione in micron
		}
		else
		{
			est[(count / 2) % 9].accorciamento.push_back(v * 1e+6);
		}
		count++;
	}

	//FINE LETTURA

	for (int j = 0; j < 10; j++)
	{
		errori.push_back(sqrt(2) * sigma_dist_uni(10, 1)); //ptl=10 e coeff affid  = 1 con propagazione su diferenza
		delta_f.push_back(0.1 * 4 * 9.806 * j);
	}

	for (int i = 0; i < 9; i++) //scorre il vettore contenete le strutture
	{
		vector<double> delta_x_all;
		vector<double> delta_x_acc;

		x0_all = est[i].allungamento[0];
		x0_acc = est[i].accorciamento[0];

		for (int j = 0; j < 10; j++) //calcola la differenza tra x_ennesimo e x_0, per annullare err sistematico di posizionamento zero
		{
			delta_x_all.push_back(est[i].allungamento[j] - x0_all);
			delta_x_acc.push_back(est[i].accorciamento[j] - x0_acc);
		}

		est[i].intercetta_all = a_intercetta(delta_f, delta_x_all);		  //non metto err perchè tutti uguali
		est[i].coeff_ango_all = b_angolare(delta_f, delta_x_all);		  //
		est[i].intercetta_acc = a_intercetta(delta_f, delta_x_acc);		  //non metto err perchè tutti uguali
		est[i].coeff_ango_acc = b_angolare(delta_f, delta_x_acc);		  //
		est[i].err_intercetta_all = sigma_a_y_uguali(delta_f, errori[0]); //attenzione al double
		est[i].err_coeff_ango_all = sigma_b_y_uguali(delta_f, errori[0]); //
		est[i].err_intercetta_acc = sigma_a_y_uguali(delta_f, errori[0]); //attenzione al double
		est[i].err_coeff_ango_acc = sigma_b_y_uguali(delta_f, errori[0]); //
		est[i].err_diametro = 0;										  //completare
	}

	for (int i = 0; i < est.size(); i++) //scorro vettore est
	{
		vector<double> k_alto_basso;
		vector<double> err_k_alto_basso;

		k_alto_basso.push_back(est[i].coeff_ango_acc);
		k_alto_basso.push_back(est[i].coeff_ango_all);
		err_k_alto_basso.push_back(est[i].err_coeff_ango_acc);
		err_k_alto_basso.push_back(est[i].err_coeff_ango_all);

		est[i].k_medio = media_ponderata(k_alto_basso, err_k_alto_basso);
		est[i].err_k_medio = errore_media_ponderata(err_k_alto_basso);
		est[i].sezione = M_PI * pow(((est[i].diametro) / 2), 2);

		ofstream gout("../Grafici_2/est_" + to_string(est[i].n_est) + ".txt");   //STAMPA PER FARE PLOT E RIEPILOGO
		ofstream hout("../Latex/riepilogo_" + to_string(est[i].n_est) + ".txt"); //RIEPILOGO PER LATEX

		hout << "Numero:\t" << est[i].n_est << endl;
		hout << "Diametro:\t" << est[i].diametro << endl;
		hout << "Lunghezza\t" << est[i].lunghezza << endl;
		hout << "Ang. acc.\t" << est[i].coeff_ango_acc << " +/- " << est[i].err_coeff_ango_acc << endl;
		hout << "Interc. acc.\t" << est[i].intercetta_acc << " +/- " << est[i].err_intercetta_acc << endl;
		hout << "Ang. all.\t" << est[i].coeff_ango_all << "+/- " << est[i].err_coeff_ango_all << endl;
		hout << "Interc. all.\t" << est[i].intercetta_all << " +/- " << est[i].err_intercetta_all << endl;
		hout << "Media Pond \t" << est[i].k_medio << "+/-" << est[i].err_k_medio << endl;

		gout << "#DeltaF[Newton]\t#All\t#ErrAll\t#Acc\t#ErrAcc" << endl; //PLOT INTESTAZIONE

		for (int k = 0; k < est[i].accorciamento.size(); k++) //per ogni valore di all e acc stampami i dati
		{
			gout << delta_f[k] << "\t" << est[i].allungamento[k] << "\t" << errori[k] << "\t" << est[i].accorciamento[k] << "\t" << errori[k] << endl;
		}

		//VERIFICA DI LEGGE DI YOUNG: se stessa sezione plotta la lunghezza e il relativo k (solo acciaio)
		//Genera i vettori su cui archiviare i dati per il plot fuori dal ciclo

		if (est[i].diametro == 0.279) //se stesso diametro plotta la lunghezza
		{
			lunghezza_l.push_back(est[i].lunghezza * 1e3); //convertito in micron
			k_medio_l.push_back(est[i].k_medio);
			err_k_medio_l.push_back(est[i].err_k_medio);
			rapporto.push_back(est[i].k_medio / (est[i].lunghezza * 1e3));
			err_rapporto.push_back(sqrt(pow((est[i].err_k_medio / (est[i].lunghezza * 1e3)), 2) + pow(((est[i].k_medio * (est[i].err_lunghezza * 1e3)) / pow((est[i].lunghezza * 1e3), 2)), 2)));
			//MANCA DEFINIRE IN STRUTTURA ERRORE SU LUNGHEZZA E DIAMETRO
		}

		if (est[i].lunghezza == 950) //se stessa lunghezza plotta il reciproco della sezione
		{
			reciproco_sezione.push_back(1 / (est[i].sezione * 1e3));
			diametro_quadrato.push_back(est[i].diametro * est[i].diametro * 1e6); //diametro quadrato in micron
			k_medio_s.push_back(est[i].k_medio);
			err_k_medio_s.push_back(est[i].err_k_medio);
			prodotto.push_back(est[i].k_medio * pow((est[i].diametro * 1e3), 2));
			err_prodotto.push_back(sqrt(pow(pow((est[i].diametro * 1e3), 2) * est[i].err_k_medio, 2) + (pow((2 * est[i].k_medio * (est[i].diametro * 1e3)), 2) * pow((est[i].err_diametro * 1e3), 2))));
		}
	}

	//STAMPA VERIFICA LEGGE YOUNG
	ofstream scost("../Grafici_2/a_sez_cost.txt");
	ofstream lcost("../Grafici_2/a_lung_cost.txt");
	ofstream rcost("../Grafici_2/rapporto.txt");
	ofstream pcost("../Grafici_2/prodotto.txt");
	//NON UNIRE I DUE CICLI PERCHÈ HANNO UN RANGE DI ESECUZIONE DIVERSO: 3 Volte e 7 Volte
	//sez costante
	cout << "A SEZ COST (lunghezza in asse x)" << endl;
	cout << "Coeff. ango.\t" << b_angolare(lunghezza_l, k_medio_l) << " +/- " << sigma_b_y_uguali(lunghezza_l, err_k_medio_l[0]) << endl;
	cout << "Intercetta\t" << a_intercetta(lunghezza_l, k_medio_l) << " +/- " << sigma_a_y_uguali(lunghezza_l, err_k_medio_l[0]) << endl;
	scost << "#LunghezzaEst[mm]\t#K[micron/Newton]\t#Err[micron/Newton]" << endl;
	rcost << "#Lunghezza[mm]\t#Rapporto[]\t#Err[]" << endl;

	for (int o = 0; o < lunghezza_l.size(); o++)
	{
		scost << lunghezza_l[o] << "\t" << k_medio_l[o] << "\t" << err_k_medio_l[o] << endl;
		rcost << lunghezza_l[o] << "\t" << rapporto[o] << "\t" << err_rapporto[o] << endl;
	}
	//lunghezza costante
	cout << "A LUNGHEZZA COST (1/sezione su asse x)" << endl;
	cout << "Coeff. ango.\t" << b_angolare(reciproco_sezione, k_medio_s) << " +/- " << sigma_b_y_uguali(reciproco_sezione, err_k_medio_l[0]) << endl;
	cout << "Intercetta\t" << a_intercetta(reciproco_sezione, k_medio_s) << " +/- " << sigma_a_y_uguali(reciproco_sezione, err_k_medio_l[0]) << endl;
	lcost << "#ReciprocoSez[1/micron]\t#K[micron/N]\t#Err[micron/Newton]" << endl;
	pcost << "#DiametroQuadrato[micron^2]\t#Prodotto[]\t#Err[]" << endl;
	for (int o = 0; o < reciproco_sezione.size(); o++)
	{
		lcost << reciproco_sezione[o] << "\t" << k_medio_s[o] << "\t" << err_k_medio_s[o] << endl;
		pcost << diametro_quadrato[o] << "\t" << prodotto[o] << "\t" << err_prodotto[o] << endl;
	}

	return 0;
}