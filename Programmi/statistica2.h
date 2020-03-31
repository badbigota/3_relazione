#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
using namespace std;


//media
double media(vector<double> dati, int inizio = 0, int fine = 0)
{
    if (fine != 0)
    {
        double media_, counter_parz = 0, sum_parz = 0;
        for (int i = inizio; i < fine; i++)
        {
            sum_parz = sum_parz + dati[i];
            counter_parz++;
        }
        media_ = sum_parz / counter_parz;
        return media_;
    }
    else if (fine == 0)
    {
        double media_, sum = 0;
        for (auto& c : dati)
        {
            sum = sum + c;
        }
        media_ = sum / dati.size();
        return media_;
    }
}
//Deviazione standard per popolazione (N-1 a denominatore)
double dstd(vector<double> dati, int inizio = 0, int fine = 0, string log = "")
{
    double dstd_camp;
    double media_camp;
    double somma_camp;
    int counter = 0;
    media_camp = media(dati, inizio, fine);
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

//Errore distribuzione triangolare
double sigma_dist_tri(double ptl, double coeff_aff)
{
    return abs(ptl / coeff_aff) / sqrt(24); //ptl con il coeff aff è doppio di err max
}

//Errore distribuzione uniforme
double sigma_dist_uni(double ptl, double coeff_aff)
{
    return abs(ptl / coeff_aff) / sqrt(12); //ptl con il suo coeff aff è il doppio di err max
}

//Delta (chi-quadro) [errori tutti uguali o del tutto assenti]
double delta(vector<double> dati_x)
{
    double delta_=0;
    double size = 0;
    double sum_1 = 0, sum_2 = 0;

    size = dati_x.size();
    for (auto& d : dati_x)
    {
        sum_1 = sum_1 + pow(d, 2);
        sum_2 = sum_2 + d;
    }
    delta_ = size * sum_1 - pow(sum_2, 2);

    return delta_;
}

//Errore intercetta se tutti i sigma sono uguali
double sigma_a_y_uguali(vector<double> dati_x, double err_y)
{
    double sum1 = 0, sum2 = 0, sum = 0;
    for (int i = 0; i < dati_x.size(); i++)
    {

        sum1 = sum1 + pow(dati_x[i], 2);
    }

    return err_y * sqrt(sum1 / delta(dati_x));
}

//Errore coeff. ang. se tutti i sigma y sono uguali
double sigma_b_y_uguali(vector<double> dati_x, double err_y)
{
    double delta_ = delta(dati_x);
    double size = dati_x.size();
    return err_y * sqrt(size / delta_);
}

//Media ponderata con errori
double media_ponderata(vector<double> valori, vector<double> errori, int inizio = 0, int fine = 0)
{
    double num = 0, den = 0;
    if (fine == 0)
    {
        fine = valori.size();
    }
    if (valori.size() != errori.size())
    {
        cout << "dimensione di vettore valori non è uguale a quella di vettore errori" << endl;
        return 1;
    }
    for (int i = inizio; i < fine; i++)
    {
        num += valori[i] * pow((1 / errori[i]), 2);
        den += pow((1 / errori[i]), 2);
    }
    return num / den;
}

//Coefficiente a di y=a+bx (intercetta)
double a_intercetta(vector<double> dati_x, vector<double> dati_y)
{
    double coeff_a=0;
    double delta_=0;
    double sum_1 = 0, sum_2 = 0, sum_3 = 0, sum_4 = 0;
    if (dati_y.size() != dati_x.size())
    {
        cout << "Dimensione di vettore per dati ascisse è diversa da dimensione vettore dati ordinate" << endl;
    }

    for (int i = 0; i < dati_x.size(); i++)
    {
        sum_1 = sum_1 + pow(dati_x.at(i), 2);
        sum_2 = sum_2 + dati_y.at(i);
        sum_3 = sum_3 + dati_x.at(i);
        sum_4 = sum_4 + (dati_x.at(i) * dati_y.at(i));
    }
    delta_ = delta(dati_x);
    coeff_a = (1 / delta_) * (sum_1 * sum_2 - sum_3 * sum_4);

    return coeff_a;
}

//Coefficiente b di y=a+bx (coeff. angolare)
double b_angolare(vector<double> dati_x, vector<double> dati_y)
{
    double coeff_b=0;
    double delta_=0;
    int size = 0;
    double sum_1 = 0, sum_2 = 0, sum_3 = 0;
    if (dati_y.size() != dati_x.size())
    {
        cout << "Dimensione di vettore per dati ascisse è diversa da dimensione vettore dati ordinate";
    }

    for (int i = 0; i < dati_x.size(); i++)
    {
        sum_1 = sum_1 + dati_x.at(i) * dati_y.at(i);
        sum_2 = sum_2 + dati_x.at(i);
        sum_3 = sum_3 + dati_y.at(i);
    }
    delta_ = delta(dati_x);
    size = dati_x.size();
    coeff_b = (1 / delta_) * (size * sum_1 - sum_2 * sum_3);

    return coeff_b;
}

//Errore medio (media ponderata)
double errore_media_ponderata(vector<double> errori)
{
    double sum = 0;
    for (auto& d : errori)
    {
        sum += pow((1 / d), 2);
    }
    return (1 / sqrt(sum));
}
//Sigma y a posteriori
double sigma_y_posteriori(vector<double> dati_x, vector<double> dati_y)
{
    double sigma_y;
    double a, b;
    int size = 0;
    double numeratore = 0;
    if (dati_x.size() != dati_y.size())
    {
        cout << "Vettori forniti non della stessa dimensione" << endl;
    }

    a = a_intercetta(dati_x, dati_y);
    b = b_angolare(dati_x, dati_y);
    size = dati_x.size();
    for (int i = 0; i < dati_x.size(); i++)
    {
        numeratore = numeratore + pow((dati_y.at(i) - a - b * dati_x.at(i)), 2);
    }
    sigma_y = sqrt(numeratore / (size - 2));
    return sigma_y;
}
