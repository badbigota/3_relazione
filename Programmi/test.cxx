#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

#include "statistica.h"

using namespace std;



int main()
{
string fread,fread1,fread2;
double t;
vector <double> media_1_acq;

	cout << "Inserire le cartella che vuoi leggere"<<endl;
	cin >> fread;
    if (fread == "1_acquisizione"){

        cout << "Vuoi legge i file di Allungmento_1 o Accorciamento_2?"<<endl;
        cin >> fread1;
        


        for (int i=200;i<1300;i=i+100) {
            string numero;
            double media_vett;
            //trasformo (i) in una stringa chiamata numero
            numero=to_string(i);

            // creo un vettore dove immagazzinare i 3 dati per ogni foglio e faccio fare la media con libreria
            vector <double> primo;
	      
           ifstream fin("../"+ fread +"/" + fread1 + "/" + numero + ".txt");
	       
           while(fin>>t) {
               primo.push_back(t);
           }

       
           media_vett= media( primo,0,0,"");
           media_1_acq.push_back(media_vett);
           
     
            if(!fin){
            cout<<"Errore di lettura"<<endl;
            return 1;
            }   
        }
    }
    if (fread== "2_acquisizione"){
        vector <double> temp(3);
        vector <double> media_2_acq;
        cout << "Vuoi legge i file di 400 o 1000?"<<endl;
        cin >> fread1;
       
        cout<< "Vuoi leggere i file dall'alto o dal basso?"<<endl;
        cin >> fread2;

        ifstream fin( "../"+ fread +"/" + fread1 + "/" + fread1 + "_" + fread2 + ".txt" );
        
        int temporanea=0;
        while(fin>>t){
                temp.push_back(t);
                temporanea++;

                /*if(temporanea==24)
                        break;*/
                }

    for(int i=0;i<=temp.size();i=i+3){
       
        double media_vett=media(temp,i,i+3,"");
        media_2_acq.push_back(media_vett);
    }

    for(auto c: media_2_acq){
        cout<<c<<endl;
     }
     cout<<endl;   
        


        if(!fin){
            cout<<"Errore di lettura"<<endl;
            return 1;
        }

    }


    

 return 0;
    
}