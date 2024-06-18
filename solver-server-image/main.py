import socket
import asyncio
import websockets
import threading

from http.server import SimpleHTTPRequestHandler, HTTPServer
import os

HTTTP_PORT = 80
SERVER_PORT = 3264
SOLVER_PORT = 1632
WORKER_URL = "multi-solver-service"


def check_matrix(matrix):
    if not matrix.startswith("[") or not matrix.endswith("]"):
        return False
    t = matrix[1:-1]
    elements = t.split(",")
    if len(elements) != 81:
        return False
    for e in elements:
        if int(e) < 0 or int(e) > 9:
            return False
    return True

def solve_matrix(host, matrix):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        sock.connect((host, SOLVER_PORT))
        sock.sendall(matrix.encode() + b'\n')
        
        # Ricevi la dimensione del file dal server
        file_size = 1024
        
        # Ricevi file dal server
        received_data = b''
        while len(received_data) < file_size:
            data = sock.recv(1024)
            if not data:
                break
            received_data += data
        
        # Invia la soluzione ricevuta
        return received_data

    finally:
        sock.close()

async def handle_client(websocket, path):
    async for message in websocket:
        threading.Thread(target=process, args=(websocket, message)).start()

def process(websocket, message):
    asyncio.run(solve_sudoku(websocket, message))

async def solve_sudoku(websocket, matrix):
    # Controlla il formato della matrice di input
    if not check_matrix(matrix):
        await websocket.send("Wrong format. Using: [81 numbers between 0 and 9]")
        return

    # Ottieni l'indirizzo del solver e risolvi
    print(f"Input matrix: {matrix}")
    ip_solver = socket.gethostbyname(WORKER_URL)
    print("Solving at", ip_solver)
    solution = solve_matrix(ip_solver, matrix)
    
    if solution == None:
        return
    
    # Invia la soluzione al client
    print(f"Output matrix: {solution}")
    await websocket.send(solution)

# Funzioni per HTTP
class CustomHTTPRequestHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory='./web', **kwargs)

def start_http_server():
    web_dir = os.path.join(os.path.dirname(__file__), 'web')
    os.chdir(web_dir)
    handler = SimpleHTTPRequestHandler
    httpd = HTTPServer(("0.0.0.0", HTTTP_PORT), handler)
    print(f"Serving HTTP on port {HTTTP_PORT} from {web_dir}...")
    httpd.serve_forever()


async def main():
    # Avvia server http
    http_thread = threading.Thread(target=start_http_server)
    http_thread.daemon = True
    http_thread.start()

    # Ascolta su tutte le interfacce
    async with websockets.serve(handle_client, "0.0.0.0", SERVER_PORT):
        print("WebSocket Solver server is running...")
        
        await asyncio.Future()


# Avvio del server
if __name__ == "__main__":
    asyncio.run(main())
