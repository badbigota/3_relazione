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
    double err_lunghezza = 2000.0; //in micron
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
    double int_medio;
    double err_int_medio;
    double err_intercetta_all_post;
    double err_intercetta_acc_post;
    double err_coeff_ango_all_post;
    double err_coeff_ango_acc_post;
    double compa_coeff_ango;
    double compa_intercetta;
    vector<double> allungamento;           //Contengono i dati grezzi dell'estensimetro
    vector<double> accorciamento;          //
    vector<double> delta_x_all;            //Elimina err sist di primo genere
    vector<double> delta_x_acc;            //
    vector<double> err_delta_x_all;        //
    vector<double> err_delta_x_acc;        //propag
    vector<double> err_delta_x_all_priori; //
    vector<double> err_delta_x_acc_priori;
    vector<double> k_no_cons_all;
    vector<double> k_no_cons_acc;
    vector<double> err_k_no_cons_all;
    vector<double> err_k_no_cons_acc;
};

int main()
{
    int count = 0;
    double v, x0_all, x0_acc;
    vector<estensimetri> est(2); //Crea vettore contente le strutture
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
    vector<double> young1;
    vector<double> err_young1;
    vector<double> young2;
    vector<double> err_young2;
    vector<double> young3_all_acc;
    vector<double> err_young3_all_acc;
    vector<double> e_1_metodo;
    vector<double> e_2_metodo;
    vector<double> e_3_metodo;
    vector<double> err_e_1_metodo;
    vector<double> err_e_2_metodo;
    vector<double> err_e_3_metodo;

    est[0].n_est = 1;
    est[1].n_est = 2;
    est[0].lunghezza = 1000.0 * 1000.0;
    est[1].lunghezza = 1000.0 * 1000.0;
    est[0].diametro = 0.25 * 1000.0;
    est[1].diametro = 0.5 * 1000.0;
    est[0].allungamento = {0, 250, 170, 610, 790, 1070, 1530, 1720, 1940, 1960};
    est[0].accorciamento = {-20, 340, 250, 720, 870, 1150, 1540, 1790, 1930, 1950};
    est[1].allungamento = {0, 220, 160, 620, 790, 1070, 1420, 1610, 1830, 1850};
    est[1].accorciamento = {20, 230, 190, 610, 760, 1040, 1450, 1590, 1830, 1850};
    for (int i = 0; i < est.size(); i++) //calcolo errorevcon distribuzione uniforme solo per fare grafici, commenta questo e decommenta sotto per avere la sigma post
    {
        for (int j = 0; j < est[i].allungamento.size(); j++)
        {
            est[i].err_delta_x_all_priori.push_back(sqrt(2) * sigma_dist_uni(5.0, 1));
            est[i].err_delta_x_acc_priori.push_back(sqrt(2) * sigma_dist_uni(5.0, 1));
        }

    } //FINE LETTURA

    for (int j = 0; j < est[0].allungamento.size(); j++)
    {
        delta_f.push_back(0.1 * 4 * 9.806 * j);
    }

    for (int i = 0; i < est.size(); i++) //scorre il vettore contenete le strutture
    {
        x0_all = est[i].allungamento[0];
        x0_acc = est[i].accorciamento[0];

        for (int j = 0; j < 10; j++) //calcola la differenza tra x_ennesimo e x_0, per annullare err sistematico di posizionamento zero
        {
            est[i].delta_x_all.push_back(est[i].allungamento[j] - x0_all);
            est[i].delta_x_acc.push_back(est[i].accorciamento[j] - x0_acc);
        }
        double sigma_post_all = sigma_y_posteriori(delta_f, est[i].delta_x_all);
        double sigma_post_acc = sigma_y_posteriori(delta_f, est[i].delta_x_acc);

        //cout<<est[i].n_est<<"\t"<<sigma_post_all<<"\t"<<sigma_post_acc<<"\t"<<sqrt(2) * sigma_dist_uni(5.0, 1)<<"\t"<<sqrt(2) * sigma_dist_uni(10.0, 1)<<endl;

        for (int j = 0; j < est[i].allungamento.size(); j++)
        {
            est[i].err_delta_x_all.push_back(sqrt(2) * sigma_post_all);
            est[i].err_delta_x_acc.push_back(sqrt(2) * sigma_post_acc);
        }

        est[i].intercetta_all = a_intercetta(delta_f, est[i].delta_x_all); //non metto err perchè tutti uguali
        est[i].coeff_ango_all = b_angolare(delta_f, est[i].delta_x_all);   //
        est[i].intercetta_acc = a_intercetta(delta_f, est[i].delta_x_acc); //non metto err perchè tutti uguali
        est[i].coeff_ango_acc = b_angolare(delta_f, est[i].delta_x_acc);   //
        //est[i].err_intercetta_all = sigma_a_y_uguali(delta_f, est[i].err_delta_x_all[0]); //DA RIMUOVERE SE LA SADA DICE CHE TENIAMO SIGMA POST
        //est[i].err_coeff_ango_all = sigma_b_y_uguali(delta_f, est[i].err_delta_x_all[0]); //
        //est[i].err_intercetta_acc = sigma_a_y_uguali(delta_f, est[i].err_delta_x_acc[0]); //attenzione al double
        //est[i].err_coeff_ango_acc = sigma_b_y_uguali(delta_f, est[i].err_delta_x_acc[0]); //
        est[i].err_intercetta_all_post = sigma_a_y_uguali(delta_f, sigma_post_all); //Sigma di a e b in all e acc con la sigma post
        est[i].err_intercetta_acc_post = sigma_a_y_uguali(delta_f, sigma_post_acc);
        est[i].err_coeff_ango_all_post = sigma_b_y_uguali(delta_f, sigma_post_all);
        est[i].err_coeff_ango_acc_post = sigma_b_y_uguali(delta_f, sigma_post_acc);
        est[i].err_diametro = est[i].err_diametro_perce * est[i].diametro;
        est[i].compa_coeff_ango = comp_3(est[i].coeff_ango_acc, est[i].coeff_ango_all, est[i].err_coeff_ango_acc_post, est[i].err_coeff_ango_all_post); //
        est[i].compa_intercetta = comp_3(est[i].intercetta_acc, est[i].intercetta_all, est[i].err_intercetta_acc_post, est[i].err_intercetta_all_post);
    }
    cout << "POST ACC tung:" << sigma_y_posteriori(delta_f, est[0].delta_x_acc) << endl; //solo debug
    cout << "POST ALL tung:" << sigma_y_posteriori(delta_f, est[0].delta_x_all) << endl;
    cout << "POST ACC ott:" << sigma_y_posteriori(delta_f, est[1].delta_x_acc) << endl; //solo debug
    cout << "POST ALL ott:" << sigma_y_posteriori(delta_f, est[1].delta_x_all) << endl;

    for (int i = 0; i < est.size(); i++) //scorro vettore est
    {
        vector<double> k_alto_basso;
        vector<double> err_k_alto_basso;
        vector<double> int_alto_basso;
        vector<double> err_int_alto_basso;

        k_alto_basso.push_back(est[i].coeff_ango_acc);
        k_alto_basso.push_back(est[i].coeff_ango_all);
        int_alto_basso.push_back(est[i].intercetta_acc);
        int_alto_basso.push_back(est[i].intercetta_all);
        err_k_alto_basso.push_back(est[i].err_coeff_ango_acc_post); //se non vuoi post elimina _post e utilizzerà errori piccoli
        err_k_alto_basso.push_back(est[i].err_coeff_ango_all_post); //
        err_int_alto_basso.push_back(est[i].err_intercetta_acc_post);
        err_int_alto_basso.push_back(est[i].err_intercetta_all_post);

        est[i].k_medio = media_ponderata(k_alto_basso, err_k_alto_basso);
        est[i].err_k_medio = errore_media_ponderata(err_k_alto_basso);
        est[i].int_medio = media_ponderata(int_alto_basso, err_int_alto_basso);
        est[i].err_int_medio = errore_media_ponderata(err_int_alto_basso);
        est[i].sezione = M_PI * pow(((est[i].diametro) / 2.0), 2.0);

        ofstream gout("../Grafici_2/est_" + to_string(est[i].n_est) + "_tung_ott.txt");   //STAMPA PER FARE PLOT E RIEPILOGO
        ofstream hout("../Latex/riepilogo_" + to_string(est[i].n_est) + "_tung_ott.txt"); //RIEPILOGO PER LATEX
        ofstream excelout("../estensimetri_per_excel_tung_ott.txt", ios::app);
        hout << "Numero:\t" << est[i].n_est << endl;
        hout << "Diametro:\t" << est[i].diametro << endl;
        hout << "Lunghezza\t" << est[i].lunghezza << endl;
        hout << "Ang. acc.\t" << est[i].coeff_ango_acc << " +/- " << est[i].err_coeff_ango_acc_post << endl;    //
        hout << "Interc. acc.\t" << est[i].intercetta_acc << " +/- " << est[i].err_intercetta_acc_post << endl; //se non vuoi post elimina _post e utilizzerà errori piccoli
        hout << "Ang. all.\t" << est[i].coeff_ango_all << "+/- " << est[i].err_coeff_ango_all_post << endl;     //
        hout << "Interc. all.\t" << est[i].intercetta_all << " +/- " << est[i].err_intercetta_all_post << endl; //
        hout << "Media Pond \t" << est[i].k_medio << "+/-" << est[i].err_k_medio << endl;
        // K_acc-K_all-K_pes-Comp_K-Int_acc-Int_all-Comp_Int
        excelout << est[i].coeff_ango_acc << " +/- " << est[i].err_coeff_ango_acc_post << "\t" << est[i].coeff_ango_all << "+/- " << est[i].err_coeff_ango_all_post << "\t" << est[i].k_medio << "+/-" << est[i].err_k_medio << "\t" << comp_3(est[i].coeff_ango_acc, est[i].coeff_ango_all, est[i].err_coeff_ango_acc_post, est[i].err_coeff_ango_all_post) << "\t" << est[i].intercetta_acc << " +/- " << est[i].err_intercetta_acc_post << "\t" << est[i].intercetta_all << " +/- " << est[i].err_intercetta_all_post << "\t" << est[i].int_medio << " +/- " << est[i].err_int_medio << "\t" << comp_3(est[i].intercetta_acc, est[i].intercetta_all, est[i].err_intercetta_acc_post, est[i].err_intercetta_all_post) << endl;

        gout << "#DeltaF[Newton]\t#All[micron]\t#ErrAll[micron]\t#Acc[micron]\t#ErrAcc[micron]\tsigma_post_ALL\tsigma_post_ACC" << endl;

        for (int k = 0; k < est[i].accorciamento.size(); k++) //per ogni valore di all e acc stampami i dati
        {                                                     //usa già la sigm a posteriori

            gout << delta_f[k] << "\t" << est[i].delta_x_all[k] << "\t" << est[i].err_delta_x_all_priori[k] << "\t" << est[i].delta_x_acc[k] << "\t" << est[i].err_delta_x_acc_priori[k] << "\t" << sigma_y_posteriori(delta_f, est[i].delta_x_all) << "\t" << sigma_y_posteriori(delta_f, est[i].delta_x_acc) << endl;
        }
    }

    //CALCOLO E
    ofstream eout1("../CalcoloE/tung_ott_primo_metodo.txt");
    ofstream eout2("../CalcoloE/tung_ott_secondo_metodo.txt");
    ofstream eout3("../CalcoloE/tung_ott_terzo_metodo.txt");

    //Primo Metodo (calcolo E per ogni estensimentro facendo media ponderata su E_all e E acc)
    for (int i = 0; i < est.size(); i++)
    {
        vector<double> young_i;
        vector<double> err_young_i;
        young_i.push_back(4 * est[i].lunghezza / (M_PI * pow(est[i].diametro, 2) * est[i].coeff_ango_all));
        young_i.push_back(4 * est[i].lunghezza / (M_PI * pow(est[i].diametro, 2) * est[i].coeff_ango_acc));
        err_young_i.push_back(young_i[0] * sqrt(pow((est[i].err_lunghezza / est[i].lunghezza), 2) + pow((est[i].err_coeff_ango_all / est[i].coeff_ango_all), 2) + 4 * pow((est[i].err_diametro / est[i].diametro), 2))); //CONTROLLARE
        err_young_i.push_back(young_i[1] * sqrt(pow((est[i].err_lunghezza / est[i].lunghezza), 2) + pow((est[i].err_coeff_ango_acc / est[i].coeff_ango_acc), 2) + 4 * pow((est[i].err_diametro / est[i].diametro), 2)));
        young1.push_back(media_ponderata(young_i, err_young_i));
        err_young1.push_back(errore_media_ponderata(err_young_i));
    }
    //Secondo Metodo (calcolo di E per ogni est partendo dalla meia dei K già fatta in Acc e All)
    for (int i = 0; i < est.size(); i++)
    {
        young2.push_back(4 * est[i].lunghezza / (M_PI * pow(est[i].diametro, 2) * est[i].k_medio));
        err_young2.push_back(young2[i] * sqrt(pow((est[i].err_lunghezza / est[i].lunghezza), 2) + pow((est[i].err_k_medio / est[i].k_medio), 2) + 4 * pow((est[i].err_diametro / est[i].diametro), 2)));
    }
    eout1 << "#Nest\t#E[N/micron^2]\t#ErrE[N/micron^2]" << endl;
    eout2 << "#Nest\t#E[N/micron^2]\t#ErrE[N/micron^2]" << endl;
    eout3 << "#Nest\t#E[N/micron^2]\t#ErrE[N/micron^2]" << endl;
    for (int i = 0; i < est.size(); i++)
    {
        eout1 << est[i].n_est << "\t" << young1[i] << "\t" << err_young1[i] << endl;
        eout2 << est[i].n_est << "\t" << young2[i] << "\t" << err_young2[i] << endl;
    }
    //Terzo Metodo
    for (int i = 0; i < est.size(); i++)
    {
        double upper_bound;
        double sigma_delta_f = sigma_dist_uni((((300.0 * 100.0) / (11.0 * 1000.0)) * (4.0 * 9.806 / 1000.0)), 1.0);
        if (i == est.size() - 1) //tutto sto casino per evitare di fare il k nel campioncino dell ultimo estensimetro
        {
            upper_bound = est[i].allungamento.size() - 1;
        }
        else
        {
            upper_bound = est[i].allungamento.size();
        }
        for (int j = 0; j < upper_bound; j++)
        {
            if (j % 2 == 0)
            {
                est[i].k_no_cons_all.push_back(abs((est[i].allungamento[j + 1] - est[i].allungamento[j])) / (0.4 * 9.806));
                est[i].k_no_cons_acc.push_back(abs((est[i].accorciamento[j + 1] - est[i].accorciamento[j])) / (0.4 * 9.806));
                est[i].err_k_no_cons_all.push_back(sqrt(pow(1 / (0.4 * 9.806), 2) * pow(est[i].err_delta_x_all[j], 2) + pow((-(est[i].allungamento[j + 1] - est[i].allungamento[j]) / pow((0.4 * 9.806), 2)), 2) * pow(sigma_delta_f, 2)));
                est[i].err_k_no_cons_acc.push_back(sqrt(pow(1 / (0.4 * 9.806), 2) * pow(est[i].err_delta_x_acc[j], 2) + pow((-(est[i].accorciamento[j + 1] - est[i].accorciamento[j]) / pow((0.4 * 9.806), 2)), 2) * pow(sigma_delta_f, 2))); //propag. deltaf
            }
        }
        double err_1 = est[i].err_lunghezza / est[i].lunghezza;
        double err_2_acc = dstd_media(est[i].k_no_cons_acc) / media(est[i].k_no_cons_acc);
        double err_2_all = dstd_media(est[i].k_no_cons_all) / media(est[i].k_no_cons_all);
        double err_3 = 2 * est[i].err_diametro / est[i].diametro;
        vector<double> young3;
        vector<double> err_young3;

        double young3_all = est[i].lunghezza / (est[i].sezione * media(est[i].k_no_cons_all));
        double young3_acc = est[i].lunghezza / (est[i].sezione * media(est[i].k_no_cons_acc));
        double sigma_young_3_all = young3_all * sqrt(pow(err_1, 2) + pow(err_2_all, 2) + pow(err_3, 2));
        double sigma_young_3_acc = young3_acc * sqrt(pow(err_1, 2) + pow(err_2_acc, 2) + pow(err_3, 2));
        young3.push_back(young3_all);
        young3.push_back(young3_acc);
        err_young3.push_back(sigma_young_3_acc);
        err_young3.push_back(sigma_young_3_all);

        //cout << young3_all << "\t" << sigma_young_3_all << "\t" << young3_acc << "\t" << sigma_young_3_acc << endl; //Stampa modulo young in all e acc
        young3_all_acc.push_back(media_ponderata(young3, err_young3));
        err_young3_all_acc.push_back(errore_media_ponderata(err_young3));
    }
    for (int k = 0; k < young3_all_acc.size(); k++)
    {
        //cout << young3_all_acc[i] << "\t" << err_young3_all_acc[i] << endl;
        eout3 << est[k].n_est << "\t" << young3_all_acc[k] << "\t" << err_young3_all_acc[k] << endl;
    }

    //for (int i = 0; i < est.size() - 1; i++)
    //{
    //	cout << est[i].n_est << "\t" << sigma_y_posteriori(delta_f, est[i].delta_x_all) << "\t" << sigma_y_posteriori(delta_f, est[i].delta_x_acc) << endl;
    //}
    //cout << est[9].n_est << "\t" << sigma_y_posteriori(delta_f_nos, est[9].delta_x_all) << "\t" << sigma_y_posteriori(delta_f_nos, est[9].delta_x_acc) << endl;

    return 0;
}