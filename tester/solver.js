const address = "localhost";
const port = 3264;

document.addEventListener("DOMContentLoaded", (event) => {

	const inputTextMatrix = document.getElementById('matrix');
	const loadTextMatrix = document.getElementById('load');
	const cellsMatrix = document.getElementById('table-matrix').querySelectorAll('td');
	const solve = document.getElementById('solve');
	const responses = document.getElementById('responses');

	const readMatrix = () => {
		array = [];
		cellsMatrix.forEach(cell => {
			const n = parseInt(cell.innerHTML);
			if (n >= 1 && n <= 9) {
				array.push(n);
			}
			else {
				array.push(0);
			}
		});
		return array;
	}
	
	const fillMatrix = (array) => {
		const matrix = JSON.parse(array);
		let index = 0;
		cellsMatrix.forEach(cell => {
			const n = matrix[index++];
			if (!cell.classList.contains("fixed")) {
				cell.innerHTML = n;
			}
		});
	}

	const resetMatrix = () => {
		cellsMatrix.forEach(cell => {
			cell.classList.remove("fixed")
			cell.innerHTML = "";
		});
	}

	const outp = (message) => {
		responses.innerHTML += "<p>" + message + "</p>";
	}

	const connect = (_address, _port, data) => {
		socket = new WebSocket("ws:" + _address + ":" + _port);
		socket.addEventListener("open", () => {
			console.log('Connesso al server');
			outp("Ok");
			socket.send(data);
		});
		socket.addEventListener("message", (data) => {
			console.log('Messaggio:', data);
			if (data.data instanceof Blob) {
				res = data.data.text().then(fillMatrix);
			}
			else {
				res = data.data;
			}
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

	loadTextMatrix.addEventListener('click', () => {
		resetMatrix();
		const matrix = JSON.parse(inputTextMatrix.value);

		let index = 0;
		cellsMatrix.forEach(cell => {
			const n = matrix[index++];
			if (n) {
				cell.innerHTML = n;
				cell.classList.add("fixed");
			}
		});
	});


	solve.addEventListener('click', () => {
		matrix = JSON.stringify(readMatrix());
		const sock = connect(address, port, matrix);
		outp("Loading...");
	});





});