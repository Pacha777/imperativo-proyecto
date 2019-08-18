#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//#include "logica.h"
//Jose Barrientos R


//Determina la cantidad de pruebas que se realizaran segun el archivo a leer
int cantidadPruebas(int *count){
	static const char filename[] = "PruebasGrupo04.txt";
	FILE *file = fopen ( filename, "r" );
	char c;  // Guarda lo leido
	char c1 = '0'; 
	// Verifica que el archivo exista
	if (file == NULL)
	{
	    printf("ERROR: No se pudo abrir el archivo %s\n", filename);
	    return 0;
	}

	// Extrae los caracteres y los guarda en c
	for (c = getc(file); c != EOF; c = getc(file))
	    if (c == '\n') // Incrementa la cuenta si el valor es un salto de linea
	        *count = *count + 1;

	// Cierra el archivo
	fclose(file);
	return 1;
}

//Lee los datos del archivo
int leerArchivo(char informacion[][180]){
	static const char filename[] = "PruebasGrupo04.txt";
	FILE *file = fopen ( filename, "r" );

	if ( file != NULL )
    {
		char line [ 180 ];

		int i = 0;
		while ( fgets ( line, sizeof line, file) != NULL ) /* read a line */
		{
			strcpy(informacion [i], line); 
			i++;
		}
		fclose ( file );
    }
    else
    {
    	perror ( filename ); /* why didn't the file open? */
    }	
}

//Valida el formato del archivo
int validarArchivo(int count, char informacion[][180]){
	char zero[2];
	strcpy(zero,informacion[count-1]);
	//En el archivo cada dos lineas implica un tablero de prueba y termina en cero
	//Por lo tanto el total de lineas deberia ser un numero impar
	if (count % 2 == 0)
	{
		printf("ERROR: El archivo no cuenta con el formato correcto\n");
		return 0;
	}

	//Se verifica que el archivo termine en 0
	if (strcmp(informacion[count-1], "0") != 10)
	{

		printf("ERROR: El archivo no termina en 0, diferencia %d \n", strcmp(informacion[count-1], "0"));
		return 0;
	}
	return 1;
}

//Valida que la entrada sea un digito
int validarString(char informacion[180]){
	int i = 0;
	while(i<strlen(informacion)-1) 
	{
		if (!isdigit(informacion [i]))
  		{
  			printf("ERROR: se encontro un caracter que no es un digito: %c \n", informacion [i]);
  			return 0;
		}
		i++;
	}

	//en caso de que la informacion sea mas de 2 digitos
	if (strlen(informacion)-1 > 2)
		return 0;
	return 1;
}

//Valida que la entrada sea un digito
/*int validarString2(char informacion[180])
{
	int i = 0;
	while(i < strlen(informacion)) 
	{
		if(informacion [i] != ' '){
			if (!isdigit(informacion [i]))
	  		{
	  			printf("ERROR1: se encontro un caracter que no es un digito: %c\n", informacion [i]);
	  			return 0;
			}
		}
		i++;
	}

	//en caso de que la informacion sea mas de 2 digitos
	if (strlen(informacion) > 2)
		return 0;
	return 1;
}*/

//Guarda las posiciones del archivo en un arreglo
int cantidadPosiciones(char posiciones [], int tamano, int *fichasPosicion){
	const char delimitador[2] = " ";  //Determina el caracter delimitador
	char *token;
	int i = 0;

	//Divide el string cada vez que encuentra el delimitador
	token = strtok(posiciones, delimitador);

	//Ciclo para asignar los valores en el arreglo de posiciones
	while( token != NULL ) 
	{
		if(validarString(token) == 0)
			return 0;
		fichasPosicion [i] = atoi(token); //Convierte el char en un integer
		
		if (fichasPosicion [i] > tamano || fichasPosicion [i] < 1)	//Verifica que sea un numero y que se encuentre en el rango
		{
			printf("ERROR: tamano incorrecto en la posicion %d \n", fichasPosicion [i]);
			return 0;
		}
		token = strtok(NULL, delimitador);
		i++;
	}

	//Si el tamano no corresponde al de la matriz devuelve un cero, ya que faltan o sobran posiciones
	if (i != tamano*2)
		return 0;

	return 1;
}

