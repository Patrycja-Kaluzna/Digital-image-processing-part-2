#include "naglowek.h"

//Źródło: https://kcir.pwr.edu.pl/~mucha/PProg/

/*******************************************************/
/* Funkcja inicjuje strukture wskazywana przez wybor   */
/* PRE:                                                */
/*      poprawnie zainicjowany argument wybor (!=NULL) */
/* POST:                                               */
/*      "wyzerowana" struktura wybor wybranych opcji   */
/*******************************************************/

void wyzeruj_opcje (w_opcje *wybor) {
    if (wybor != NULL) {
    wybor->plik_we = NULL;
    wybor->plik_wy = NULL;
    wybor->negatyw = 0;
    wybor->konturowanie = 0;
    wybor->progowanie = 0;
    wybor->progowanie = 0;
    wybor->rozmycie = 0;
    wybor->w_progu = 0;
    } else {
    fprintf(stderr, "Blad: Niepoprawnie zainicjowany argument wybor\n");}
}

/************************************************************************/
/* Funkcja rozpoznaje opcje wywolania programu zapisane w tablicy argv  */
/* i zapisuje je w strukturze wybor                                     */
/* Skladnia opcji wywolania programu                                    */
/*         program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-k] [-r]    */
/*                    [-d] }                                            */
/* Argumenty funkcji:                                                   */
/*         argc  -  liczba argumentow wywolania wraz z nazwa programu   */
/*         argv  -  tablica argumentow wywolania                        */
/*         wybor -  struktura z informacjami o wywolanych opcjach       */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      funkcja otwiera odpowiednie pliki, zwraca uchwyty do nich       */
/*      w strukturze wyb�r, do tego ustawia na 1 pola, ktore            */
/*	    poprawnie wystapily w linii wywolania programu,                 */
/*  	pola opcji nie wystepujacych w wywolaniu ustawione sa na 0;     */
/*	    zwraca wartosc W_OK, gdy wywolanie bylo poprawne                */
/*	    lub kod bledu zdefiniowany stalymi B_* (<0)                     */
/* UWAGA:                                                               */
/*      funkcja nie sprawdza istnienia i praw dostepu do plikow         */
/*      w takich przypadkach zwracane uchwyty maja wartosc NULL         */
/************************************************************************/

int przetwarzaj_opcje (int argc, char **argv, w_opcje *wybor) {
    int i, prog;
    char *nazwa_pliku_we, *nazwa_pliku_wy;
    wybor->plik_wy = stdout; //Na wypadek gdy nie podano opcji "-o"

    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-') //Blad: to nie jest opcja - brak znaku "-"
            return B_NIEPOPRAWNAOPCJA;
        switch (argv[i][1]) {
        case 'i':
        { //Opcja z nazwa pliku wejsciowego
            if (++i < argc)
            { //Wczytujemy kolejny argument jako nazwe pliku
                nazwa_pliku_we = argv[i];
                if (strcmp(nazwa_pliku_we, "-") == 0) //Gdy nazwa jest "-"
                    wybor->plik_we = stdin;           //Ustwiamy wejscie na stdin
                else                                  //Otwieramy wskazany plik
                    wybor->plik_we = fopen(nazwa_pliku_we, "r");
            }
            else
                return B_BRAKNAZWY; /* blad: brak nazwy pliku */
            break;
        }
        case 'o':
        { //Opcja z nazwa pliku wyjsciowego
            if (++i < argc)
            { //Wczytujemy kolejny argument jako nazwe pliku
                nazwa_pliku_wy = argv[i];
                if (strcmp(nazwa_pliku_wy, "-") == 0) //Gdy nazwa jest "-"
                    wybor->plik_wy = stdout;          //Ustwiamy wyjscie na stdout
                else                                  //Otwieramy wskazany plik
                    wybor->plik_wy = fopen(nazwa_pliku_wy, "w");
            }
            else
                return B_BRAKNAZWY; //Blad: brak nazwy pliku
            break;
        }
        case 'p':
        {
            if (++i < argc)
            { //Wczytujemy kolejny argument jako wartosc progu
                if (sscanf(argv[i], "%d", &prog) == 1)
                {
                    wybor->progowanie = 1;
                    wybor->w_progu = prog;
                }
                else
                    return B_BRAKWARTOSCI; //Blad: niepoprawna wartosc progu
            }
            else
                return B_BRAKWARTOSCI; //Blad: brak wartosci progu
            break;
        }
        case 'n':
        { //Mamy wykonac negatyw
            wybor->negatyw = 1;
            break;
        }
        case 'k':
        { //Mamy wykonac konturowanie
            wybor->konturowanie = 1;
            break;
        }
        case 'r':
        { //Mamy wykonac rozmycie
            wybor->rozmycie = 1;
            break;
        }
        case 'd':
        { //Mamy wyswietlic obraz
            wybor->wyswietlenie = 1;
            break;
        }
        default: //Nierozpoznana opcja
            return B_NIEPOPRAWNAOPCJA;
        } //Koniec switch
    }     //Koniec for

    if (wybor->plik_we != NULL) //Ok: wej. strumien danych zainicjowany
        return W_OK;
    else
        return B_BRAKPLIKU; //Blad:  nie otwarto pliku wejsciowego
}