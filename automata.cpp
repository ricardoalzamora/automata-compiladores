/*
Autor: 
		Ricardo Rafael Alzamora Valle 	-	2015214008
*/
#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <sstream>

using namespace std;

class Relacion{
	public:
	int estadoRelacionado;
	string caracter;
	char excepcion;
	Relacion *sig;
	
	Relacion(){
		sig = NULL;
	}

	int getEstadoRelacionado(){
		return estadoRelacionado;
	}

	string getCaracter(){
		return caracter;
	}

	char getExcepcion(){
		return excepcion;
	}

	Relacion *getSiguiente(){
		return sig;
	}
	
};

typedef struct modeloArchivoRelaciones{
	int estadoRelacion;
	char caracter[40];
	char excepcion;
	bool activo;
}Relaciones;

typedef struct modeloArchivoNodo{
	int estado;
	char token[40];
	bool activo;
}Nodo;

void almacenarNodo(int estado, string token){
	ifstream archivoNodos;
	archivoNodos.open("nodos.dat", ios::binary);
	if(archivoNodos.good()){
		archivoNodos.seekg(0, ios::end);
		int tamanioLectura = archivoNodos.tellg()/sizeof(Nodo);
		archivoNodos.seekg(0);
		Nodo nodoAuxiliar;
		for(int i = 0; i < tamanioLectura; i++){
			archivoNodos.read(reinterpret_cast<char*>(&nodoAuxiliar), sizeof(Nodo));
			if(nodoAuxiliar.estado == estado){
				cout<<"Ya existe este lugar"<<endl;
				return;
			}
		}
	}
	archivoNodos.close();

	ofstream archivoNodoD;
	archivoNodoD.open("nodos.dat", ios::binary|ios::app);
	if(!archivoNodoD){
		cout<<"Error de apertura";
	}
	Nodo nodo;
	nodo.estado = estado;
	
	char *c = new char[40];
    strcpy(c, token.c_str());
		
	strcpy(nodo.token, c);	
	nodo.activo = true;
	
	archivoNodoD.write(reinterpret_cast<char*>(&nodo), sizeof(Nodo));
	archivoNodoD.close();
}

void almacenarRelacion(int estadoOrigen, int estadoLlegada, char caracter[40], char excepcion){
	ifstream archivoRelaciones;
	
	string str;         
	ostringstream temp;  
	temp<<estadoOrigen;
	str=temp.str(); 
	
	char *c1 = new char[40];
	strcpy(c1, str.c_str());
		
	archivoRelaciones.open(c1, ios::binary); 
	if(archivoRelaciones.good()){
		archivoRelaciones.seekg(0, ios::end);
		int tamanioLectura = archivoRelaciones.tellg()/sizeof(Relaciones);
		archivoRelaciones.seekg(0);
		Relaciones relacionesAuxiliar;
		for(int i = 0; i < tamanioLectura; i++){
			archivoRelaciones.read(reinterpret_cast<char*>(&relacionesAuxiliar), sizeof(Relaciones));
			if(relacionesAuxiliar.estadoRelacion == estadoLlegada){
				archivoRelaciones.close();
				return;
			}
		}
	}
	archivoRelaciones.close();

	ofstream archivoRelacionesD;
	archivoRelacionesD.open(c1, ios::binary|ios::app);
	if(!archivoRelacionesD){
		cout<<"Error de apertura";
	}
	
	Relaciones relaciones;
	relaciones.estadoRelacion = estadoLlegada;
	strcpy(relaciones.caracter, caracter);
	relaciones.excepcion = excepcion;
	relaciones.activo = true;
	archivoRelacionesD.write(reinterpret_cast<char*>(&relaciones), sizeof(Relaciones));
	archivoRelacionesD.close();
}

class Vertice{
	public:
		int estado;
		string token;
		Relacion *listaRelaciones;
		Vertice *sig;
		
		Vertice(int estado, string token){
			this->estado = estado;
			this->token = token;
			listaRelaciones = NULL;
			sig = NULL;
		}
		
		bool estaRelacionado(int estado){
        	Relacion *listaRelacionesAuxiliar = listaRelaciones;
        	while(listaRelacionesAuxiliar != NULL){
            	if(listaRelacionesAuxiliar->estadoRelacionado == estado){
            	    return true;
            	}
            	listaRelacionesAuxiliar = listaRelacionesAuxiliar->sig;
        	}
        	return false;
    	}
    	