//Guarda los tamanos de las matrices
int** tamanoMatriz(int tamano, char informacion[][180], int matricesTamano[], int **fichasPosicion, int *errorLectura){
	int i, j=0;

	//Asigna el tamano de cada tablero
	for (i = 0; i < tamano; i++)
	{
		//Valida que sea un numero
		if(validarString(informacion [j]) == 0)
			*errorLectura = 1;

		matricesTamano [i] = atoi (informacion [j]);

		//Valida que este entre 1 y 15
		if(matricesTamano [i] > 15 || matricesTamano [i] < 1)
			*errorLectura = 1;
		j+=2;
	}

	//Asigna la cantidad de tableros
	fichasPosicion = (int **)malloc (tamano*sizeof(int *));   // Se crean las Filas.

	//Asigna el tamano de las posiciones necesarias a cada tablero
	for(i=0; i < tamano; i++)
	{
		fichasPosicion [i] = (int *) malloc (matricesTamano[i]*2*sizeof(int)); // Se crean las Columnas.
	}

	j=1; //La informacion se encuentra cada dos espacios iniciando en 1

	//Recorre cada tablero leyendo las asignaciones
	for ( i = 0; i < tamano; i ++)
	{	
		if(cantidadPosiciones(informacion [j], matricesTamano [i], fichasPosicion[i]) == 0)
			*errorLectura = 1;
		j+=2; //La informacion se encuentra cada dos espacios iniciando en 1
	}

	//Devuelve las fichas en sus posiciones
	return fichasPosicion;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Logica
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void imprimirMatriz(int N, char **matriz){ // se imprime la matriz para ver los valores que tien adentro

	int i = 0, j = 0; //inicializar i j para llevar los contadores para imrimir en pantalla

	printf("\n");	// para dejar un espacio en la consola y se vea lindo

	printf("\t");	// para dejar un tab y dejar lindo

	for (i = 0; i < N; i++){ //Ciclo que coloca el número a cada columna

		if(i==0){ //para la primera iteracion se debe dejar un espacio mayor para que se vea ordenado al imprimir

			printf("        %d", i+1); // imprime en pantalla el primer numer

		}else if(i<10){

			printf("     %d", i+1); // imprime en orden los numero del 1 al 9

		}else{
			printf("    %d", i+1);	//imprime en orden los numero con 2 digitos del 10 al 15		
		}
	}
	
	for(i=0; i < N; i++){	// contador de filas
	
		printf("\n\t"); // salto de linea a la siguiente fila

		if(i<9){//Sentencia If que coloca el número de cada fila

			printf("   %d ", i+1); //esto es para colocal el numero de la filas

		}else{
			printf("  %d ", i+1);

		}

		for(j=0; j<N; j++){	// Contador de columnas 

			if(matriz[i][j] == '*'){
				printf("|  %c  ",matriz[i][j]); // Imprimir las columnas 
			}else if(matriz[i][j] == 'v' || matriz[i][j] == '^'){
				printf("|  %c  ",matriz[i][j]); // Imprimir las columnas
			}else {
				printf("|  %c  ",matriz[i][j]); // Imprimir las columnas
			}
		}
			
		printf("|"); //para la utlima que no pone el ultimo entonces para que se vea ordenado
	}
	
	printf("%s","\n \n"); // solo para que se vea bonito en el shell
}

char** crearMatriz(int N, char **pMatriz){ //Funcion que crea la matriz dinámica y devuelve la matriz ya hecha

	int i, j; // tiene doble puntero por fila columna.	

	pMatriz = (char **)malloc (N*sizeof(char *));   // Se crean las Filas.

	for(i=0; i < N;i++){ // ciclo para crear columnas

		pMatriz[i] = (char *) malloc (N*sizeof(char)); // Se crean las Columnas.

	}

	return pMatriz; // regresa la matriz dinamica creada, la matriz se hizo se con malloc para que queda llena de 0, los cuales vamos 
                  //trabajar como nulls
}

int asignarFichas(char **pMatriz, int *fichas, int N){
	
	int i, j;
	
	int posFila = 0;
	
	int posColumna = 0;

	//Hace todas las celdas de la matriz de un mismo tamaño
	for (i = 0; i < N; i++)
	{	
		for (j = 0; j < N; j++)
		{
			pMatriz[i][j] = ' ';
		}
	}


	for (i = 0; i < N*2 -1; i+=2) //recorre el arreglo de fichas para colocarlas
	{
		posFila = fichas[i]-1;

		posColumna = fichas[i+1]-1;
		if(pMatriz[posFila][posColumna] == '*')
			return 1;
		pMatriz[posFila][posColumna] = '*'; //Asigna la ficha en la fila y columna

	}

	return 0;
}

void imprimirSolucionVertical(int N, int pColumnaOptima, int pCantMinima, char **pMatriz){
	int i, j, w; // se inicializa variables para llevar la cuenta de columnas filas y la columna principal
	char **matrizAuxiliar; // sea crea una matriz auxiliar dinamica
	matrizAuxiliar = crearMatriz(N, matrizAuxiliar);//crea una matriz auxiliar

	for (i = 0; i < N; i++)//Recorre las filas
	{	
		for (j = 0; j < N; j++)//Recorre las columnas
		{
			matrizAuxiliar[i][j] = ' '; // se llena de chars vacios "espacios vacios"
		}
	}

	for (i = 0; i < N; i++)//Recorre las filas
	{	
		for (j = 0; j < N; j++)//Recorre las columnas
		{

			if(pMatriz[i][j]=='*'){ // si la matriz encuentra un "*" entonces hace la otra logica.

				if(pColumnaOptima > j){
					//Asigna > en posiciones menores a la posicion optima
					for (w = j; w < pColumnaOptima; w++)
					{
						matrizAuxiliar[i][w] = '>'; // este son los movimientos que se movio la "*" a la derecha
					}					
				}else if(pColumnaOptima < j){
					//Asigna < en posiciones menores a la posicion optima
					for (w = j; w > pColumnaOptima; w--)
					{
						matrizAuxiliar[i][w] = '<';// este son los movimientos que se movio la "*" a la izquierda
					}					
				}
			}	
		}
		matrizAuxiliar[i][pColumnaOptima] = '*';//pone la "*" en la columna que hace menos moviemientos 
	}
  //imprime informacion importante 
	printf("\n\t\tSOLUCION OPTIMA.\n\n");
	printf("\tMovimientos: %d\n",pCantMinima);
	printf("\tPosicion ideal: columna %d\n", pColumnaOptima + 1);
	imprimirMatriz(N, matrizAuxiliar);
	free(matrizAuxiliar);
  //fin imprime informacion importante 
}

void imprimirSolucionHorizontal(int N, int pFilaOptima, int pCantMinima, char **pMatriz){
	int i, j, w;
	char **matrizAuxiliar;
	matrizAuxiliar = crearMatriz(N, matrizAuxiliar);//crea una matriz auxiliar
	//Imprimir Solucion
	for (i = 0; i < N; i++)//Recorre las filas
	{	
		for (j = 0; j < N; j++)//Recorre las columnas
		{
			matrizAuxiliar[i][j] = ' ';// se asigna espacios adentro
		}
	}

	for (j = 0; j < N; j++)//Recorre las columnas
	{	
		for (i = 0; i < N; i++)//Recorre las columnas
		{

			if(pMatriz[i][j]=='*'){ // Si encuentra la "*" entonces hace la logica

				if(pFilaOptima > i){
					//Asigna > en posiciones menores a la posicion optima
					for (w = i; w < pFilaOptima; w++)
					{
						matrizAuxiliar[w][j] = 'v'; //la cantida de moviemientos hacia abajo para mover la "*"
					}					
				}else if(pFilaOptima < i){
					//Asigna < en posiciones menores a la posicion optima
					for (w = i; w > pFilaOptima; w--)
					{
						matrizAuxiliar[w][j] = '^';//la cantida de moviemientos hacia arriba para mover la "*"
					}					
				}
			}	
		}
		matrizAuxiliar[pFilaOptima][j] = '*'; //imprimir en la fila que tiene menos moviemientos  
	}
  //imprime en pantalla informacion 
	printf("\n\t\tSOLUCION OPTIMA.\n\n");
	printf("\tMovimientos: %d\n",pCantMinima);
	printf("\tPosicion ideal: fila %d\n", pFilaOptima + 1);
	imprimirMatriz(N, matrizAuxiliar);
	free(matrizAuxiliar);
  //fin imprimir en pantalla informacion
}

void imprimirSolucionDiagonal(int N, int pFilaOptima, int pCantMinima, char **pMatriz){
	int i, j, w;
	char **matrizAuxiliar;// matriz dinamica auxiliar 
	matrizAuxiliar = crearMatriz(N, matrizAuxiliar);//crea una matriz auxiliar
	//Imprimir Solucion
	for (i = 0; i < N; i++)//Recorre las filas
	{	
		for (j = 0; j < N; j++)//Recorre las columnas
		{
			matrizAuxiliar[i][j] = ' '; // llenar de espacios para imprimir la matriz
		}
	}

	for (j = 0; j < N; j++)//Recorre las columnas
	{	
		for (i = 0; i < N; i++)//Recorre las columnas
		{

			if(pMatriz[i][j]=='*'){ // cuando se encuentra "*" se hace la siguiemte logica
				if((i+j) > (N-1)){
          for(w = j; w > i; w--){
					  matrizAuxiliar[i][w] = '<'; // mover a la izquierda luego de encontrar "*"
          }  
				}
				if((i+j) < (N-1)) {
          for(w = i; w > j; w--){
					  matrizAuxiliar[w][j] = '^'; // mover hacia arriba luego de encontrar "*"
          }
				}else if((i+i) > (N-1)){
          for(w = j; w < i; w++){
					  matrizAuxiliar[i][w] = '>';// mover hacia la derecha luego de encontrar "*"
          }  
				}else if((i+i) < (N-1)){
          for(w = i; w < j; w++){
					  matrizAuxiliar[w][j] = 'v';// mover hacia la izquierda luego de encontrar "*"
          }
				}
			}
		}
		matrizAuxiliar[j][j] = '*';// dibujar la diagonal normal 
	}
  //imprimir en pantalla
	printf("\n\t\tSOLUCION OPTIMA.\n\n");
	printf("\tMovimientos: %d\n",pCantMinima);
	printf("\tPosicion ideal: diagonal principal\n");
	imprimirMatriz(N, matrizAuxiliar);
	free(matrizAuxiliar);
  //fin de imprimir en pantalla
}

void imprimirSolucionDiagonal2(int N, int pFilaOptima, int pCantMinima, char **pMatriz){
	int i, j, w; // para llevar la cuenta de las columnas, filas y la columna principal
	char **matrizAuxiliar; //auxiliar de la matriz dinamica
	matrizAuxiliar = crearMatriz(N, matrizAuxiliar);//crea una matriz auxiliar
	//Imprimir Solucion
	for (i = 0; i < N; i++)//Recorre las filas
	{	
		for (j = 0; j < N; j++)//Recorre las columnas
		{
			matrizAuxiliar[i][j] = ' '; // poner espacios en blanco para que al imprimir se a vea lindo
		}
	}

	for (j = 0; j < N; j++)//Recorre las filas
	{	
		for (i = 0; i < N; i++)//Recorre las columnas
		{

			if(pMatriz[i][j]=='*'){// en caso que se encuentre "*" entonces se hace la sigiente logica
				if((i+j) > (N-1) && matrizAuxiliar[j][i+1] == ' '){
          for(w = j; w >= N-i; w--){
					  matrizAuxiliar[i][w] = '<'; // mover a la derecha luego de encontrar "*"
          }  
				}
				if((i+j) < (N-1)) {
          for(w = i; w < N-j; w++){
					  matrizAuxiliar[w][j] = 'v';// mover hacia abajo luego de encontrar "*"
          }
				}
				else if((i+j) > (N-1) ){
          for(w = i; w > j+1; w--){
					  matrizAuxiliar[w][j] = '^';// mover hacia arriba luego de encontrar "*"
          }  
				}
        else if((j+j) < (N-1) && matrizAuxiliar[j+1][i] == ' '){
          //for(w = j; w < N-j; w++){
					  //matrizAuxiliar[i][w] = '>'; // mover a la derecha luego de encontrar "*"
          //}  
				}
			}
			if ((i+j) == (N-1)){
				matrizAuxiliar[i][j] = '*';// poner en la matriz diagonal "*" la matriz invertida.
			}
		}		
	}
  //imprimi datos impotantes
	printf("\n\t\tSOLUCION OPTIMA.\n\n");
	printf("\tMovimientos: %d\n",pCantMinima);
	printf("\tPosicion ideal: diagonal invertida\n");
	imprimirMatriz(N, matrizAuxiliar);
	free(matrizAuxiliar);
  //fin imprimir datos importantes.
}

int* ordenarEnVertical(char **pMatriz, int N, int* pValores){ //Mejor opcion vertical
	int i, j, columnaActual, columnaOptima = 0, cantMovimientos = 0, cantMinima = 10000;

	//Recorre cada columna para ver en cual tiene menos movimientos	
	for (columnaActual = 0; columnaActual < N; columnaActual++)
	{
		//Recorre las filas del tablero
		for (i = 0; i < N; i++)
		{
			//Recorre las columnas del tablero
			for (j = 0; j < N; j++)
			{
				if(pMatriz[i][j] == '*'){ //cuando se encuentra una "*" entonces se hace la siguiente logica
					if(j<columnaActual){// si la j que cuenta la columana es menor  que la fila actual
						cantMovimientos = cantMovimientos + (columnaActual - j);	// aplica esta formula matematica
						j=N;//para salirse del ciclo
					}else{
						cantMovimientos = cantMovimientos + (j-columnaActual);	//aplica esta formula matematica				
						j=N;//para salirse del ciclo
					}
				}
			}

		}
		//Comprueba la cantidad minima por cada columna que chequea y es la que va a regresar para saber la columana 
    //con menos moviemientos 
		if(cantMinima > cantMovimientos){
			cantMinima = cantMovimientos;
			columnaOptima = columnaActual;			
		}
		cantMovimientos = 0;
	}
	//se guardan los datos en "pValores" la cantidad minima y la optima para asi tambien saber si es mejor ordenar en verticar o en diagonal
	pValores[0] = columnaOptima;
	pValores[1] = cantMinima;
	return pValores;
}

int* ordenarEnHorizontal(char **pMatriz, int N, int* pValores){ //Mejor opcion vertical
	int i, j, filaActual, filaOptima = 0, cantMovimientos = 0, cantMinima = 10000; //variables para llevar filas columnas y datos importantes

	for (filaActual = 0; filaActual < N; filaActual++) // lleva la fila a revisar
	{
		for (i = 0; i < N; i++) // fila 
		{
			for (j = 0; j < N; j++)//columna
			{
				if(pMatriz[i][j] == '*'){ // si dentro de la fila se encuentra un "*" se crea la siguiente logica
					if(j<filaActual){// esta parte es para saber si j esta nates que la fila a revisar
						if((filaActual - i) > 0){ //fi fila actual es menor que 0
							cantMovimientos = cantMovimientos + (filaActual - i);	//formula matematica para contar contidad de moviemientos
						}else{
							cantMovimientos = cantMovimientos - (filaActual - i);	//formula matematica para contar contidad de moviemientos
						}	
					}else{
						if((i-filaActual) > 0){ // si la fila actual es mayor que 0
							cantMovimientos = cantMovimientos + (i-filaActual);//formula matematica para contar contidad de moviemientos
						}else{
							cantMovimientos = cantMovimientos - (i-filaActual);	//formula matematica para contar contidad de moviemientos
						}
					}

				}
			}
		}
		//Comprueba la cantidad minima por cada columna que chequea
		if(cantMinima > cantMovimientos){//comprueba la cantidad minima de moviemientos y la va almacenando 
			cantMinima = cantMovimientos;
			filaOptima = filaActual;			
		}
		cantMovimientos = 0; // reinicia la cantidad de moviemientos para saber la minima en la siguiente iteracion
	}
	pValores[0] = filaOptima;
	pValores[1] = cantMinima;
	return pValores; // resgresa la cantidad minima de moviemientos para saber si las diagonales hacen menos o si la diagonal es la mejor.
}

int* ordenarEnDiagonal(char **pMatriz, int N, int* pValores){ //Mejor opcion vertical
	
	int i, j, posDiagonal, columnaOptima = 0, cantMovimientos = 0, cantTotal = 0; // variables para los ciclos y las cantidad de moviemientos

	//Recorre cada columna para ver en cual tiene menos movimientos	
	for (posDiagonal = 0; posDiagonal < N; posDiagonal++)
	{		
		if(pMatriz[posDiagonal][posDiagonal] != '*'){// hace la busqueda de "*" por columna
		//Recorre las filas del tablero
			for (i = 0; i < N; i++)
			{
				//Recorre las columnas del tablero
				for (j = 0; j < N; j++)
				{
					if( i != j){
						if(i == posDiagonal || j == posDiagonal){// verifica la diagonal
							if(pMatriz[i][j] == '*'){//hace las busquedas por columna
								if(j<posDiagonal){							
									cantMovimientos = cantMovimientos + (posDiagonal - j);// formula matematica para saber la cantidad de movimientos
									pMatriz[i][j] = ' ';
								}else{
									if((j - posDiagonal) == 0){// verifica la diagonal
										if(i<posDiagonal){
											cantMovimientos = cantMovimientos + (posDiagonal - i);	// formula matematica para saber la cantidad de movimientos
											pMatriz[i][j] = ' ';// rellena la matriz con espacios vacios
										}else{
											cantMovimientos = cantMovimientos + (i-posDiagonal);// formula matematica para saber la cantidad de movimientos
											pMatriz[i][j] = ' ';// rellena la matriz con espacios vacios
										}
									}else{
										cantMovimientos = cantMovimientos + (j-posDiagonal);// formula matematica para saber la cantidad de movimientos
										pMatriz[i][j] = ' ';	// rellena la matriz con espacios vacios	
									}
								}
							}
						}
					}
				}
			}
		}
		//Suma la cantidad de movimientos
		cantTotal = cantTotal + cantMovimientos;	// contiene la cantidad total minima de moviemeitos
		cantMovimientos = 0; // reinicia los movimientos para saber en la sigueitne iteracion la minimas
	}

	pValores[0] = columnaOptima;
	pValores[1] = cantTotal;
	return pValores; //regres la columna optima, y la cantidad minima de moviemientos
}

int* ordenarEnDiagonal2(char **pMatriz, int N, int* pValores){ //Mejor opcion vertical
	
	int i, j, k, l, posDiagonal, columnaOptima = 0, cantMovimientos = 0, cantTotal = 0, hola=1;// variables para los ciclos y las cantidad de moviemientos

	//Recorre cada columna para ver en cual tiene menos movimientos	
	for (k = 0; k < N; k++)
	{
		for (l = 0; l < N; l++)
		{
			if((k + l) == (N-1)){//Entra en cada posicion de la diagonal invertida

				if(pMatriz[k][l] != '*'){
				//Recorre las filas del tablero
					for (i = 0; i < N; i++)
					{
						//Recorre las columnas del tablero
						for (j = 0; j < N; j++)
						{
							if((i + j) != (N-1)){//&& ((j==l) || (i==k))){//Valida que no sea una casilla de la diagonal invertida
								if((j==l) || (i==k)){
									//printf("k:%d, l:%d, i:%d, j:%d\n", k, l, i, j);									
									if(pMatriz[i][j] == '*'){//hace las busquedas por columna
																		
										if(j < l){
											cantMovimientos = cantMovimientos + (l - j);// formula matematica para saber la cantidad de movimientos
											pMatriz[i][j] = ' ';	// rellena la matriz con espacios vacios	
										}else {
											if((j-l)==0){
												if(i<k){
													cantMovimientos = cantMovimientos + (k - i);	// formula matematica para saber la cantidad de movimientos
													pMatriz[i][j] = ' ';	// rellena la matriz con espacios vacios	
												}else{
													cantMovimientos = cantMovimientos + (i - k);// formula matematica para saber la cantidad de movimientos
													pMatriz[i][j] = ' ';													
												}
											}else{
												cantMovimientos = cantMovimientos + (j - l);// formula matematica para saber la cantidad de movimientos
												pMatriz[i][j] = ' ';	// rellena la matriz con espacios vacios		
											}												
										}											
									}
								}
							}								
						}
					}
				}
			}
		}
		//Suma la cantidad de movimientos
		cantTotal = cantTotal + cantMovimientos; // contiene la cantidad total minima de moviemeitos				
		cantMovimientos = 0; // reinicia los moviemientos 
	}
	pValores[0] = columnaOptima; // guarda la columna donde se hicieron menos moviemientos
	pValores[1] = cantTotal; // cantidadd total de moviemientos
	return pValores; // regresa los valores minimos y optimos de moviemientos
}

char** calcularOrdenFichas(char **pMatriz, int N, int *pFichas){  // Verifica si es horizontal o vertical.
	
	int i, j, p, verHor = 0; // si es vertical verhor = 1 // si es horizontal verhor = 2 // en caso que se puedan ambas seria 0
	int *valoresVertical;
	valoresVertical = (int *)malloc (2*sizeof(int)); // para guardar los valores que retorna el ordenarEnVertical
	int *valoresHorizontal;
	valoresHorizontal = (int *)malloc (2*sizeof(int));// para guardar los valores que retorna el ordenarEnHorizontal
	int *valoresDiagonal;
	valoresDiagonal = (int *)malloc (2*sizeof(int));// para guardar los valores que retorna el ordenarEnDiagonal
	int *valoresDiagonal2;
	valoresDiagonal2 = (int *)malloc (2*sizeof(int));// para guardar los valores que retorna el ordenarEnDiagonal2
	
  //el siguiente codigo contiene la logica para saber si la matriz ingresada es vertical o horizonta
  
	for(i=0;i<N-1;i++){// recorre filas
		for(j=0;j<N;j++){ // recorre columnas
			
			for(p=1;p<N-i;p++){ // hace un for que recorre filas en la posicion de J para validadr si es vertical
				if(pMatriz[i][j] == pMatriz[i+p][j] && pMatriz[i][j] != 0){//Verifica el vertical
					
						verHor = 1; // en caso que es vertical guarda un 2
				}
			}

			for(p=1;p<N;p++){// hace un for que recorre columnas en la posicion de i para validadr si es horizontal
				if(j+1<N){// condicion extra para que cuando llegue a la ultima posicion no de error
					if(pMatriz[i][j] == pMatriz[i][j+p] && pMatriz[i][j] != 0){//Verifica el horizontal
						
							verHor = 2; // en caso que es horizontal guarda un 1
					}
				}
			}
		}
	}
	//validar cual es la funcion que debe mover.

	if(verHor == 1){//ORDENA VERTICALMENTE
    // Verifia primero si es vertical, pero igualmente busca en las diagonales para ver si tambien se puede solucionar de 
    // forma diagonal normal o diagonal invertidapara saber si son diagonales primero se pueden ordenar, y luego deben de tener menos moviemeientos
    valoresVertical = ordenarEnVertical(pMatriz, N, valoresVertical);
		asignarFichas(pMatriz, pFichas, N);
    // ver si es diagonal norma
		valoresDiagonal = ordenarEnDiagonal(pMatriz, N, valoresDiagonal);
		asignarFichas(pMatriz, pFichas, N);
    //ver si es diagonal invertida 
		valoresDiagonal2 = ordenarEnDiagonal2(pMatriz, N, valoresDiagonal2);
		asignarFichas(pMatriz, pFichas, N);

		if( valoresVertical[1]<=valoresDiagonal[1] && valoresVertical[1]<=valoresDiagonal2[1]){//Compara si es mejor ordenar en vertical u horizontal
				
				imprimirSolucionVertical(N, valoresVertical[0], valoresVertical[1], pMatriz);
		}else if(valoresDiagonal[1]<=valoresVertical[1] && valoresDiagonal[1]<=valoresDiagonal2[1]){//Compara si es mejor ordenar en vertical u horizontal
				
				imprimirSolucionDiagonal(N, valoresDiagonal[0], valoresDiagonal[1], pMatriz);

		}else if(valoresDiagonal2[1]<=valoresVertical[1] && valoresDiagonal2[1]<=valoresDiagonal[1]){//Compara si es mejor ordenar en vertical u horizontal
				
				imprimirSolucionDiagonal2(N, valoresDiagonal2[0], valoresDiagonal2[1], pMatriz);
		}
		
	}else if(verHor == 2){//ORDENA HORIZONTALMENTE
    // Verifia primero si es horizontal, pero igualmente busca en las diagonales para ver si tambien se puede solucionar de 
    // forma diagonal normal o diagonal invertida, para saber si son diagonales primero se pueden ordenar, y luego deben de tener menos moviemeientos 
		valoresHorizontal = ordenarEnHorizontal(pMatriz, N, valoresHorizontal);
		asignarFichas(pMatriz, pFichas, N);
		valoresDiagonal = ordenarEnDiagonal(pMatriz, N, valoresDiagonal);
		asignarFichas(pMatriz, pFichas, N);
		valoresDiagonal2 = ordenarEnDiagonal2(pMatriz, N, valoresDiagonal2);
		asignarFichas(pMatriz, pFichas, N);
    

		if( valoresHorizontal[1]<=valoresDiagonal[1] && valoresHorizontal[1]<=valoresDiagonal2[1]){//Compara si es mejor ordenar en vertical u horizontal
				
				imprimirSolucionHorizontal(N, valoresHorizontal[0], valoresHorizontal[1], pMatriz);

		}else if(valoresDiagonal[1]<=valoresHorizontal[1]	&& valoresDiagonal[1]<=valoresDiagonal2[1]){//Compara si es mejor ordenar en vertical u horizontal
				
				imprimirSolucionDiagonal(N, valoresDiagonal[0], valoresDiagonal[1], pMatriz);

		}else if(valoresDiagonal2[1]<=valoresHorizontal[1] && valoresDiagonal2[1]<=valoresDiagonal[1]){//Compara si es mejor ordenar en vertical u horizontal
				
				imprimirSolucionDiagonal2(N, valoresDiagonal2[0], valoresDiagonal2[1], pMatriz);
		}
		
	}
	
	free(valoresVertical);// se libera espacios en memoria
	free(valoresHorizontal);// se libera espacio en memoria
	free(valoresDiagonal);//se libera espacio en memoria
  free(valoresDiagonal2);//se libera espacio en memoria
	return pMatriz;
}

int resolverMatriz(int N, int * fichas){
	//int error;
	char **matriz; //tablero dinámico

	matriz = crearMatriz(N, matriz);

	if(asignarFichas(matriz, fichas, N) == 1)
		return 1;

	printf("\n\t\t MATRIZ INICIAL\n");

	imprimirMatriz(N, matriz);

	calcularOrdenFichas(matriz,N,fichas);	
	return 0;
}

int main ( void ){
	static const char filename[] = "PruebasGrupo04.txt";
	FILE *file = fopen ( filename, "r" );
	int count = 0;
	int errorLectura = 0;

	if (cantidadPruebas(&count) == 1)
	{	
		int i, j;
		int matricesTamano [ count ];
		int **fichasPosicion;
		char informacion [count] [ 180 ];
		//Llama a la lectora de archivos
		leerArchivo(informacion);
		if(validarArchivo(count, informacion) == 1)
		{
			count = count / 2;
			
			fichasPosicion = tamanoMatriz(count, informacion, matricesTamano, fichasPosicion, &errorLectura);
 
			if(errorLectura == 0)	
			{
				for ( i = 0; i < count; i++)
				{
					printf("\tTABLERO %d \n", i+1);
			  		if(resolverMatriz(matricesTamano[i], fichasPosicion[i]) == 1)
			  		{
			  			printf("Asignacion de fichas en una misma posicion \n");	
			  			printf("Modifique el archivo PruebasGrupo04.txt conforme al formato establecido\n");	  		
			  			return 0;
			  		}
				}
			}
			else
			{
				printf ("Formato incorrecto\n");
				printf("Modifique el archivo PruebasGrupo04.txt conforme al formato establecido\n");	
			}
		}
		else
		{
			printf ("Formato incorrecto\n");
			printf("Modifique el archivo PruebasGrupo04.txt conforme al formato establecido\n");	
		}
	}  
	else
	{
		printf ("Archivo no encontrado\n");
		printf("Modifique el archivo PruebasGrupo04.txt conforme al formato establecido\n");
	}	
	return 0;
}