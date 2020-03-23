# Analisi dati 3 Relazione
Tutti i file presenti sono indispensabili per il funzionamento del codice scritto.  
Nelle cartelle **1ac** e **2ac** sono contenuti tutti i dati grezzi elaborati dai vari script.


## Analisi_1ac.cxx


Contiene la maggior parte del codice. Si occupa di:
- [x] Leggere i dati da file
- [x] Stampare per i grafici in gnuplot
- [x] Stampare dev std nei grafici
    - [x] Sostituire dev std con distrib uniforme per evitare errori nulli
- [ ] Verificare ipotesi di Chi quadro (errori % su asse x e y)
- [x] Esecuzione di fit con Chi quadro con libreria corretta
- [ ] Controllo compatibilità fra Chi quadro in allungamento e accorciamento

> Delta x assunto come 300micron (spessore di tacca incisa * 2) convertiti in Newton in dist uniforme



## 2ac ovvero 2ac_2metodo.cxx
Analizza le misure ripetute (secondo video). Ci sono due metodi, entrambi vengono impiegati per vedere quale è meglio. Dovrebbe dare errore maggiore di analisi_1ac. Citare isteresi meccanica perchè in tendenza le misure di x aumentano con il numero consecutivo di esecuzione.
- [x] Legge da file le misure
- [x] Genera le medie e dev std per ciascuna misura (T, M, F) e le archivia in vettore per 400 e 1000, per all e acc
- [ ] Genera media fra gli 8/9 punti (ciascuno era 3 misure) e relativo errore **non è media pondeerata ed errore ponderato, ma semplicemeente media e dev su media da campione di punti**


**Prima via Chi Quadro**
- [ ] Genera Chi fra i due punti in acc di 400-1000 e Chi con all 400-1000
- [ ] Media ponderata fra i 2 coeff. ang. ottenuti genera un solo coeff. ang.


**Seconda via di campioncini di K all e acc**
- [ ] Genera K 400/1000 e all/acc, attenzione a propagazione
- [ ] Media di K all e K acc
- [ ] Media fra K all e acc e genera K con errore tramite ponderata



## Seconda parte Young
- [x] Lettura di file excel (in realtà dati estrapolati in file CSV) solo di estensiemtri in acciaio



<!--

- [x] Generare set di {K_i} consecutivi
- [x] Generare set di {K_i} *non consecutivi*
- [x] Media e dev std con {K_i} *consecutivi*
- [x] Media e dev std con {K_i} *non consecutivi*
- [ ] Compatibilità fra {K_i} *consecutivi* e {K_i} *non consecutivi*  -->
