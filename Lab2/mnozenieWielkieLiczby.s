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

innerMultiLoop: # petla odpowiedzialna za wymnozenie calej liczby pierwszej przez poszczegolna czesc liczby drugiej - petla wewnetrzna

movl num1(, %esi, 4), %eax # umieszczenie odpowiedniej czesci liczby pierwszej w rejestrze za pomoca trybu adresowania indekoswego
movl num2(, %edi, 4), %ebx # umieszczenie odpowiedniej czesci liczby drugiej w rejestrze za pomoca trybu adresowania indekoswego
mull %ebx # mnozenie wartosci rejestrow, do %eax zostaje umieszczona mlodsza czesc wyniku, do %edx starsza czesc wyniku
push %rax # umieszczenie mlodszej czesci wyniku na stosie
push %rdx # umieszczenie starszej czesci wyniku na stosie

cmp $0, %esi # sprawdzenie czy przemnozno przez cala liczbe pierwsza
jz outerMultiLoop # jezeli tak to nastepuje skok do petli zewnetrznej mnozenia

dec %esi # jezeli nie nastapil skok do petli zewnetrznej mnozenia nastepuje dekrementacja iteracji po liczbie pierwszej
jmp innerMultiLoop # skok do nastepnego mnozenia w petli wewnetrznej

outerMultiLoop: # petla odpowiedzialna za poruszanie sie po drugiej liczbie
cmp $0, %edi # sprawdzenie czy mnozono cala liczbe pierwsza przez ostatnia czesc liczby drugiej 
jz exit # jezeli tak to nastepuje skok do wyjscia z programu
dec %edi # jezeli powyzszy skok sie nie wykonal to nastepuje dekrementacja iteracji po liczbie drugiej 
mov $num1_len, %esi # ponowne przypisanie ilosci iteracji po liczbie pierwszej, analogicznie jak na poczatku programu
dec %esi
jmp innerMultiLoop # skok do petli wewnetrznej

exit:
mov $SYSEXIT, %eax
mov $EXIT_SUCCESS, %ebx
int $SYSCALL
