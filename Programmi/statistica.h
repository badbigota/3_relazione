#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
using namespace std;

//Media
double media(vector<double> dati, int inizio = 0, int fine = 0, string log = "")
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
        if (log == "verbose") //log
        {
            cout << "Esecuzione media da " << inizio << " elemento a " << fine << "su vettore fornito" << endl;
        }
    }
    else if (fine == 0)
    {
        double media_, sum = 0;
        for (auto c : dati)
        {
            sum = sum + c;
        }
        media_ = sum / dati.size();
        return media_;
        if (log == "verbose") //log
        {
            cout << "Esecuzione media su tutto vettore fornito (" << media_ << ")" << endl;
        }
    }
}

//Scarto quadratico medio (N a denominatore)
double squad_medio(vector<double> dati, int inizio = 0, int fine = 0, string log = "")
{
    double squad;
    double media_camp;
    double somma_camp;
    int counter = 0;

    if (fine != 0)
    {
        media_camp = media(dati, inizio, fine);
        for (int j = inizio; j < fine; j++)
        {
            somma_camp = somma_camp + (media_camp - dati[j]) * (media_camp - dati[j]);
            counter++;
        }
        squad = sqrt(somma_camp / counter);
        if (log == "verbose")
        {
            cout << "Scarto quadratico medio (S) (N a denominatore) su parte del vettore (" << squad << ")" << endl;
        }
    }
    else if (fine == 0)
    {
        media_camp = media(dati);
        for (auto d : dati)
        {
            somma_camp = somma_camp + (media_camp - d) * (media_camp - d);
        }
        counter = dati.size();
        squad = sqrt(somma_camp / counter);
        if (log == "verbose")
        {
            cout << "Scarto quadratico medio (S) (N a denominatore) su tutto il vettore (" << squad << ")" << endl;
        }
    }

    return squad;
}

//Deviazione standard per popolazione (N-1 a denominatore)
double dstd(vector<double> dati, int inizio = 0, int fine = 0, string log = "")
{
    double dstd_camp;
    double media_camp;
    double somma_camp;
    int counter = 0;
    media_camp = media(dati, inizio, fine);
    if (fine != 0)
    {
        for (int j = inizio; j < fine; j++)
        {
            somma_camp = somma_camp + (media_camp - dati[j]) * (media_camp - dati[j]);
            counter++;
        }
        dstd_camp = sqrt(somma_camp / (counter - 1));
        if (log == "verbose")
        {
            cout << "Calcolo su parte del vettore di dev std(" << dstd_camp << ")" << endl;
        }
    }
    else if (fine == 0)
    {
        for (auto c : dati)
        {
            somma_camp = somma_camp + (media_camp - c) * (media_camp - c);
        }
        counter = dati.size();
        dstd_camp = sqrt(somma_camp / (counter - 1));
        if (log == "verbose")
        {
            cout << "Calcolo di dev std su tutto il vettore (" << dstd_camp << ")" << endl;
        }
    }
    return dstd_camp;
}

//Deviazione standard della media
double dstd_media(vector<double> dati, int inizio = 0, int fine = 0, string log = "")
{
    double dstd_media_;
    if (fine != 0)
    {
        dstd_media_ = dstd(dati, inizio, fine) / sqrt(fine - inizio);
        if (log == "verbose")
        {
            cout << "Calcolo deviazione standard della media su parte del vettore (" << dstd_media_ << ")" << endl;
        }
    }
    else if (fine == 0)
    {
        dstd_media_ = dstd(dati) / sqrt(dati.size());
        if (log == "verbose")
        {
            cout << "Calcolo deviazione standard della media tutto il vettore (" << dstd_media_ << ")" << endl;
        }
    }
    return dstd_media_;
}

