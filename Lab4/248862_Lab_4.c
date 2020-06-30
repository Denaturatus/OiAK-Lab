#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// zmienne potrzebne do mierzenia czasu wykonywania operacji arytmetycznych
clock_t startMeasurement, endMeasurement;
double totalTime;

// wektor 128-bitowy przechowujacy 32-bitowe liczby zmiennoprzecinkowe
typedef struct Vector128{
    float firstNumber, secondNumber, thirdNumber, fourthNumber;
}Vector128;

// funkcja generujca pseudo-losowe wartosci dla liczb w wektorze w tablicy wektorow
void fillArrayOfVectorsWithRandomValues(Vector128* array)
{
    for(int i = 0; i < 8192; i++)
    {
        array[i].firstNumber = rand() % 999;
        array[i].secondNumber = rand() % 999;
        array[i].thirdNumber = rand() % 999;
        array[i].fourthNumber = rand() % 999;
    }
}

// szczegolowy opis implementacji operacji arytmetycznych dla SISD i SIMD znajduje sie w sprawozdaniu
// SISD 
// operacja dodawania dwoch liczb zmiennoprzecinkowych w architekturze SISD
double additionSISD(float* a, float* b)
{
	float sum;
	startMeasurement = clock();
	asm(
		"fld %1 \n\t"
		"fadd %2 \n\t"
		:"=t"(sum)
		:"m"(*a), "m"(*b)
		);
	endMeasurement = clock();
	totalTime = ((double)(endMeasurement - startMeasurement)) / CLOCKS_PER_SEC;
	return totalTime;
}

// operacja odejmowania dwoch liczb zmiennoprzecinkowych w architekturze SISD
double subtractionSISD(float* a, float* b)
{
	float difference;
	startMeasurement = clock();
	asm(
		"fld %1 \n\t"
		"fsub %2 \n\t"
		:"=t"(difference)
		:"m"(*a), "m"(*b)
		);
	endMeasurement = clock();
	totalTime = ((double)(endMeasurement - startMeasurement)) / CLOCKS_PER_SEC;
	return totalTime;
}

// operacja mnozenia dwoch liczb zmiennoprzecinkowych w architekturze SISD
double multiplicationSISD(float* a, float* b)
{
	float product;
	startMeasurement = clock();
	asm(
		"fld %1 \n\t"
		"fmul %2 \n\t"
		:"=t"(product)
		:"m"(*a), "m"(*b)
		);
	endMeasurement = clock();
	totalTime = ((double)(endMeasurement - startMeasurement)) / CLOCKS_PER_SEC;
	return totalTime;
}

// operacja dzielenia dwoch liczb zmiennoprzecinkowych w architekturze SISD
double divisionSISD(float* a, float* b)
{
	float quotient;
	startMeasurement = clock();
	asm(
		"fld %1 \n\t"
		"fdiv %2 \n\t"
		:"=t"(quotient)
		:"m"(*a), "m"(*b)
		);
	endMeasurement = clock();
	totalTime = ((double)(endMeasurement - startMeasurement)) / CLOCKS_PER_SEC;
	return totalTime;
}

// SIMD
// operacja dodawania liczb w dwoch wektorach w architekturze SIMD
double additionSIMD(Vector128* v1, Vector128* v2)
{
	Vector128 sum;   
	startMeasurement = clock();
	asm(
		"movq %1, %%rax \n\t"
		"movups (%%rax), %%xmm0 \n\t"
		"movq %2, %%rax \n\t"
		"movups (%%rax), %%xmm1 \n\t"
		"addps %%xmm1, %%xmm0 \n\t"
		"movups %%xmm0, %0 \n\t"
		:"=m"(sum)
		:"m"(v1), "m"(v2)
		:"rax");
	endMeasurement = clock();
	totalTime = ((double)(endMeasurement - startMeasurement)) / CLOCKS_PER_SEC;
	return totalTime;
}

// operacja odejmowania liczb w dwoch wektorach w architekturze SIMD
double subtractionSIMD(Vector128* v1, Vector128* v2)
{
	Vector128 difference;   
	startMeasurement = clock();
	asm(
		"movq %1, %%rax \n\t"
		"movups (%%rax), %%xmm0 \n\t"
		"movq %2, %%rax \n\t"
		"movups (%%rax), %%xmm1 \n\t"
		"subps %%xmm1, %%xmm0 \n\t"
		"movups %%xmm0, %0 \n\t"
		:"=m"(difference)
		:"m"(v1), "m"(v2)
		:"rax");
	endMeasurement = clock();
	totalTime = ((double)(endMeasurement - startMeasurement)) / CLOCKS_PER_SEC;
	return totalTime;
}

