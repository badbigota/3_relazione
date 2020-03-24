#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include "statistica.h"
using namespace std;
double kgpeso_to_newton(double kg_peso);

struct estensimetri
{
	int n_est;
	int lunghezza;
	double diametro;
	double intercetta;
	double coeff_ango;
	vector<double> allungamento;
	vector<double> accorciamento;
};

int main()
{
	int count = 0;
	double v, x0_all, x0_acc;
	vector<estensimetri> est(9);
	vector<double> delta_x_all;
	vector<double> delta_x_acc;
	vector<double> delta_f;
	vector<double> errori;

	ifstream fin("set_dati.txt");
	if (!fin)
	{
		cout << "Errore lettura da file";
		return 1;
	}
	for (int i = 0; i < 9; i++) //legge tutti i parametri degli estensimetri
	{
		est[i].n_est = i + 1;
		fin >> est[i].lunghezza;
		fin >> est[i].diametro;
	}
	while (fin >> v) //legge tutti i dati di allungamento e accorciamento per ogni estensimentro
	{
		if (count % 2 == 0)
		{
			est[(count / 2) % 9].allungamento.push_back(v);
		}
		else
		{
			est[(count / 2) % 9].accorciamento.push_back(v);
		}
		count++;
	}

	ofstream fout("prova.txt");
	if (!fout)
	{
		cout << "Errore scrittura su file";
		return 1;
	}
	fout << fixed << setprecision(3);

	for (int i = 0; i < 9; i++)
		fout << "Estensimetro " << est[i].n_est << "\t\t";
	fout << endl;
	for (int i = 0; i < 9; i++)
		fout << est[i].lunghezza << "\t" << est[i].diametro << "\t\t";
	fout << endl
		 << setprecision(5);
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 9; i++)
		{
			fout << est[i].allungamento[j] << "\t" << est[i].accorciamento[j] << "\t";
			if (!((i == 2) && (j == 0)))
				fout << "\t";
		}
		fout << endl;
	}

	for (int i = 0; i < 9; i++)
	{
		x0_all = est[i].allungamento[0];
		x0_acc = est[i].accorciamento[0];
		for (int j = 0; j < 10; j++)
		{
			delta_x_all.push_back(est[i].allungamento[j] - x0_all); //ricalibrazione a zero per tutti gli allungamenti
			delta_x_acc.push_back(est[i].accorciamento[j] - x0_acc);
			delta_f.push_back(kgpeso_to_newton(100 * j * 4 / 1000)); //aggiunge in Newton la delta forza
		}
	}

	for (auto c : delta_x_all)
		cout << c << endl;
	cout << endl;
	for (auto d : delta_x_acc)
		cout << d << endl;
	cout << endl;
	for (auto e : delta_f)
		cout << e << endl;
	cout << endl;

	//sigma_dist_uni(10,10);

	return 0;
}

double kgpeso_to_newton(double kg_peso)
{
	return 9.806 * kg_peso;
}
