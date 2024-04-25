import socket

def send_command_and_receive_file(host, port, command):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        sock.connect((host, port))
        sock.sendall(command.encode() + b'\n')
        
        # Ricevi la dimensione del file dal server
        file_size = 1024
        
        # Ricevi file dal server
        received_data = b''
        while len(received_data) < file_size:
            data = sock.recv(1024)
            if not data:
                break
            received_data += data
        
        # Salva il file ricevuto
        with open('results/output_file_from_container.txt', 'wb') as f:
            f.write(received_data)
            print("File salvato come 'output_file'")

    finally:
        sock.close()


def main():
    print("OK CI SIAMO\n\n\n\n\n")
    m = "[8,0,0,3,0,5,0,4,2,2,6,0,7,0,9,0,3,0,1,0,3,4,2,6,9,0,8,0,2,0,0,5,0,8,0,7,0,3,0,9,7,0,0,1,0,7,8,9,0,0,2,3,6,5,6,0,5,0,0,7,1,0,0,0,7,0,2,3,0,0,5,0,0,1,2,0,6,0,7,8,9]"
    worker_service_url = "multi-solver-service"
    ip_solver = socket.gethostbyname(worker_service_url)
    response = send_command_and_receive_file(ip_solver, 3264, m)
    print(response)
    

# Avvio del server
if __name__ == "__main__":
    main()