		Relacion *getRelacion(int estado){
    		Relacion *listaRelacionesAuxiliar = listaRelaciones;
        	while(listaRelacionesAuxiliar != NULL){
            	if(listaRelacionesAuxiliar->estadoRelacionado == estado){
            	    return listaRelacionesAuxiliar;
            	}
            	listaRelacionesAuxiliar = listaRelacionesAuxiliar->sig;
        	}
        	return NULL;
		}
		
    void agregarRelacion(int estadoOrigen, int estadoLlegada, string caracter, char excepcion){
        if(listaRelaciones != NULL){
            if(!estaRelacionado(estadoLlegada)){
                Relacion *listaRelacionesAuxiliar = listaRelaciones;
                while(listaRelaciones->sig != NULL){
                    listaRelaciones = listaRelaciones->sig;
                }
                listaRelaciones->sig = new Relacion();
                listaRelaciones->sig->estadoRelacionado = estadoLlegada;
                listaRelaciones->sig->caracter = caracter;
                listaRelaciones->sig->excepcion = excepcion;
                listaRelaciones->sig->sig = NULL;
                listaRelaciones = listaRelacionesAuxiliar;

            }else{
                cout<<"Ya se encuentra relacionado!"<<endl;
            }
        }else{
            listaRelaciones = new Relacion();
            listaRelaciones->estadoRelacionado = estadoLlegada;
            listaRelaciones->caracter = caracter;
            listaRelaciones->excepcion = excepcion;
            listaRelaciones->sig = NULL;
        }
    }
	
	void imprimirRelacion(){
        Relacion *listaRelacionesAuxiliar = listaRelaciones;
        while(listaRelacionesAuxiliar != NULL){
            cout<<"["<<listaRelacionesAuxiliar->estadoRelacionado<<"]"<<endl;
            listaRelacionesAuxiliar = listaRelacionesAuxiliar->sig;
        }
    }		
		
};

class Grafo{
	private:
		Vertice *vertices;
		
	public:
		Grafo(){
			vertices = NULL;
		}
		
		Vertice *esta(int estado){
        	Vertice *verticeAuxiliar = vertices;
        	while(verticeAuxiliar != NULL){
            	if(verticeAuxiliar->estado == estado){
            	    return verticeAuxiliar;
            	}
            	verticeAuxiliar = verticeAuxiliar->sig;
        	}
        	return NULL;
    	}
		
    void agregarVertice(int estado, string token = "", bool guardar = true){
        if(vertices != NULL){
            if(esta(estado) == NULL){
                Vertice *verticeAuxiliar = vertices;
                while(vertices->sig != NULL){
                    vertices = vertices->sig;
                }
                vertices->sig = new Vertice(estado, token);
                vertices->sig->listaRelaciones = NULL;
                vertices->sig->sig = NULL;
                vertices = verticeAuxiliar;
                
                if(guardar){
                	almacenarNodo(estado, token);
				}

            }else{
                cout<<"Ya esta en vertice!";
            }
        }else{
            vertices = new Vertice(estado, token);
            vertices->listaRelaciones = NULL;
            vertices->sig = NULL;
            
            if(guardar){
                almacenarNodo(estado, token);
			}
        }
    }
	
	void imprimirVertices(){
        Vertice *verticeAuxiliar = vertices;
        while(verticeAuxiliar != NULL){
            cout<<"["<<verticeAuxiliar->estado<<"]"<<endl;
            verticeAuxiliar = verticeAuxiliar->sig;
        }
    }
	
    void relacionar(int estadoOrigen, int estadoLlegada, string caracter, char excepcion, bool op = true){
        Vertice *verticeAuxiliar = esta(estadoOrigen);

        if(verticeAuxiliar != NULL && esta(estadoLlegada) != NULL){

            if(!verticeAuxiliar->estaRelacionado(estadoLlegada)){
                verticeAuxiliar->agregarRelacion(estadoOrigen, estadoLlegada, caracter, excepcion);
                if(op){
                	char *c1 = new char[40];
                	strcpy(c1, caracter.c_str());
                	almacenarRelacion(estadoOrigen, estadoLlegada, c1, excepcion);
				}
            }else{
                cout<<"Ya esta relacionado!"<<endl;
            }
        }else{
        	cout<<"No se puede realizar la relacion "<<endl;
		}
    }
    
