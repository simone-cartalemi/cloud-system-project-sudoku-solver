import socket
import subprocess
import concurrent.futures

# Funzione per gestire una singola connessione client
def handle_client_connection(client_socket):
    # Ricevi il comando dal client
    command = client_socket.recv(1024).decode().strip()
    
    # Esegui il comando bash per scaricare il file
    try:
        output = subprocess.check_output(command, shell=True)
        # Invia il contenuto del file al client
        client_socket.sendall(output)
    except subprocess.CalledProcessError as e:
        # Se il comando non è riuscito, invia un messaggio di errore al client
        client_socket.sendall(b"Errore durante l'esecuzione del comando")

    # Chiudi la connessione con il client
    client_socket.close()

# Funzione principale del server
def main():
    # Configurazione del server
    host = '127.0.0.1'
    port = 8080

    # Crea un socket TCP/IP
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        # Bind del socket all'indirizzo e alla porta
        server_socket.bind((host, port))

        # Metti il socket in modalità ascolto
        server_socket.listen(5)
        print(f'Server in ascolto su {host}:{port}...')

        # Accetta connessioni e gestiscile in modo concorrente
        with concurrent.futures.ThreadPoolExecutor() as executor:
            while True:
                # Accetta una nuova connessione
                client_socket, client_address = server_socket.accept()
                print(f'Connessione accettata da {client_address}')

                # Avvia un thread per gestire la connessione client
                executor.submit(handle_client_connection, client_socket)

# Avvio del server
if __name__ == "__main__":
    main()
