/**
 *----------------------------------------
 * primePrt.cpp
 * ---------------------------------------
 * UNIVERSIDAD DEL VALLE DE GUATEMALA
 * CC3056 - Programacion de Microprocesadores
 * Fecha:  2026/01/18
 * References:
 * "Predicates" - The C++ Standard Library (Josuttis)
 * pp.226-227
 * "Shared Objects and Synchronization" - The Art of 
 * Multiprocessor Programming (Herlihy & Shavit)
 * pp.3-5
 *
 * ---------------------------------------
 * Imprime los números primos en un rango de 10^n
 * Implementación simplista usando pthreads
 * ----------------------------------------
 */

#include <iostream>    
#include <cmath> 		
#include <pthread.h>	
#include <ctime>

using namespace std;

#define THREADS 4

//Prototipo función identificadora
bool isPrime(int number);

//Prototipo rutina para hilos
void *primeTest (void *thisNumber);

/*
 * MAIN
 */
int main()
{
    
    clock_t tiempo_inicial;
    clock_t tiempo_final;
    
    double tiempo_total;
    
    
	int exponente, numThreads = THREADS;

	cout<<"-----------------------------------------------\n";
	cout<<"Impresión de los numeros primos menores a 10^n\n";
	cout<<"-----------------------------------------------\n";
	cout<<"Ingrese el valor del exponente para base 10: ";


	cin>>exponente;
	
	cout<<"Ingrese el numero de threads: ";
	cin>>numThreads;
	
	long limit = pow(10,exponente);
	
	cout<<"Buscando primos entre 0 y "<<limit<<"...";
	cout<<endl;
	
	long i = 0;
	long j = 0; 		
	int rc;

	pthread_t threadID;
	

    
    tiempo_inicial = clock();
    
	for(i=0; i<limit;)
	{
		//Asignamos alternamente numeros a cada hilo
        for(j = 0; j<numThreads; j++)	
		{
			rc = pthread_create(&threadID,NULL,primeTest,(void *)i);
			if(rc)
			{
				cout<<"No se pudo crear el hilo "<<j<<endl;
				exit(-1);
			}

			i++;
		}	
		
	}
	for(j = 0; j<numThreads; j++)	
	{
		rc = pthread_join(threadID,NULL);
		if(rc)
		{
			cout<<"No se pudo unir el hilo "<<j<<endl;
			exit(-1);
		}
		i++;
		}
	
	tiempo_final = clock();
	tiempo_total = difftime(tiempo_final, tiempo_inicial)/CLOCKS_PER_SEC;
		
	cout<<"Tiempo total transcurrido: \n"<<tiempo_total;
		

	pthread_exit(NULL);
	
	return 0;
}

/*
 * primeTest
 */
void *primeTest (void *thisNumber)
{

	long intNumber = (long)thisNumber;

    //Acciones solamente si el numero es primo
	if(isPrime(intNumber))
	{
		cout<<intNumber<<"from trhead num: " << pthread_self() <<endl;
	}
	pthread_exit(NULL);
}

/*
 * isPrime - function
 */
bool isPrime (int number)
{
	if (number == 0 || number == 1)
	{
		return false;
	}
	int divisor;
	for (divisor = number/2; number%divisor != 0; --divisor)
	{
		; //Algoritmo rapido para encontrar el residuo de un primo
	}
	if (divisor != 1)
	{
		return false;  //El residuo <> 1 si el numero NO es primo
	}
	else
	{
		return true;  //El residuo es 1 si el numero es primo
	}
}