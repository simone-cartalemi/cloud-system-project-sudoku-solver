const address = "localhost";
const port = 3264;

document.addEventListener("DOMContentLoaded", (event) => {

	const inputMatrix = document.getElementById('matrix');
	const solve = document.getElementById('solve');
	const responses = document.getElementById('responses');
	const outp = (message) => {
		responses.innerHTML += "<p>" + message + "</p>";
	}

	const connect = (_address, _port, data, fun) => {
		socket = new WebSocket("ws:" + _address + ":" + _port);
		socket.addEventListener("open", () => {
			console.log('Connesso al server');
			outp("Ok");
			socket.send(data);
		});
		socket.addEventListener("message", (data) => {
			console.log('Messaggio:', data);
			if (data.data instanceof Blob) {
				res = data.data.text().then(fun);
			}
			else {
				res = data.data;
			}
			outp("Risultato: " + res);
			fun(res);
			socket.close();
		});
		socket.addEventListener("close", () => {
			console.log('Connessione chiusa');
			outp("Closed");
		});
		socket.addEventListener("error", (error) => {
			console.error('Errore durante la connessione al server: ', error);
			outp("Error");
		});
		return socket;
	};
	


	solve.addEventListener('click', () => {
		matrix = inputMatrix.value;
		const sock = connect(address, port, matrix, (response) => {
			outp(response);
		});
		outp("Loading...");
	});





});