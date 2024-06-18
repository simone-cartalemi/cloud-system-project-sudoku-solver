# Progetto Sudoku con Tabu Search distribuito su Kubernetes

### Introduzione al problema: il Sudoku
Il Sudoku è un popolare rompicapo che ha avuto origine in Giappone. Il gioco consiste in una griglia di $9 \times 9$ celle, suddivisa in $9$ blocchi di $3 \times 3$. L'obiettivo è riempire tutte le celle della griglia con numeri da $1$ a $9$, seguendo specifiche regole: ogni numero deve apparire una sola volta in ogni riga, colonna e blocco $3 \times 3$. Nonostante le regole semplici, alcuni puzzle possono essere estremamente complessi e la loro soluzione richiede strategie sofisticate e meticolose.

### La soluzione proposta: Tabu Search
Tabu Search è una tecnica meta-euristica usato per risolvere problemi di ottimizzazione, tra cui problemi di scheduling e routing, problemi su grafi e programmazione intera. Questo metodo si basa sull'esplorazione dello spazio delle soluzioni, evitando il ritorno a soluzioni già esplorate marcandole come "tabù". Nella soluzione del Sudoku, Tabu Search può essere particolarmente efficace per superare i minimi locali, un punto comune dove altri algoritmi si blocchano, permettendo di esplorare nuove possibilità nonostante alcune mosse siano temporaneamente proibite.


## Architettura del progetto

Il sistema è stato progettato per utilizzare container Docker per incapsulare l'ambiente di esecuzione del solutore di Sudoku basato su Tabu Search. Questi container sono gestiti e orchestrati tramite Kubernetes, che permette la distribuzione scalabile e la gestione del carico di lavoro attraverso diversi nodi in un cluster.

### Costruzione immagine Docker
Il sistema è stato configurato per massimizzare l'efficienza e minimizzare l'overhead. L'immagine del risolutore di Sudoku si basa su un'immagine di Alpine Linux personalizzata, integrata con gli strumenti essenziali di *GCC*. L'immagine ufficiale di *GCC*, pur essendo completa, risulta essere troppo pesante per i nostri scopi. La scelta di Alpine Linux permette di mantenere l'immagine finale leggera e veloce, essenziale per un'efficace distribuzione scalabile.\
Sopra questa immagine base, costruiamo l'immagine contenente il risolutore di Sudoku. Al fine di implementare un'interfaccia, l'immagine include un server che ascolta sulla porta `1632`, pronto per ricevere e risolvere puzzle di Sudoku inviati.\
Un altro container fungerà da server ricevitore, in ascolto sulla porta `3264`, che si occuperà di far eseguire le istanze ai container e mantenere la connessione con i client. L'immagine del server si basa su *Python-Alpine* che è una versione molto leggera dell'interprete di Python. Ogni volta che un client si connette ad esso, avvia un thread e inoltra la soluzione ad un container di risoluzione. Ottenuta quest'ultima si occuperà di reinoltrarla al client.

### Deploy su Kubernetes
Kubernetes gioca un ruolo cruciale nella distribuzione e nella gestione del sistema. Utilizzando l'immagine costruita come descritto sopra, Kubernetes istanzia `N` container che rimangono in ascolto e non raggiungibili dall'esterno. Ogni container esegue una copia indipendente del risolutore, permettendo di processare più richieste in parallelo, in modo da facilitare la scalabilità orizzontale.
Oltre ai risolutori, viene avviato anche l'immagine del server.\
In base alla domanda, possono essere aggiunti altri container dinamicamente per gestire un carico di lavoro crescente, garantendo così che le risorse siano utilizzate in modo efficiente e che i tempi di risposta rimangano ottimali.

### Interfaccia di test
Una semplice interfaccia web visualizzerà e mostrerà la tavola del sudoku. Il server HTTP è lo stesso del backend, quindi nel container del server. Sarà sufficiente collegarsi al container per visualizzarlo.

---

## Build e Esecuzione dei Container
Il progetto è stato configurato per funzionare esclusivamente su sistemi operativi Linux, o Windows con WSL. Di seguito, sono riportati i passaggi per costruire e eseguire i container.

> Una guida per implementarlo su macchina hostata da server GARR (tramite OpenStack Dashboard) è disponibile in [guides/garr_configuration.md](guides/garr_configuration.md) Seguire questa guida di preparazione all'ambiente prima di passare al deploy del progetto.

#### Prerequisiti
- Docker
- Kubernetes (Minikube o un cluster configurato)
- Kubectl configurato per comunicare con il cluster Kubernetes
- Repository clonata in locale

#### Costruzione delle immagini Docker
Bisogna fare la build di tre immagini, di cui due per il risolutore e una per il server:
1. Immagine di **Linux Alpine**
```bash
docker build -t gcc-alpine:v1.1 ./gcc-alpine-image/
```
2. Immagine del risolutore
```bash
docker build -t sudoku-solver:v1.0 ./sudoku-solver-image/
```

Per testare eventualmente l'immagine eseguire il comando
```bash
docker run -it --rm --name solver -p 1632:1632 sudoku-solver:v1.0
```

3. Immagine del server:
```bash
docker build -t solver-server:v1.1 ./solver-server-image/
```

Per testare eventualmente l'immagine eseguire il comando
```bash
docker run -it --rm --name server -p 80:80 -p 3264:3264 solver-server:v1.1
```

#### Distribuzione su Kubernetes

Per avviare il cluster saranno creati due *Deployment* e due *Service*, eseguendo il comando
```bash
kubectl apply -f deploy.yaml
```
\
Per fermare il cluster sarà necessario stoppare i servizi tramite i comandi
```bash
kubectl delete service multi-solver-service
kubectl delete deployment solver-deployment
kubectl delete service server-service
kubectl delete deployment server-pod
```
Oppure pià semplicemente con
```bash
kubectl delete -f deploy.yaml
```
\
Per scalare il numero di repliche eseguire il comando
```bash
kubectl scale --replicas=2 multi-solver-app
kubectl scale deployment solver-deployment --replicas=0
```

