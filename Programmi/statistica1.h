#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
using namespace std;

//MEDIA
double media(vector<double> dati)
{

    double media_, sum = 0;
    for (auto &c : dati)
    {
        sum = sum + c;
    }
    media_ = sum / dati.size();
    return media_;
}

//Deviazione standard per popolazione (N-1 a denominatore)
double dstd(vector<double> dati)
{
    double dstd_camp;
    double media_camp;
    double somma_camp;
    int counter = 0;
    media_camp = media(dati);

    for (auto c : dati)
    {
        somma_camp = somma_camp + (media_camp - c) * (media_camp - c);
    }
    counter = dati.size();
    dstd_camp = sqrt(somma_camp / (counter - 1));

    return dstd_camp;
}

//Deviazione standard della media
double dstd_media(vector<double> dati)
{
    double dstd_media_;

    dstd_media_ = dstd(dati) / sqrt(dati.size());

    return dstd_media_;
}

//Errore distribuzione uniforme
double sigma_dist_uni(double ptl, double coeff_aff)
{
    return abs(ptl / coeff_aff) / sqrt(12); //ptl con il suo coeff aff è il doppio di err max
}

//Calcolo di Delta con errori tutti diversi
double delta(vector<double> dati_x, vector<double> errori_y)
{
    double delta_d_ = 0;
    double sum_1 = 0, sum_2 = 0, sum_3 = 0;
    for (int i = 0; i < dati_x.size(); i++)
    {
        sum_1 = sum_1 + (1 / pow(errori_y[i], 2));
        sum_2 = sum_2 + (pow(dati_x[i], 2) / pow(errori_y[i], 2));
        sum_3 = sum_3 + (dati_x[i] / pow(errori_y[i], 2));
    }
    delta_d_ = sum_1 * sum_2 - pow(sum_3,2);
    return delta_d_;
}

//Coeff. a di y=a+bx con errori tutti diversi (intercetta)
double a_intercetta(vector<double> dati_x, vector<double> dati_y, vector<double> errori_y)
{
    double a_intercetta_d_ = 0;
    double sum_1 = 0, sum_2 = 0, sum_3 = 0, sum_4 = 0;
    for (int i = 0; i < dati_x.size(); i++)
    {
        sum_1 = sum_1 + pow((dati_x[i] / errori_y[i]), 2);
        sum_2 = sum_2 + (dati_y[i] / pow(errori_y[i], 2));
        sum_3 = sum_3 + (dati_x[i] / pow(errori_y[i], 2));
        sum_4 = sum_4 + ((dati_x[i] * dati_y[i]) / pow(errori_y[i], 2));
    }
    a_intercetta_d_ = (1 / delta(dati_x, errori_y)) * (sum_1 * sum_2 - sum_3 * sum_4);
    return a_intercetta_d_;
}

//Coeff. b di y=a+bx con errori tutti diversi (coeff. ang.)
double b_angolare(vector<double> dati_x, vector<double> dati_y, vector<double> errori_y)
{
    double b_angolare_d_ = 0;
    double sum_1 = 0, sum_2 = 0, sum_3 = 0, sum_4 = 0;
    for (int i = 0; i < dati_x.size(); i++)
    {
        sum_1 = sum_1 + (1 / pow(errori_y[i], 2));
        sum_2 = sum_2 + ((dati_x[i] * dati_y[i]) / (pow(errori_y[i], 2)));
        sum_3 = sum_3 + ((dati_x[i]) / (pow(errori_y[i], 2)));
        sum_4 = sum_4 + (dati_y[i] / pow(errori_y[i], 2));
    }
    b_angolare_d_ = (1 / delta(dati_x, errori_y)) * (sum_1 * sum_2 - sum_3 * sum_4);
    return b_angolare_d_;
}

//Errore su coeff. a di y=a+bx con errori tutti diversi (intercetta)
double sigma_a(vector<double> dati_x, vector<double> dati_y, vector<double> errori_y)
{
    double sum = 0;
    for (int i = 0; i < dati_x.size(); i++)
    {
        sum = sum + (pow(dati_x[i], 2) / pow(errori_y[i], 2)); //ho cambiato FABIO
    }
    return sqrt((1 / delta(dati_x, errori_y)) * sum);
}

//Errore su coeff. b di y=a+bx con errori tutti diversi (coeff. ang.)
double sigma_b(vector<double> dati_x, vector<double> dati_y, vector<double> errori_y)
{
    double sum = 0;
    for (int i = 0; i < dati_x.size(); i++)
    {
        sum = sum + (1 / pow(errori_y[i], 2));
    }
    return sqrt((1 / delta(dati_x, errori_y)) * sum);
}