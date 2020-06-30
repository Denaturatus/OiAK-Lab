# stale dla funkcji systemowych
SYSCALL = 0x80
SYSEXIT = 1
EXIT_SUCCESS = 0

# liczby na ktorych przeprowadzona bedzie operacja dodawania
.data
num1:
        .long 0x10304008, 0x701100FF, 0x45100020, 0x08570030
num1_len = ( . - num1)/4 # obliczenie z ilu 32-bitowych "liczb" sklada sie liczba pierwsza
num2:
        .long 0xF040500C, 0x00220026, 0x321000CB, 0x04520031
num2_len = ( . - num2)/4 # obliczenie z ilu 32-bitowych "liczb" sklada sie liczba druga

.global _start
_start:

mov $num1_len, %esi # wpisanie do rejestru esi dlugosci liczby pierwszej
mov $num2_len, %edi # wpisanie do rejestru edi dlugosci liczby drugiej
dec %esi # dekrementacja wartosci rejestru zeby mogl sluzyc za licznik pozostalych iteracji po danej liczbie (ilosc iteracji liczymy do zera wlacznie)
dec %edi # dekrementacja wartosci rejestru zeby mogl sluzyc za licznik pozostalych iteracji po danej liczbie (ilosc iteracji liczymy do zera wlacznie)

clc # ustawienie flagi przeniesienia na zero
pushf # umieszczenie flag na stos

subLoop: # glowna petla programu odpowiedzialna za odejmowania czesciowe, sprawdzania czy iteracja po ktorejs liczbie zostala zakonczona i przeplyw sterowania miedzy subrutynami
clc
popf # pobranie flag ze stosu

movl num1(,%esi, 4), %eax # umieszczenie odpowiedniej czesci liczby pierwszej w rejestrze za pomoca trybu adresowania indekoswego
movl num2(,%edi, 4), %ebx # umieszczenie odpowiedniej czesci liczby drugiej w rejestrze za pomoca trybu adresowania indekoswego
sbb %eax, %ebx # odjecie od rejestru ebx rejestru eax, wynik jest umieszczony w ebx
push %rbx # umieszczenie czesci wyniku na stosie
pushf # umieszczenie flag na stosie

clc

cmp $0, %esi # sprawdzenie czy liczba pierwsza sie skonczyla za pomoca porownania wartosci rejestru esi do zera
jz isSecondDone # jezeli liczba pierwsza sie skonczyla to nastepuje skok do subrutyny ktora sprawdza czy druga rowniez jest skonczona

cmp $0, %edi # sprawdzenie czy liczba druga sie skonczyla za pomoca porownania wartosci rejestru edi do zera
jz isFirstDone # jezeli druga liczba sie skonczyla to nastepuje skok do subrutyny ktora sprawdza czy pierwsza rowniez jest skonczona

# jezeli nie doszlo do zadnego skoku to znaczy ze na obu liczbach dalej nalezy normalnie przeprowadzac dodawanie
dec %esi # zmniejszenie ilosci iteracji po liczbie pierwszej
dec %edi # zmniejszenie ilosci iteracji po liczbie drugiej
jmp subLoop # powrot do glownej petli programu

isFirstDone: # subrutyna odpowiedzialna za sprawdzenia czy liczba pierwsza sie skonczyla jezeli druga jest skonczona
cmp $0, %esi
jz lastCarry # jezeli obie sa skonczone to nastepuje skok do subrutyny odpowiedzialnej za ostatnie pozyczke
jmp secondDone # jezeli powyzszy skok sie nie wykonal to przechodzimy do subrutyny odejmujacej liczbe pierwsza od zera, poniewaz druga jest juz skonczona

isSecondDone: # subrutyna odpowiedzialna za sprawdzenia czy druga liczba sie skonczyla jezeli pierwsza jest skonczona
cmp $0, %edi
jz lastCarry # jezeli obie sa skonczone to nastepuje skok do subrutyny odpowiedzialnej za ostatnie pozyczke
jmp firstDone # jezeli powyzszy skok sie nie wykonal to przechodzimy do subrutyny odejmujacej zera od liczby drugiej, poniewaz pierwsza jest juz skonczona

firstDone: # subrutyna odpowiedzialna za odejmowanie zera od liczby drugiej z uwzglednieniem pozyczki, poniewaz liczba pierwsza sie skonczyla
clc
popf
dec %edi # dekrementacja ilosci iteracji w tym przypadku nastepuje przed operacja odejmowania poniewaz po dokonaniu skoku do isSecondDone nie doszlo do dekrementacji
movl $0, %eax # umieszczonie odpowiednich wartosci w rejestrach analogicznie jak w glownej petli programu
movl num2(,%edi, 4), %ebx
sbb %eax, %ebx
push %rbx
pushf
jmp isSecondDone # ponowne sprawdzenie czy iteracja po drugiej liczbie zostala zakonczona

secondDone: # subrutyna odpowiedzialna za odejmowanie zera od liczby drugiej z uwzglednieniem pozyczki, poniewaz liczba pierwsza sie skonczyla
clc
popf
dec %esi # dekrementacja ilosci iteracji w tym przypadku nastepuje przed operacja odejmowania poniewaz po dokonaniu skoku do isSecondDone nie doszlo do dekrementacji
movl num1(,%esi, 4), %eax # umieszczonie odpowiednich wartosci w rejestrach analogicznie jak w glownej petli programu
movl $0, %ebx
sbb %eax, %ebx
push %rbx
pushf
jmp isFirstDone # ponowne sprawdzenie czy iteracja po pierwszej liczbie zostala zakonczona

lastCarry: # subrutyna odpowiedzialna za wykonanie operacji ostatniej pozyczki
clc
popf
mov $0, %eax # w rejestrach zostaja umieszczone zera poniewaz obie liczby zostaly juz wykorzystane
mov $0, %ebx
sbb %eax, %ebx # jezeli pozyczka wynosi jeden to rejestr bedzie wypelniony f, jak nie to zero
cmp $0, %ebx # sprawdzamy wartosc w rejestrze
jz exit # jezeli wartosc rejestru wynosi zero to nastepuje przeskok do konca programu, poniewaz umieszczajac zawartosc pustego rejestru rbx na stosie, stos jest czyszczony
push %rbx # jezeli powyzszy skok sie nie wykonal to wartosc rejestru zostaje umieszczona na stosie
jmp exit # skok do konca programu

exit:
mov $SYSEXIT, %eax
mov $EXIT_SUCCESS, %ebx
int $SYSCALL
