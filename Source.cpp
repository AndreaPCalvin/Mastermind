//Andrea Peña Calvin

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>


const int TAM_CODIGO = 4;//tamaño del codigo
const unsigned short int NUM_ELEM = 6;//numero de colores tColor (sin contar error)
const unsigned short int NUMINTENTOS = 15;//numero max de intentos
const int VMIN = 0;//valor minimo aceptado en las opciones de juego
const int VMAX = 3;//valor maximo aceptado en las opciones de juego

enum tColor { ROJO, AZUL, VERDE, AMARILLO, MARRON, BLANCO, ERROR };

using tCodigo = std::vector<tColor>; //vector de colores


tColor char2color(char const & jug) { //conversion de char a tColor
	tColor var;
	if (jug == 'R' || jug == 'r') var = ROJO;
	else if (jug == 'Z' || jug == 'z') var = AZUL;
	else if (jug == 'V' || jug == 'v') var = VERDE;
	else if (jug == 'A' || jug == 'a') var = AMARILLO;
	else if (jug == 'M' || jug == 'm') var = MARRON;
	else if (jug == 'B' || jug == 'b') var = BLANCO;
	else var = ERROR;

	return var;
}

char color2char(tColor const & elem) { //conversion de tColor a char

	switch (elem) {
	case ROJO: return 'R';
	case AZUL: return 'Z';
	case VERDE: return 'V';
	case AMARILLO: return 'A';
	case MARRON: return 'M';
	case BLANCO: return 'B';
	}
}

int leeOpcion(){

	int opt; //opcion elegida 
	
	std::cout << "1- Jugar con un codigo sin colores repetidos" << '\n';
	std::cout << "2- Jugar con un codigo con colores repetidos" << '\n';
	std::cout << "3- Mostrar partidas jugadas" << '\n';
	std::cout << "0- Salir" << '\n';
	
	std::cout << "Ingrese una opcion: ";
	std::cin >> opt;

	while (!((VMIN <= opt) && (opt <= VMAX))) {//comprueba que el valor elegido sea un numero correcto
		std::cout << "Opcion incorrecta. Escoja una opcion valida: ";
		std::cin >> opt;
	}
	return opt;
}

bool repeticiones(const tColor & color, const int & i, const tCodigo & codigo) {//comprueba si el color ya aparece en el vector

	int j = 0;
	bool repe = false;
	while (j < i && !repe) {
		if (color2char(color) == color2char(codigo[j])) repe = true;
		j++;
	}

	return repe;
}

void codigoAleatorio(tCodigo & codigo, const bool & admiteRepetidos) { //generar el codigo que debe averiguar el jugador
	tColor color;
	bool repetido;
	std::cout << "INFO para depuracion. Codigo secreto: ";

	if (admiteRepetidos) { //opc 2
		for (int i = 0; i < TAM_CODIGO; i++) {
			codigo.at(i) = tColor(rand() % NUM_ELEM);
			std::cout << color2char(codigo[i]);
		}
	}

	else {//opcion 1, sin repeticiones

		codigo.at(0) = tColor(rand() % NUM_ELEM);
		std::cout << color2char(codigo[0]);

		for (int i = 0; i < TAM_CODIGO - 1; i++) {//comprobar que no hay repeticiones y si las hay, generar otro color 
			color = tColor(rand() % NUM_ELEM);
			repetido = repeticiones(color, i + 1, codigo);

			while (repetido) {
				color = tColor(rand() % NUM_ELEM);
				repetido = repeticiones(color, i + 1, codigo);
			}

			codigo.at(i + 1) = color;
			std::cout << color2char(codigo[i + 1]);
		}
	}

	std::cout << '\n';
}


void compararCodigos(tCodigo const & codigo, tCodigo const & hipotesis, int & colocados, int & descolocados) {

	std::vector<bool> vec(TAM_CODIGO, true);

	for (int i = 0; i < codigo.size(); ++i) {
		if (codigo[i] == hipotesis[i]) {
			++colocados;
			vec[i] = false;
		}
	}
	for (int i = 0; i < TAM_CODIGO; ++i) {
		if (codigo[i] != hipotesis[i]) {
			bool parar = false;

			for (int j = 0; j < TAM_CODIGO && !parar; ++j) {
				if (vec[j] && codigo[i] == hipotesis[j]) {
					++descolocados;
					vec[j] = false;
					parar = true;
				}
			}
		}
	}

}

bool correct(std::string const & pal) {
	int i = 0;
	bool error = false;
	while (i < pal.size() && !error) {
		//pal.size porque puede ser que se introduzca la longitud incorrecta

		if (char2color(pal[i]) == ERROR) {
			std::cout << "Has introducido un caracter no valido, elige letras de las siguientes: R, Z, V, A, M o B. ";
			error = true;
		}
		i++;
	}
	return error;
}

