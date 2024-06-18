const address = "localhost";
const port = 3264;

document.addEventListener("DOMContentLoaded", (event) => {

	const showButton = document.getElementById('show');
	const examples = document.getElementById('examples');
	
	const inputTextMatrix = document.getElementById('matrix');
	const loadTextMatrix = document.getElementById('load');
	const cellsMatrix = document.getElementById('table-matrix').querySelectorAll('td');
	const responses = document.getElementById('responses');
	const loading = document.getElementById('loading');

	const solve = document.getElementById('solve');
	const reset = document.getElementById('reset');
	
	const showResponses = (message, optionalClass = "") => {
		responses.innerHTML = "<p" + ((optionalClass != "")? " class='" + optionalClass +  "'" : "") + ">" + message + "</p>";
	}

	const freeze = () => {
		loading.classList.remove('invisible');
		solve.classList.add('invisible');
		reset.classList.add('invisible');
		showResponses("Attendere...");
	}

	const unfreeze = () => {
		loading.classList.add('invisible');
		solve.classList.remove('invisible');
		reset.classList.remove('invisible');
	}

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
		unfreeze();
		showResponses("Ecco la soluzione!", 'right')
	}

	const resetMatrix = () => {
		cellsMatrix.forEach(cell => {
			cell.classList.remove("fixed")
			cell.innerHTML = "";
		});
		showResponses("Scrivi un'istanza e fai click su Risolvi!");
	}


	const connect = (_address, _port, data) => {
		socket = new WebSocket("ws:" + _address + ":" + _port);
		socket.addEventListener("open", () => {
			console.log('Connesso al server');
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
		});
		socket.addEventListener("error", (error) => {
			console.error('Errore durante la connessione al server: ', error);
			unfreeze();
			showResponses("Si è verificato un errore", 'wrong');
		});
		return socket;
	};

	showButton.addEventListener('click', () => {
		examples.classList.remove('invisible');
	})

	loadTextMatrix.addEventListener('click', () => {
		let matrix;
		try {
			matrix = JSON.parse(inputTextMatrix.value);
		} catch (e) {
			console.error('La stringa non è valida');
			showResponses("La stringa non è valida", 'wrong');
			return;
		}
		if (matrix.length != 81) {
			console.error('Devi inserire 81 elementi');
			showResponses("Devi inserire 81 elementi: [n1,n2,n3,..., n81]", 'wrong');
			return;
		}
		resetMatrix();

		let index = 0;
		cellsMatrix.forEach(cell => {
			const n = matrix[index++];
			if (n) {
				cell.innerHTML = n;
				cell.classList.add("fixed");
			}
		});
		showResponses("Matrice caricata con successo!", 'right')
	});

	solve.addEventListener('click', () => {
		freeze();
		matrix = JSON.stringify(readMatrix());
		const sock = connect(address, port, matrix);
	});

	reset.addEventListener('click', () => {
		resetMatrix();
	});

});