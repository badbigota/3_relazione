# Analisi dati 3 Relazione
Tutti i file presenti sono indispensabili per il funzionamento del codice scritto.  
Nelle cartelle **1ac** e **2ac** sono contenuti tutti i dati grezzi elaborati dai vari script.


## Analisi.cxx


Contiene la maggior parte del codice. Si occupa di:
- [x] Leggere i dati da file
- [x] Stampare per i grafici in gnuplot
- [x] Stampare dev std nei grafici
    - [x] Sostituire dev std con distrib triangolare per evitare errori nulli
- [ ] Verificare ipotesi di Chi quadro (errori % su asse x e y)


> Delta x assunto come 1/20 di 100g_peso cioè errore massimo in dinamometro


**Analisi Primo Metodo**


- [x] Stimare coeff angolare Chi quadro
- [ ] Compatibilità fra coeff angolari primo metodo (allungmaneto e accorciamento) (da prendere in 2 file diversi)



**Analisi Secondo Metodo**


- [x] Generare set di {k_i} consecutivi
- [x] Generare set di {k_i} *non consecutivi*
- [x] Media e dev std con {k_i} *consecutivi*
- [x] Media e dev std con {k_i} *non consecutivi*
- [ ] Compatibilità fra {k_i} *consecutivi* e {k_i} *non consecutivi*

## Analisi2.cxx


Ancora sconosciuta.  