void leerFichero(std::vector<int>&vecpar) {
	
	int c;
	std::ifstream ficheroI;

	ficheroI.open("partidas.txt");

	if (ficheroI.is_open()) {       //almacena la info de las partidas anteriores en el vector vecpar

		ficheroI >> c;
		while (!ficheroI.fail()) {
			vecpar.push_back(c);
			ficheroI >> c;
		}
	}
	ficheroI.close();
}

void escribirFichero(std::vector<int>const & vecpar) { //
	
	std::ofstream ficheroO;

	ficheroO.open("partidas.txt");

	if (ficheroO.is_open()) 
		for (int i = 0; i < vecpar.size(); i++)
			ficheroO << vecpar[i] << " ";

	ficheroO.close();

}

void mresultados(std::vector<int>const & vecpar) {//resultado de partidas jugadas
	
	std::cout << "Resultado de las partidas jugadas" << '\n';

	for (int i = 0; i < vecpar.size(); i++) {
		if (vecpar[i] == 16) std::cout << "Partida " << i + 1 << ": No consiguio resolverse" << '\n';
		else std::cout << "Partida " << i + 1 << ": Resuelto en " << vecpar[i] << " intento(s)" << '\n';
	}
}

void sesionDeJuego() {
	tCodigo codigo(TAM_CODIGO);//almacena el codigo que genera el ordenador
	tCodigo hipotesis(TAM_CODIGO);//codigo elegido por el ususario
	int colocados = 0, descolocados = 0, intentos=0; 
	bool admiteRepetidos;
	std::vector<int>vecpar;//almacena los intentos
	
	leerFichero(vecpar);//almacena los intentos de las partidas anteriores en un vector
	int opcion = leeOpcion();//lee la opcion de juego elegida por el jugador

	while (opcion != 0) {
		if (opcion != 3) {

			if (opcion == 1) admiteRepetidos = false; //jugar SIN repetidos
			else if (opcion == 2) admiteRepetidos = true;//jugar CON repetidos

			codigoAleatorio(codigo, admiteRepetidos);//genera el codigo a adivinar

			while (intentos < NUMINTENTOS  && colocados != TAM_CODIGO) {//solo se permiten 15 intentos/ cuando todas las letras coincidan se acaba la partida

				std::cout << "Introduce el codigo (palabra de 4 letras con alguna de R, Z, V, A, M o B) : ";
				std::string pal;
				std::cin >> pal;

				bool error = correct(pal);//comprueba que los caracteres sean validos

				while (pal.size() != TAM_CODIGO || error) {//comprueba si hay un error en el codigo introducido y dice cual es

					if (pal.size() != TAM_CODIGO) std::cout << "Longitud incorrecta, deben ser 4 letras: ";//error de longitud
					std::cin >> pal;
					error = correct(pal);//error de caracter
				}
								
				for (int i = 0; i < TAM_CODIGO; i++) 	
					hipotesis.at(i) = char2color(pal[i]); //mete EL CODIGO CORRECTO en la hipotesis

				compararCodigos(codigo, hipotesis, colocados, descolocados);//determina las letras colocadas y descolocadas
				std::cout << "Hay " << colocados << " letras colocadas y " << descolocados << " descolocadas." << '\n';

				if (colocados == TAM_CODIGO) {
					std::cout << "Enhorabuena. Te ha costado " << intentos + 1 << " intento(s)." << '\n';
					vecpar.push_back(intentos + 1);
				}
				else {
					colocados = 0; descolocados = 0; intentos++;
					if (intentos == NUMINTENTOS) {
						std::cout << "Has agotado los intentos" << '\n';
						vecpar.push_back(16);
					}
				}
			}
		}

		else if (opcion == 3) mresultados(vecpar); //muestra el resultado de las partidas jugadas
		
		//aqui se acaba la partida o la consulta de resultados. Se vuelven a mostrar las opciones
		colocados = 0; descolocados = 0; intentos = 0;
		opcion = leeOpcion();//muestra al jugador las opciones
	}

	//Si opcion == 0: salir
	escribirFichero(vecpar);//escribe en el fichero los intentos de las partidas jugadas

}

int main() {
	srand(time(NULL));

	std::ifstream normas;
	std::string x;

	normas.open("normas.txt");

	if (normas.is_open()) {       //abre el fichero que lee las normas del juego
		while (!normas.eof()) {
			getline(normas, x);
			std::cout << x << '\n';
		}
	}
	normas.close();
	
	sesionDeJuego();
	
	system("PAUSE");
	return 0;
}