        void imprimirVerticesWAdy(){
        	Vertice *verticeAuxiliar = vertices;
        	while(verticeAuxiliar != NULL){
            	cout<<"["<<verticeAuxiliar->estado<<"] relacionado: "<<endl;
            	verticeAuxiliar->imprimirRelacion();
            	cout<<endl;
            	cout<<"------"<<endl;
            	verticeAuxiliar = verticeAuxiliar->sig;
        	}
    	}
		
};

Grafo g = Grafo();

void leerR(){
	ifstream f;
	f.open("nodos.dat", ios::binary);
	if(f.good()){
		f.seekg(0, ios::end);
		int n = f.tellg()/sizeof(Nodo);
		f.seekg(0);
		Nodo nodo;
		for(int i = 0; i < n; i++){
			f.read(reinterpret_cast<char*>(&nodo), sizeof(Nodo));

			ifstream u;
			
			string str;         
			ostringstream temp;  
			temp<<nodo.estado;
			str=temp.str(); 
	
			char *c1 = new char[40];
			strcpy(c1, str.c_str());
			
			u.open(c1, ios::binary);
			if(u.good()){
				u.seekg(0, ios::end);
				int m = u.tellg()/sizeof(Relaciones);
				u.seekg(0);
				Relaciones relaciones;
				for(int j = 0; j < m; j++){
					u.read(reinterpret_cast<char*>(&relaciones), sizeof(Relaciones));
					g.relacionar(nodo.estado, relaciones.estadoRelacion, relaciones.caracter, relaciones.excepcion, false);
				}
			}
			u.close();

		}
	}
	f.close();
}

void leer(){
	ifstream archivoNodos;
	archivoNodos.open("nodos.dat", ios::binary);
	if(archivoNodos.good()){
		archivoNodos.seekg(0, ios::end);
		int n = archivoNodos.tellg()/sizeof(Nodo);
		archivoNodos.seekg(0);
		Nodo nodo;
		for(int i = 0; i < n; i++){
			archivoNodos.read(reinterpret_cast<char*>(&nodo), sizeof(Nodo));
			g.agregarVertice(nodo.estado, nodo.token, false);
		}
	}
	archivoNodos.close();

	leerR();
}



int analizarCaracter(char caracter, string palabra, int estado){
	
	if(estado == 0){
		if(caracter == '+'){
			return 1;
		}
		if(caracter == '-'){
			return 2;
		}
		if(caracter == '='){
			return 3;
		}
		if(caracter > 48 && caracter <= 57){
			return 4;
		}
		if(((caracter >= 65 && caracter <=90) || (caracter >= 97 && caracter <= 122)) && caracter != 'p'){
			return 6;
		}
		if(caracter == 'p'){
			return 8;
		}
	}
	
	if(estado == 4){
		if(caracter >= 48 && caracter <= 57){
			return 4;
		}
		return 5;
	}
	

	if(estado == 6){
		if((caracter >= 65 && caracter <=90) || (caracter >= 97 && caracter <= 122)){
			return 6;
		}
		return 7;
	}
	
	if(estado == 8){
		if(caracter == 'r'){
			return 9;
		}
		if(((caracter >= 65 && caracter <=90) || (caracter >= 97 && caracter <= 122))){
			return 6;
		}
		return 13;
	}
	
	if(estado == 9){
		if(caracter == 'i'){
			return 10;
		}
		if((caracter >= 65 && caracter <=90) || (caracter >= 97 && caracter <= 122)){
			return 6;
		}
		return 13;
		
	}
	
	if(estado == 10){
		if(caracter == 'n'){
			return 11;
		}
		if((caracter >= 65 && caracter <=90) || (caracter >= 97 && caracter <= 122)){
			return 6;
		}
		return 13;
	}
	
	if(estado == 11){
		if(caracter == 't'){
			return 12;
		}
		if((caracter >= 65 && caracter <=90) || (caracter >= 97 && caracter <= 122)){
			return 6;
		}
		return 13;
	}
	
}