//Compatibilità in uno stesso vettore con inizio e fine di ciascuno dei due intervalli
double comp(vector<double> dati, int inizio_primo, int fine_primo, int inizio_secondo, int fine_secondo, string log = "")
{
    double compatibility;
    double m_primo, m_secondo, dstd_primo, dstd_secondo;
    m_primo = media(dati, inizio_primo, fine_primo);
    m_secondo = media(dati, inizio_secondo, fine_secondo);
    dstd_primo = dstd_media(dati, inizio_primo, fine_primo);
    dstd_secondo = dstd_media(dati, inizio_secondo, fine_secondo);
    compatibility = (abs(m_primo - m_secondo) / sqrt(pow(dstd_primo, 2) + pow(dstd_secondo, 2)));
    if (log == "verbose")
    {
        cout << "Calcolo di compatibilità fra due serie di valori consecutivi nel vettore (" << compatibility << ") ATTENZIONE CHE QUI SI DEVE USARE LA SIGMA RIFERITA ALLA MISURA" << endl;
    }
    return compatibility;
}

//Compatibilità fra un sottoset di valori di un vettore e uno specifico valore fornito separatamente
double comp_2(vector<double> dati, int inizio, int fine, double media_ext, double dstd_media_ext, string log = "")
{
    double compatibility;
    double media_vettore, dstd_media_vet;
    media_vettore = media(dati, inizio, fine);
    dstd_media_vet = dstd_media(dati, inizio, fine);
    compatibility = abs(media_vettore - media_ext) / sqrt(pow(dstd_media_vet, 2) + pow(dstd_media_ext, 2));
    if (log == "verbose")
    {
        cout << "Calcolo di compatibilità fra parte di vettore vs. media e dstd media esterne (" << compatibility << ")" << endl;
    }
    return compatibility;
}

//Compatibilità avendo due valori medi e i sigma relativi
double comp_3(double a, double b, double sigma_a, double sigma_b)
{
    return abs(a - b) / sqrt(pow(sigma_a, 2) + pow(sigma_b, 2));
}

//Ampiezza degli intervalli per istogramma dato un vettore e il numero di intervalli
double ampiezza_int(vector<double> dati, int num_int)
{
    double min = 1000.0, max = 0.0;
    for (auto x : dati)
    {
        if (x < min)
            min = x;
        if (x > max)
            max = x;
    }
    return ((max - min) / num_int);
}

/*
INIZIO CHI QUADRO CON ERRORI TUTTI DIFFERENTI tramite function overloading
*/

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

