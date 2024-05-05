import socket
import asyncio
import websockets
import threading

SERVER_PORT = 3264
SOLVER_PORT = 1632


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

def process(websocket, matrix):
    asyncio.run(solve_sudoku(websocket, matrix))

async def solve_sudoku(websocket, matrix):
    # Controlla il formato
    if not check_matrix(matrix):
        await websocket.send("Wrong format")

    # Ottieni l'indirizzo del solver e risolvi
    print(f"Input matrix: {matrix}")
    worker_service_url = "multi-solver-service"
    ip_solver = socket.gethostbyname(worker_service_url)
    print(ip_solver)
    solution = solve_matrix(ip_solver, matrix)
    
    # Invia la soluzione al client
    if solution != None:
        await websocket.send(solution)


async def main():
    # Ascolta su tutte le interfacce
    async with websockets.serve(handle_client, "0.0.0.0", SERVER_PORT):
        print("WebSocket Solver server is running...")
        
        await asyncio.Future()


# Avvio del server
if __name__ == "__main__":
    print("Tentativo")
    worker_service_url = "multi-solver-service"
    ip_solver = socket.gethostbyname(worker_service_url)
    print(ip_solver)
    asyncio.run(main())
