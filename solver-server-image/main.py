import socket
import asyncio
import websockets
import threading

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


async def main():
    # Ascolta su tutte le interfacce
    async with websockets.serve(handle_client, "0.0.0.0", SERVER_PORT):
        print("WebSocket Solver server is running...")
        
        await asyncio.Future()


# Avvio del server
if __name__ == "__main__":
    asyncio.run(main())