//Delta (chi-quadro) [errori tutti uguali o del tutto assenti]
double delta(vector<double> dati_x, int inizio = 0, int fine = 0, string log = "")
{
    double delta_;
    double size = 0;
    double sum_1 = 0, sum_2 = 0;
    if (fine == 0)
    {
        size = dati_x.size();
        for (auto d : dati_x)
        {
            sum_1 = sum_1 + pow(d, 2);
            sum_2 = sum_2 + d;
        }
        delta_ = size * sum_1 - pow(sum_2, 2);
        if (log == "verbose")
        {
            cout << "Calcolo di delta su tutto il vettore delle ascisse per chi quadro (" << delta_ << ")" << endl;
        }
    }
    else if (fine != 0)
    {
        for (int i = inizio; i < fine; i++)
        {
            size++;
            sum_1 = sum_1 + pow(dati_x.at(i), 2);
            sum_2 = sum_2 + dati_x.at(i);
        }
        delta_ = size * sum_1 - pow(sum_2, 2);
        if (log == "verbose")
        {
            cout << "Calcolo di delta su parte del vettore delle ascisse per chi quadro (" << delta_ << ")" << endl;
        }
    }

    return delta_;
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
//Errore in tercetta se tutti i sigma sono uguali
double sigma_a_y_uguali(vector<double> dati_x, vector<double> dati_y, double err_y)
{

    double sum1 = 0, sum2 = 0, sum = 0;
    for (int i = 0; i < dati_x.size(); i++)
    {

        sum1 = sum1 + pow(dati_x[i], 2);
    }

    return err_y * sqrt(sum1 / delta(dati_x));
}

//Errore coeff. ang. se tutti i sigma y sono uguali
double sigma_b_y_uguali(vector<double> dati_x, vector<double> dati_y, double err_y)
{
    double delta_ = delta(dati_x);
    double size = dati_x.size();
    return err_y * sqrt(size / delta_);
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

/*
INIZIO CHI QUADRO CON ERRORI ERRORI ASSENTI< VIENE USATA LA SIGMA POSTERIORI
*/



//Coefficiente a di y=a+bx (intercetta) 
double a_intercetta(vector<double> dati_x, vector<double> dati_y, int inizio = 0, int fine = 0, string log = "")
{
    double coeff_a;
    double delta_;
    double sum_1 = 0, sum_2 = 0, sum_3 = 0, sum_4 = 0;
    if (dati_y.size() != dati_x.size())
    {
        cout << "Dimensione di vettore per dati ascisse è diversa da dimensione vettore dati ordinate" << endl;
    }
    if (fine == 0)
    {
        for (int i = 0; i < dati_x.size(); i++)
        {
            sum_1 = sum_1 + pow(dati_x.at(i), 2);
            sum_2 = sum_2 + dati_y.at(i);
            sum_3 = sum_3 + dati_x.at(i);
            sum_4 = sum_4 + (dati_x.at(i) * dati_y.at(i));
        }
        delta_ = delta(dati_x);
        coeff_a = (1 / delta_) * (sum_1 * sum_2 - sum_3 * sum_4);
        if (log == "verbose")
        {
            cout << "Calcolo di coeff a intercetta su tutti i valori dei vettori forniti (" << coeff_a << ")" << endl;
        }
    }
    else if (fine != 0)
    {
        for (int i = inizio; i < fine; i++)
        {
            sum_1 = sum_1 + pow(dati_x.at(i), 2);
            sum_2 = sum_2 + dati_y.at(i);
            sum_3 = sum_3 + dati_x.at(i);
            sum_4 = sum_4 + (dati_x.at(i) * dati_y.at(i));
        }
        delta_ = delta(dati_x, inizio, fine);
        coeff_a = (1 / delta_) * (sum_1 * sum_2 - sum_3 * sum_4);
        if (log == "verbose")
        {
            cout << "Calcolo di coeff a intercetta su alcuni valori dei vettori forniti (" << coeff_a << ")" << endl;
        }
    }
    return coeff_a;
}

//Coefficiente b di y=a+bx (coeff. angolare)
double b_angolare(vector<double> dati_x, vector<double> dati_y, int inizio = 0, int fine = 0, string log = "")
{
    double coeff_b;
    double delta_;
    int size = 0;
    double sum_1 = 0, sum_2 = 0, sum_3 = 0;
    if (dati_y.size() != dati_x.size())
    {
        cout << "Dimensione di vettore per dati ascisse è diversa da dimensione vettore dati ordinate";
    }
    if (fine == 0)
    {
        for (int i = 0; i < dati_x.size(); i++)
        {
            sum_1 = sum_1 + dati_x.at(i) * dati_y.at(i);
            sum_2 = sum_2 + dati_x.at(i);
            sum_3 = sum_3 + dati_y.at(i);
        }
        delta_ = delta(dati_x);
        size = dati_x.size();
        coeff_b = (1 / delta_) * (size * sum_1 - sum_2 * sum_3);
        if (log == "verbose")
        {
            cout << "Calcolo su tutti i valori del vettore (" << coeff_b << ")" << endl;
        }
    }
    else if (fine != 0)
    {
        for (int i = inizio; i < fine; i++)
        {
            sum_1 = sum_1 + dati_x.at(i) * dati_y.at(i);
            sum_2 = sum_2 + dati_x.at(i);
            sum_3 = sum_3 + dati_y.at(i);
            size++;
        }
        delta_ = delta(dati_x, inizio, fine);
        coeff_b = (1 / delta_) * (size * sum_1 - sum_2 * sum_3);
        if (log == "verbose")
        {
            cout << "Calcolo su parte dei valori del vettore (" << coeff_b << ")" << endl;
        }
    }

    return coeff_b;
}

//Sigma y a posteriori
double sigma_y_posteriori(vector<double> dati_x, vector<double> dati_y, int inizio = 0, int fine = 0, string log = "")
{
    double sigma_y;
    double a, b;
    int size = 0;
    double numeratore = 0;
    if (dati_x.size() != dati_y.size())
    {
        cout << "Vettori forniti non della stessa dimensione" << endl;
    }
    if (fine == 0)
    {
        a = a_intercetta(dati_x, dati_y);
        b = b_angolare(dati_x, dati_y);
        size = dati_x.size();
        for (int i = 0; i < dati_x.size(); i++)
        {
            numeratore = numeratore + pow((dati_y.at(i) - a - b * dati_x.at(i)), 2);
        }
        sigma_y = sqrt(numeratore / (size - 2));
    }
    else if (fine != 0)
    {
        a = a_intercetta(dati_x, dati_y, inizio, fine);
        b = b_angolare(dati_x, dati_y, inizio, fine);
        for (int i = inizio; i < fine; i++)
        {
            numeratore = numeratore + pow((dati_y.at(i) - a - b * dati_x.at(i)), 2);
            size++;
        }
        sigma_y = sqrt(numeratore / (size - 2));
    }
    return sigma_y;
}

//Errore su coeff. a di y=a+bx con sigma y a posteriori (intercetta)
double sigma_a(vector<double> dati_x, vector<double> dati_y, int inizio = 0, int fine = 0, string log = "")
{
    double sigma_a_;
    double sigma_y;
    double delta_;
    double sum = 0;
    if (fine == 0)
    {
        sigma_y = sigma_y_posteriori(dati_x, dati_y);
        delta_ = delta(dati_x);
        for (auto d : dati_x)
        {
            sum = sum + pow(d, 2);
        }
        sigma_a_ = sigma_y * sqrt(sum / delta_);
        if (log == "verbose")
        {
            cout << "Calcolo su tutti i valori di vettori (" << sigma_a_ << ")" << endl;
        }
    }
    else if (fine != 0)
    {
        sigma_y = sigma_y_posteriori(dati_x, dati_y, inizio, fine);
        delta_ = delta(dati_x, inizio, fine);
        for (int i = inizio; i < fine; i++)
        {
            sum = sum + pow(dati_x.at(i), 2);
        }
        sigma_a_ = sigma_y * sqrt(sum / delta_);
        if (log == "verbose")
        {
            cout << "Calcolo su parte di valori di vettori (" << sigma_a_ << ")" << endl;
        }
    }

    return sigma_a_;
}

//Errore su coeff. b con sigma y a posteriori(coeff. angolare)
double sigma_b(vector<double> dati_x, vector<double> dati_y, int inizio = 0, int fine = 0, string log = "")
{
    double sigma_b_p;
    double sigma_y;
    double delta_;
    int size = 0;
    if (fine == 0)
    {
        sigma_y = sigma_y_posteriori(dati_x, dati_y);
        size = dati_x.size();
        delta_ = delta(dati_x);
        sigma_b_p = sigma_y * sqrt(size / delta_);
        if (log == "verbose")
        {
            cout << "Calcolo su tutti i valori di vettori (" << sigma_b_p << ")" << endl;
        }
    }
    else if (fine != 0)
    {
        sigma_y = sigma_y_posteriori(dati_x, dati_y, inizio, fine);
        size = fine - inizio;
        delta_ = delta(dati_x, inizio, fine);
        sigma_b_p = sigma_y * sqrt(size / delta_);

        if (log == "verbose")
        {
            cout << "Calcolo su parte di valori di vettori (" << sigma_b_p << ")" << endl;
        }
    }
    return sigma_b_p;
}

//Media ponderata con errori
double media_ponderata(vector<double> valori, vector<double> errori, int inizio = 0, int fine = 0, string log = "")
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
    if (log == "verbose")
    {
        cout << "Calcolo di valor vero tramite vettore di valori e vettori di errori" << endl;
    }
    return num / den;
}

//Errore medio (media ponderata)
double errore_media_ponderata(vector<double> errori)
{
    double sum = 0;
    for (auto d : errori)
    {
        sum += pow((1 / d), 2);
    }
    return (1 / sqrt(sum));
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