// operacja mnozenia liczb w dwoch wektorach w architekturze SIMD
double multiplicationSIMD(Vector128* v1, Vector128* v2)
{
	Vector128 product;  
	startMeasurement = clock();
	asm(
		"movq %1, %%rax \n\t"
		"movups (%%rax), %%xmm0 \n\t"
		"movq %2, %%rax \n\t"
		"movups (%%rax), %%xmm1 \n\t"
		"mulps %%xmm1, %%xmm0 \n\t"
		"movups %%xmm0, %0 \n\t"
		:"=m"(product)
		:"m"(v1), "m"(v2)
		:"rax");
	endMeasurement = clock();
	totalTime = ((double)(endMeasurement - startMeasurement)) / CLOCKS_PER_SEC;
	return totalTime;
}

// operacja dzielenia liczb w dwoch wektorach w architekturze SIMD
double divisionSIMD(Vector128* v1, Vector128* v2)
{
	Vector128 quotient;   
	startMeasurement = clock();
	asm(
		"movq %1, %%rax \n\t"
		"movups (%%rax), %%xmm0 \n\t"
		"movq %2, %%rax \n\t"
		"movups (%%rax), %%xmm1 \n\t"
		"divps %%xmm1, %%xmm0 \n\t"
		"movups %%xmm0, %0 \n\t"
		:"=m"(quotient)
		:"m"(v1), "m"(v2)
		:"rax");
	endMeasurement = clock();
    totalTime = ((double)(endMeasurement - startMeasurement)) / CLOCKS_PER_SEC;
	return totalTime;
}

