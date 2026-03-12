/**
 *----------------------------------------
 * primesThreadPool.cpp
 * ---------------------------------------
 * UNIVERSIDAD DEL VALLE DE GUATEMALA
 * CC3064 - Sistemas Operativos
 * Fecha: 2026/02/06
 * References:
 * "Predicates" - The C++ Standard Library (Josuttis)
 * pp.226-227
 * "Shared Objects and Synchronization" - The Art of 
 * Multiprocessor Programming (Herlihy & Shavit)
 * pp.3-5
 * ---------------------------------------
 * Imprime los números primos en un rango de 10^n
 * Implementación usando thread pool para mejor eficiencia
 * ----------------------------------------
 */

#include <iostream> 
#include <thread>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

// Test if number is prime
bool isPrime (int number)
{
	if (number == 0 || number == 1)
	{
		return false;
	}
	int divisor;
	for (divisor = number/2; number%divisor != 0; --divisor)
	{
		;
	}
	if (divisor != 1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// Sequential test of primes with thread ID for identification
void primeTest(int thisNumber, int thisLimit, int threadId)
{
	// Acumular resultados en un string para evitar salida mezclada
	string resultado = "";
	resultado += "=== THREAD " + to_string(threadId) + " INICIADO ===\n";
	resultado += "Rango asignado: [" + to_string(thisNumber+1) + " - " + to_string(thisLimit) + "]\n";
	resultado += "Numeros primos encontrados:\n";
	
	int contador = 0;
	string primosLinea = "";
	
	while (thisNumber < thisLimit)
	{
		thisNumber++;
		if(isPrime(thisNumber))
		{
			contador++;
			primosLinea += to_string(thisNumber) + " ";
			
			// Cada 8 números, nueva línea para mejor legibilidad
			if(contador % 8 == 0)
			{
				resultado += primosLinea + "\n";
				primosLinea = "";
			}
		}
	}
	
	// Agregar números restantes si los hay
	if(!primosLinea.empty())
	{
		resultado += primosLinea + "\n";
	}
	
	resultado += "Total encontrados: " + to_string(contador) + "\n";
	resultado += "=== THREAD " + to_string(threadId) + " TERMINADO ===\n\n";
	
	// Imprimir todo de una vez para evitar mezcla de salidas
	cout << resultado;
}


int main()
{
	int exponente = 2;
	char expChar;
	cout << "Ingrese el exponente x para calcular el limite superior 10^x." << endl;
    cout << "Ejemplo: si ingresa 3, se buscaran primos entre 0 y 1000 (10^3)." << endl;
	cout << "x = ";
	cin>>expChar;
	
	//Numbers in ASCII begin at decimal 48
	exponente = int(expChar) - 48;
	
	long limit = pow(10,exponente);
	cout<<endl;
	cout<<"Se buscaran numeros primos en el rango [0, " <<limit<< "].";
	cout << "\n=== INICIANDO CALCULO CON THREAD POOL ===" << endl;
	cout<<endl;
	
	// Determinar número de threads (núcleos disponibles)
	unsigned int numThreads = std::thread::hardware_concurrency();
	if (numThreads == 0) numThreads = 4; // fallback si no se puede detectar
	
	cout << ">>> CONFIGURACION DEL THREAD POOL <<<" << endl;
	cout << "Nucleos detectados: " << numThreads << endl;
	cout << "Threads creados: " << numThreads << endl;
	cout << "Rango total: [1 - " << limit << "]" << endl;
	cout << "Numeros por thread: ~" << (limit / numThreads) << endl << endl;
	
	// Crear vector para almacenar los threads
	std::vector<std::thread> threadPool;
	
	// Calcular el tamaño de cada segmento
	long segmentSize = limit / numThreads;
	long remainder = limit % numThreads;
	
	// Crear y lanzar threads
	for (unsigned int i = 0; i < numThreads; ++i)
	{
		long startRange = i * segmentSize;
		long endRange = (i + 1) * segmentSize;
		
		// El último thread toma los números restantes
		if (i == numThreads - 1)
		{
			endRange += remainder;
		}
		
		// Crear thread y agregarlo al pool
		threadPool.emplace_back(primeTest, startRange, endRange, i+1);
	}
	
	// Esperar a que todos los threads terminen
	for (auto& t : threadPool)
	{
		t.join();
	}
	
	cout << "=== RESUMEN FINAL ===" << endl;
	cout << "Procesamiento completado exitosamente" << endl;
	cout << "Todos los " << numThreads << " threads han terminado" << endl;
	cout << "Rango total procesado: [1 - " << limit << "]" << endl;
	
	return 0;
}