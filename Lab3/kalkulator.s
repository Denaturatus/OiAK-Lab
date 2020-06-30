# stale dla funkcji systemowych
SYSCALL = 0x80
SYSEXIT = 1
EXIT_SUCCESS = 0

# liczby na ktorych przeprowadzone sa operacje
.data
firstFloat: .float 13.45
secondFloat: .float 3.27
firstDouble: .double -1.5432
secondDobule: .double 0.0

FPUControlWord: .short 0x037f 
/* 
slowo kontrolne jednostki zmiennoprzecinkowej o 16 bitach
bity 8 i 9 kontroluja precyzje, a 10 i 11 tryb zaokraglania
dla precyzji: 00 - pojedyncza precyzja, 10 - podwojna precyzja
11 - rozszerzona podwojna precyzja
dla zaokraglania: 00 - do najblizszej parzystej, 01 - w dol do - nieskonczonosci
10 - do gory do + nieskonczonosci, 11 - w kierunku zera
*/

.global _start
_start:

finit # inicjalizacja FPU, ustawienie rejestrow na domyslne wartosci 
fldcw FPUControlWord # zaladuj slowo sterujace FPU z pamieci

jmp doubleDiv # przeskok do konkretnej operacji 

floatAdd: # dodawanie liczb zmiennoprzecinkowych o pojedynczej precyzji
flds firstFloat # instrukcja fld laduje liczbe do st(0), sufiks s oznacza pojedyncza precyzje
fadds secondFloat # dodawanie liczby do st(0), wynik w st(0), sufiks s oznacza pojedyncza precyzje
jmp exit # skok do konca programu

floatSub: # odejmowanie liczb zmiennoprzecinkowych o pojedynczej precyzji
flds firstFloat
fsubs secondFloat # odejmowanie liczby od st(0), wynik w st(0), sufiks s oznacza pojedyncza precyzje
jmp exit

floatMul: # mnozenie liczb zmiennoprzecinkowych o pojedynczej precyzji
flds firstFloat
fmuls secondFloat # mnozenie zawartosci st(0) przez liczbe, wynik w st(0), sufiks s oznacza pojedyncza precyzje
jmp exit

floatDiv: # dzielenie liczb zmiennoprzecinkowych o pojedynczej precyzji
flds firstFloat
fdivs secondFloat # dzielenie zawartosci st(0) przez liczbe, wynik w st(0), sufiks s oznacza pojedyncza precyzje
jmp exit

doubleAdd: # dodawanie liczb zmiennoprzecinkowych o podwojnej precyzji
fldl firstDouble # instrukcja fld laduje liczbe do st(0), sufiks l oznacza podwojna precyzje
faddl secondDobule # dodawanie liczby do st(0), wynik w st(0), sufiks l oznacza podwojna precyzje
jmp exit

doubleSub:  # odejmowanie liczb zmiennoprzecinkowych o podwojnej precyzji
fldl firstDouble
fsubl secondDobule # odejmowanie liczby od st(0), wynik w st(0), sufiks l oznacza podwojna precyzje
jmp exit

doubleMul: # mnozenie liczb zmiennoprzecinkowych o podwojnej precyzji
fldl firstDouble
fmull secondDobule # mnozenie zawartosci st(0) przez liczbe, wynik w st(0), sufiks l oznacza podwojna precyzje
jmp exit

doubleDiv: # dzielenie liczb zmiennoprzecinkowych o podwojnej precyzji
fldl firstDouble
fdivl secondDobule # dzielenie zawartosci st(0) przez liczbe, wynik w st(0), sufiks l oznacza podwojna precyzje
jmp exit

exit:
mov $SYSEXIT, %eax
mov $EXIT_SUCCESS, %ebx
int $SYSCALL