int main ()
{
    // inicjalizacja zegara potrzebnego do generowania pseudo-losowych liczb
    srand(time(NULL)); 
    // zmienna okreslajaca ilosc powtorzen pomiarow czasu wykonywania operacji arytmetycznych
    int reps = 10;
    // utworzenie tabic wektorow
    Vector128 vectorArray1[8192];
    Vector128 vectorArray2[8192];
    // deklaracja zmiennych indeksowych for-loopow
    int i, j;

    // zmienne przechowujace sredni czas wykonania poszczegolnych operacji 
    double avgAdditionTimeSISD = 0, avgSubtractionTimeSISD = 0, avgMultiplicationTimeSISD = 0, avgDivisionTimeSISD = 0;
    double avgAdditionTimeSIMD = 0, avgSubtractionTimeSIMD = 0, avgMultiplicationTimeSIMD = 0, avgDivisionTimeSIMD = 0;
    // zmienna ograniczajaca ilosc liczb na ktorych zostana wykonane operacje arytmetyczne w danej petli pomiarowej
    int numberAmount = 2048;

    // zmienna odpowiedzialna za wpisywanie do pliku
    FILE *file;
    // otworzenie pliku
	file = fopen("measurementResults.txt", "w");

    // petla pomiarow czasu wykonywania operacji dla 2048 wektorow w tablicach
    for(i = 0; i < reps; i++)
    {
        // wypelnienie liczb w wektorach pseudo-losowymi wartosciami
        fillArrayOfVectorsWithRandomValues(vectorArray1);
        fillArrayOfVectorsWithRandomValues(vectorArray2);
        //pomiary czasow
        for(j = 0; j < numberAmount; j++)
        {
            avgAdditionTimeSISD += additionSISD(&vectorArray1[j].firstNumber, &vectorArray2[j].firstNumber);
            avgAdditionTimeSISD += additionSISD(&vectorArray1[j].secondNumber, &vectorArray2[j].secondNumber);
            avgAdditionTimeSISD += additionSISD(&vectorArray1[j].thirdNumber, &vectorArray2[j].thirdNumber);
            avgAdditionTimeSISD += additionSISD(&vectorArray1[j].fourthNumber, &vectorArray2[j].fourthNumber);

            avgAdditionTimeSIMD += additionSIMD(&vectorArray1[j], &vectorArray2[j]);
        } 
        for(j = 0; j < numberAmount; j++)
        {
            avgSubtractionTimeSISD += subtractionSISD(&vectorArray1[j].firstNumber, &vectorArray2[j].firstNumber);
            avgSubtractionTimeSISD += subtractionSISD(&vectorArray1[j].secondNumber, &vectorArray2[j].secondNumber);
            avgSubtractionTimeSISD += subtractionSISD(&vectorArray1[j].thirdNumber, &vectorArray2[j].thirdNumber);
            avgSubtractionTimeSISD += subtractionSISD(&vectorArray1[j].fourthNumber, &vectorArray2[j].fourthNumber);

            avgSubtractionTimeSIMD += subtractionSIMD(&vectorArray1[j], &vectorArray2[j]);
        }
        for(j = 0; j < numberAmount; j++)
        {
            avgMultiplicationTimeSISD += multiplicationSISD(&vectorArray1[j].firstNumber, &vectorArray2[j].firstNumber);
            avgMultiplicationTimeSISD += multiplicationSISD(&vectorArray1[j].secondNumber, &vectorArray2[j].secondNumber);
            avgMultiplicationTimeSISD += multiplicationSISD(&vectorArray1[j].thirdNumber, &vectorArray2[j].thirdNumber);
            avgMultiplicationTimeSISD += multiplicationSISD(&vectorArray1[j].fourthNumber, &vectorArray2[j].fourthNumber);

            avgMultiplicationTimeSIMD += multiplicationSIMD(&vectorArray1[j], &vectorArray2[j]);
        }
        for(j = 0; j < numberAmount; j++)
        {
            avgDivisionTimeSISD += divisionSISD(&vectorArray1[j].firstNumber, &vectorArray2[j].firstNumber);
            avgDivisionTimeSISD += divisionSISD(&vectorArray1[j].secondNumber, &vectorArray2[j].secondNumber);
            avgDivisionTimeSISD += divisionSISD(&vectorArray1[j].thirdNumber, &vectorArray2[j].thirdNumber);
            avgDivisionTimeSISD += divisionSISD(&vectorArray1[j].fourthNumber, &vectorArray2[j].fourthNumber);

            avgDivisionTimeSIMD += divisionSIMD(&vectorArray1[j], &vectorArray2[j]);
        }
    }
    // zapis do pliku, czas zostaje tutaj podzielony przez ilosc powtorzen obiegu petli, zeby otrzymac czas sredni
    fprintf(file, "Typ obliczen: SISD\nLiczba liczb: %d\nSredni czas [s]:\n", numberAmount);
    fprintf(file, "+ %lf\n", avgAdditionTimeSISD / reps);
    fprintf(file, "- %lf\n", avgSubtractionTimeSISD / reps);
    fprintf(file, "* %lf\n", avgMultiplicationTimeSISD / reps);
    fprintf(file, "/ %lf\n\n", avgDivisionTimeSISD/reps);

    fprintf(file, "Typ obliczen: SIMD\nLiczba liczb: %d\nSredni czas [s]:\n", numberAmount);
    fprintf(file, "+ %lf\n", avgAdditionTimeSIMD / reps);
    fprintf(file, "- %lf\n", avgSubtractionTimeSIMD / reps);
    fprintf(file, "* %lf\n", avgMultiplicationTimeSIMD / reps);
    fprintf(file, "/ %lf\n\n", avgDivisionTimeSIMD / reps);

    // ponowna inicjalizacja zmiennych przechowujacych sredni czas wykonania poszczegolnych operacji
    avgAdditionTimeSISD = 0, avgSubtractionTimeSISD = 0, avgMultiplicationTimeSISD = 0, avgDivisionTimeSISD = 0;
    avgAdditionTimeSIMD = 0, avgSubtractionTimeSIMD = 0, avgMultiplicationTimeSIMD = 0, avgDivisionTimeSIMD = 0;
    // ustawienie zmiennej ograniczajaca ilosc liczb
    numberAmount = 4096;

    // petla pomiarow czasu wykonywania operacji dla 4096 wektorow w tablicach
    for(i = 0; i < reps; i++)
    {
        // wypelnienie liczb w wektorach pseudo-losowymi wartosciami
        fillArrayOfVectorsWithRandomValues(vectorArray1);
        fillArrayOfVectorsWithRandomValues(vectorArray2);
        //pomiary czasow
        for(j = 0; j < numberAmount; j++)
        {
            avgAdditionTimeSISD += additionSISD(&vectorArray1[j].firstNumber, &vectorArray2[j].firstNumber);
            avgAdditionTimeSISD += additionSISD(&vectorArray1[j].secondNumber, &vectorArray2[j].secondNumber);
            avgAdditionTimeSISD += additionSISD(&vectorArray1[j].thirdNumber, &vectorArray2[j].thirdNumber);
            avgAdditionTimeSISD += additionSISD(&vectorArray1[j].fourthNumber, &vectorArray2[j].fourthNumber);

            avgAdditionTimeSIMD += additionSIMD(&vectorArray1[j], &vectorArray2[j]);
        } 
        for(j = 0; j < numberAmount; j++)
        {
            avgSubtractionTimeSISD += subtractionSISD(&vectorArray1[j].firstNumber, &vectorArray2[j].firstNumber);
            avgSubtractionTimeSISD += subtractionSISD(&vectorArray1[j].secondNumber, &vectorArray2[j].secondNumber);
            avgSubtractionTimeSISD += subtractionSISD(&vectorArray1[j].thirdNumber, &vectorArray2[j].thirdNumber);
            avgSubtractionTimeSISD += subtractionSISD(&vectorArray1[j].fourthNumber, &vectorArray2[j].fourthNumber);

            avgSubtractionTimeSIMD += subtractionSIMD(&vectorArray1[j], &vectorArray2[j]);
        }
        for(j = 0; j < numberAmount; j++)
        {
            avgMultiplicationTimeSISD += multiplicationSISD(&vectorArray1[j].firstNumber, &vectorArray2[j].firstNumber);
            avgMultiplicationTimeSISD += multiplicationSISD(&vectorArray1[j].secondNumber, &vectorArray2[j].secondNumber);
            avgMultiplicationTimeSISD += multiplicationSISD(&vectorArray1[j].thirdNumber, &vectorArray2[j].thirdNumber);
            avgMultiplicationTimeSISD += multiplicationSISD(&vectorArray1[j].fourthNumber, &vectorArray2[j].fourthNumber);

            avgMultiplicationTimeSIMD += multiplicationSIMD(&vectorArray1[j], &vectorArray2[j]);
        }
        for(j = 0; j < numberAmount; j++)
        {
            avgDivisionTimeSISD += divisionSISD(&vectorArray1[j].firstNumber, &vectorArray2[j].firstNumber);
            avgDivisionTimeSISD += divisionSISD(&vectorArray1[j].secondNumber, &vectorArray2[j].secondNumber);
            avgDivisionTimeSISD += divisionSISD(&vectorArray1[j].thirdNumber, &vectorArray2[j].thirdNumber);
            avgDivisionTimeSISD += divisionSISD(&vectorArray1[j].fourthNumber, &vectorArray2[j].fourthNumber);

            avgDivisionTimeSIMD += divisionSIMD(&vectorArray1[j], &vectorArray2[j]);
        }
    }
    // zapis do pliku, czas zostaje tutaj podzielony przez ilosc powtorzen obiegu petli, zeby otrzymac czas sredni
    fprintf(file, "Typ obliczen: SISD\nLiczba liczb: %d\nSredni czas [s]:\n", numberAmount);
    fprintf(file, "+ %lf\n", avgAdditionTimeSISD / reps);
    fprintf(file, "- %lf\n", avgSubtractionTimeSISD / reps);
    fprintf(file, "* %lf\n", avgMultiplicationTimeSISD / reps);
    fprintf(file, "/ %lf\n\n", avgDivisionTimeSISD/reps);

    fprintf(file, "Typ obliczen: SIMD\nLiczba liczb: %d\nSredni czas [s]:\n", numberAmount);
    fprintf(file, "+ %lf\n", avgAdditionTimeSIMD / reps);
    fprintf(file, "- %lf\n", avgSubtractionTimeSIMD / reps);
    fprintf(file, "* %lf\n", avgMultiplicationTimeSIMD / reps);
    fprintf(file, "/ %lf\n\n", avgDivisionTimeSIMD / reps);

    // ponowna inicjalizacja zmiennych przechowujacych sredni czas wykonania poszczegolnych operacji
    avgAdditionTimeSISD = 0, avgSubtractionTimeSISD = 0, avgMultiplicationTimeSISD = 0, avgDivisionTimeSISD = 0;
    avgAdditionTimeSIMD = 0, avgSubtractionTimeSIMD = 0, avgMultiplicationTimeSIMD = 0, avgDivisionTimeSIMD = 0;
    // ustawienie zmiennej ograniczajaca ilosc liczb
    numberAmount = 8192;

    // petla pomiarow czasu wykonywania operacji dla 8192 wektorow w tablicach
    for(i = 0; i < reps; i++)
    {
        // wypelnienie liczb w wektorach pseudo-losowymi wartosciami
        fillArrayOfVectorsWithRandomValues(vectorArray1);
        fillArrayOfVectorsWithRandomValues(vectorArray2);
        //pomiary czasow
        for(j = 0; j < numberAmount; j++)
        {
            avgAdditionTimeSISD += additionSISD(&vectorArray1[j].firstNumber, &vectorArray2[j].firstNumber);
            avgAdditionTimeSISD += additionSISD(&vectorArray1[j].secondNumber, &vectorArray2[j].secondNumber);
            avgAdditionTimeSISD += additionSISD(&vectorArray1[j].thirdNumber, &vectorArray2[j].thirdNumber);
            avgAdditionTimeSISD += additionSISD(&vectorArray1[j].fourthNumber, &vectorArray2[j].fourthNumber);

            avgAdditionTimeSIMD += additionSIMD(&vectorArray1[j], &vectorArray2[j]);
        } 
        for(j = 0; j < numberAmount; j++)
        {
            avgSubtractionTimeSISD += subtractionSISD(&vectorArray1[j].firstNumber, &vectorArray2[j].firstNumber);
            avgSubtractionTimeSISD += subtractionSISD(&vectorArray1[j].secondNumber, &vectorArray2[j].secondNumber);
            avgSubtractionTimeSISD += subtractionSISD(&vectorArray1[j].thirdNumber, &vectorArray2[j].thirdNumber);
            avgSubtractionTimeSISD += subtractionSISD(&vectorArray1[j].fourthNumber, &vectorArray2[j].fourthNumber);

            avgSubtractionTimeSIMD += subtractionSIMD(&vectorArray1[j], &vectorArray2[j]);
        }
        for(j = 0; j < numberAmount; j++)
        {
            avgMultiplicationTimeSISD += multiplicationSISD(&vectorArray1[j].firstNumber, &vectorArray2[j].firstNumber);
            avgMultiplicationTimeSISD += multiplicationSISD(&vectorArray1[j].secondNumber, &vectorArray2[j].secondNumber);
            avgMultiplicationTimeSISD += multiplicationSISD(&vectorArray1[j].thirdNumber, &vectorArray2[j].thirdNumber);
            avgMultiplicationTimeSISD += multiplicationSISD(&vectorArray1[j].fourthNumber, &vectorArray2[j].fourthNumber);

            avgMultiplicationTimeSIMD += multiplicationSIMD(&vectorArray1[j], &vectorArray2[j]);
        }
        for(j = 0; j < numberAmount; j++)
        {
            avgDivisionTimeSISD += divisionSISD(&vectorArray1[j].firstNumber, &vectorArray2[j].firstNumber);
            avgDivisionTimeSISD += divisionSISD(&vectorArray1[j].secondNumber, &vectorArray2[j].secondNumber);
            avgDivisionTimeSISD += divisionSISD(&vectorArray1[j].thirdNumber, &vectorArray2[j].thirdNumber);
            avgDivisionTimeSISD += divisionSISD(&vectorArray1[j].fourthNumber, &vectorArray2[j].fourthNumber);

            avgDivisionTimeSIMD += divisionSIMD(&vectorArray1[j], &vectorArray2[j]);
        }
    }
    // zapis do pliku, czas zostaje tutaj podzielony przez ilosc powtorzen obiegu petli, zeby otrzymac czas sredni
    fprintf(file, "Typ obliczen: SISD\nLiczba liczb: %d\nSredni czas [s]:\n", numberAmount);
    fprintf(file, "+ %lf\n", avgAdditionTimeSISD / reps);
    fprintf(file, "- %lf\n", avgSubtractionTimeSISD / reps);
    fprintf(file, "* %lf\n", avgMultiplicationTimeSISD / reps);
    fprintf(file, "/ %lf\n\n", avgDivisionTimeSISD/reps);

    fprintf(file, "Typ obliczen: SIMD\nLiczba liczb: %d\nSredni czas [s]:\n", numberAmount);
    fprintf(file, "+ %lf\n", avgAdditionTimeSIMD / reps);
    fprintf(file, "- %lf\n", avgSubtractionTimeSIMD / reps);
    fprintf(file, "* %lf\n", avgMultiplicationTimeSIMD / reps);
    fprintf(file, "/ %lf\n\n", avgDivisionTimeSIMD / reps);

    // zamkniecie pliku
    fclose(file);

    return 0;
}