# Installazione di Cluster Kubernetes su Ubuntu

In questa guida, ci concentreremo sull'installazione di Minikube su una macchina Ubuntu e di un cluster funzionante.

> La macchina remota era utilizzabile mediante accesso SSH, quindi se necessario creare le chiavi per la connessione e poi connettersi tramite comando
> ```bash
> ssh <user_remoto>@<ip_remoto> -i <path_della_chiave_pem> -L 80:localhost:80
> ```
> questo comando crea inoltre un tunneling tra la macchina locale e quella remota.

## Requisiti
- Sistema Operativo: Ubuntu 18.04 o successivo
- Processore: CPU con supporto alla virtualizzazione (almeno 2 VCPU)
- Memoria: Almeno 2GB di RAM dedicati a Minikube
- Spazio su Disco: Almeno 20GB di spazio libero

> Per ulteriori informazioni si rimanda ai siti ufficiali.

---

## Installazione di Docker

1. Aggiornare i pacchetti di Ubuntu
```bash
sudo apt update & sudo apt upgrade
```

2. Installare "curl"
```bash
sudo apt install -y curl wget apt-transport-https
```

3. Installare Docker
```bash
sudo apt install docker.io
```

4. Installare servizi Docker
```bash
sudo systemctl enable docker
sudo systemctl start docker
```
Per verificare il corretto funzionamento di Docker digitare
```bash
docker --version
```
e otterremo un output simile a questo
```yaml
Docker version 20.10.21, build 20.10.21-0ubuntu1~20.04.2
```
un'ulteriore prova di funzionamento può essere fatta scaricando l'immagine ufficiale di Hello World
```bash
docker run hello-world
```

Ulteriori dettagli [qui](https://medium.com/@areesmoon/installing-docker-on-ubuntu-20-04-lts-focal-fossa-be807a50d16f).

---


## Installazione di Minikube (Kubernetes)


1. Scaricare il file binario di Minikube
```bash
curl -LO https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64
```

2. Installare il file scaricato
```bash
sudo install minikube-linux-amd64 /usr/local/bin/minikube
```
e verifichiamo il funzionamento tramite il comando
```bash
minikube version
```
e otterremo un output simile a questo
```yaml
minikube version: v1.32.0
commit: 8220a6eb95f0a4d75f7f2d7b14cef975f050512d
```

3. Scaricare l'ultima versione stabile di Kubectl
```bash
VERSION=$(curl -s https://storage.googleapis.com/kubernetes-release/release/stable.txt)
curl -LO "https://storage.googleapis.com/kubernetes-release/release/${VERSION}/bin/linux/amd64/kubectl"

```

4. Installare il file binario scaricato
```bash
chmod +x kubectl
sudo mv kubectl /usr/local/bin/
```
e verifichiamo il funzionamento tramite il comando
```bash
kubectl version -o yaml
```
l'output dovrebbe essere simile a questo
```yaml
clientVersion:
 buildDate: "2023–11–15T16:58:22Z"
 compiler: gc
 gitCommit: bae2c62678db2b5053817bc97181fcc2e8388103
 gitTreeState: clean
 gitVersion: v1.28.4
 goVersion: go1.20.11
 major: "1"
 minor: "28"
 platform: linux/amd64
kustomizeVersion: v5.0.4–0.20230601165947–6ce0bf390ce3
serverVersion:
 buildDate: "2023–10–18T11:33:18Z"
 compiler: gc
 gitCommit: a8a1abc25cad87333840cd7d54be2efaf31a3177
 gitTreeState: clean
 gitVersion: v1.28.3
 goVersion: go1.20.10
 major: "1"
 minor: "28"
 platform: linux/amd64
```

5. Avviare Minikube
```bash
minikube start --driver=docker
```
per verificare lo stato dopo la fine dell'esecuzione del comando digitare
```bash
minikube status
```
si dovrebbe avere un output simile a questo
```
minikube
type: Control Plane
host: Running
kubelet: Running
apiserver: Running
kubeconfig: Configured
```

Ulteriori dettagli [qui](https://medium.com/@areesmoon/installing-minikube-on-ubuntu-20-04-lts-focal-fossa-b10fad9d0511).

Così abbiamo concluso la configurazione del nostro nodo.

[!WARNING] **Prima di buildare le immagini di Docker**: è necessario eseguire questo comando per configurare la shell corrente in modo che possa utilizzare il daemon Docker all'interno di Minikube. Questo permette di costruire e gestire le immagini Docker direttamente all'interno del contesto di Minikube senza doverle pushare su un registry Docker esterno.
```bash
eval $(minikube docker-env)
```
Questo eviterà di riscontrare problemi con la visibilità delle immagini Docker costruite localmente.

A questo punto è possibile procedere con la clonazione del repository e il deploy.



