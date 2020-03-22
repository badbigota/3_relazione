#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "statistica.h"
using namespace std;

struct estensimetri
{
	int n_est;
	int lunghezza;
	double diametro;
	vector<double> allungamento;
	vector<double> accorciamento;
};

int main()
{
	int count = 0;
	double v;
	vector<estensimetri> est(9);

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
	for (int i = 0; i < 9; i++)
	{
		fout << "Estensimetro " << est[i].n_est << endl
			 << est[i].lunghezza << endl
			 << est[i].diametro << endl;
		for (int j = 0; j < 10; j++)
		{
			fout << est[i].allungamento[j] << endl
				 << est[i].accorciamento[j] << endl;
		}
		fout << endl;
	}
	return 0;
}