void analizarPalabra(string palabra, int estado = 0){
	char caracter;
	for(int i = 0; i < palabra.length(); i++){
		caracter = palabra[i];		
		estado = analizarCaracter(caracter, palabra, estado);		
	}
	
	if(g.esta(estado)->token.length() > 0){
		cout<<"Palabra "<<palabra<<" es valida -> \t\t"<<g.esta(estado)->token<<endl;
	}else{
		cout<<"Palabra "<<palabra<<" es invalida -> \t\tsin token"<<endl;
	}	
}

void analizarTexto(string texto){
	int inicioPalabra = 0, tamanioPalabra = 0;
	string palabra;
	

	for(int i = inicioPalabra; i < texto.length(); i++){
		if(i == texto.length() - 1){
			palabra = texto.substr(inicioPalabra, i + 1);
			analizarPalabra(palabra);
			break;
		}
		if((char)texto[i] != ' '){
			tamanioPalabra++;
		}else{
			palabra = texto.substr(inicioPalabra, tamanioPalabra);
			analizarPalabra(palabra);
			
			for(int j = inicioPalabra + tamanioPalabra; j < texto.length(); j++){
				if((char)texto[j] != ' '){
					inicioPalabra = j;
					tamanioPalabra = 0;
					break;
				}
			}
					
		}
	}	
}

void leerTexto(){
	fstream archivo("lenguaje");
	string linea;
	
	archivo.open("lenguaje.txt", ios::in);
	
	if(archivo.good()){
		
		archivo.seekg(0);
		while(getline(archivo, linea)){
			analizarTexto(linea);
		}
		archivo.close();
		
	}		
}

main(){
	
	leer();

	int opcionMenu;	
	
	do{
		system("cls");
		cout<<"\t\t\tMenu"<<endl<<"1.Leer archivo de texto."<<endl<<"2.Mostrar nodos."<<endl<<"3.Mostrar nodos y sus relacionados."<<endl<<"4.Salir"<<endl<<"Opcion: ";
		cin>>opcionMenu;
		switch(opcionMenu){
			case 1:{
				system("cls");
				cout<<endl<<"Palabras:"<<endl<<endl;
				leerTexto();
				system("PAUSE>NULL");				
				break;
			}
			
			case 2:{
				system("cls");
				g.imprimirVertices();
				system("PAUSE>NULL");
				break;
			}

			case 3:{
				system("cls");
				g.imprimirVerticesWAdy();
				system("PAUSE>NULL");
				break;
			}
		}
	}while(opcionMenu != 4);
	
	/*g.agregarVertice(0);
	g.agregarVertice(1, "SUMAR");
	g.agregarVertice(2, "RESTAR");
	g.agregarVertice(3, "ASIGNAR");
	g.agregarVertice(4);
	g.agregarVertice(5, "ENTERO");
	g.agregarVertice(6);
	g.agregarVertice(7, "LETRAS");
	g.agregarVertice(8);
	g.agregarVertice(9);
	g.agregarVertice(10);
	g.agregarVertice(11);
	g.agregarVertice(12, "IMPRIMIR");
	g.agregarVertice(13, "LETRAS");*/
	
	
	/*g.relacionar(0, 1, "+", NULL);
	g.relacionar(0, 2, "-", NULL);
	g.relacionar(0, 3, "=", NULL);
	g.relacionar(0, 4, "1-9", '0');
	g.relacionar(0, 6, "letra", 'p');
	g.relacionar(0, 8, "p", NULL);	
	g.relacionar(4, 4, "0-9", NULL);
	g.relacionar(4, 5, "otro", NULL);	
	g.relacionar(6, 6, "letra", NULL);
	g.relacionar(6, 7, "otro", NULL);	
	g.relacionar(8, 6, "letra", 'r');
	g.relacionar(8, 9, "r", NULL);
	g.relacionar(8, 13, "otro", NULL);	
	g.relacionar(9, 6, "letra", 'i');
	g.relacionar(9, 10, "i", NULL);
	g.relacionar(9, 13, "otro", NULL);	
	g.relacionar(10, 6, "letra", 'n');
	g.relacionar(10, 11, "n", NULL);
	g.relacionar(10, 13, "otro", NULL);	
	g.relacionar(11, 6, "letra", 't');
	g.relacionar(11, 12, "t", NULL);
	g.relacionar(11, 13, "otro", NULL);*/

	system("PAUSE>NULL");
}















