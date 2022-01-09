#include "naglowek.h"

/******************************************************************/
/* Funkcja wczytuje obraz i informacje o nim z pliku wskazywanego */
/* przez plik_we do struktury wskazywanej przez obraz             */
/* PRE:                                                           */
/*     poprawnie zainicjowane argumenty:                          */
/*     plik_we != NULL                                            */
/*     obraz != NULL                                              */
/* POST:                                                          */
/*      obraz i informacje o nim wczytane do struktury            */
/*      wskazywanej przez obraz                                   */
/******************************************************************/

void wczytaj (FILE *plik_we, t_obraz *obraz) {
    char buf[DL_LINII]; // Bufor pomocniczy do czytania naglowka i komentarzy
    int znak;           // Zmienna pomocnicza do czytania komentarzy
    int koniec = 0;     // Czy napotkano koniec danych w pliku
    int i,j;
    int r = 0;
    int g = 0;
    int b = 0;

    // Sprawdzenie czy podano prawidlowy uchwyt pliku
    if (plik_we == NULL) {
        fprintf(stderr, "Blad: Nie podano uchwytu do pliku\n");
        return;}

    //Sprawdzenie czy powiodla sie dynamiczna alokacja pamieci
    if (obraz == NULL) {
    fprintf(stderr, "Blad: Nie powiodla sie dynamiczna alokacja pamieci dla t_obraz *obraz\n");
    return;}

    // Sprawdzenie "numeru magicznego"
    if (fgets(buf, DL_LINII, plik_we) == NULL) // Wczytanie pierwszej linii pliku do bufora
        koniec = 1;                            // Koniec danych
    if ((buf[0] != 'P') || ((buf[1] != '2') && (buf[1] != '3')) || koniec > 0) { // Czy jest numer magiczny 
        fprintf(stderr, "Blad: To nie jest plik PGM lub PPM\n");
        return;}

    // Zapamietanie formatu pliku w strukturze
    if ((buf[1] == '2'))
        obraz->typ = PGM;
    if ((buf[1] == '3'))
        obraz->typ = PPM;

    // Pominiecie komentarzy
    do {
        if ((znak = fgetc(plik_we)) == '#') {          // Czy linia rozpoczyna sie od znaku '#'
            if (fgets(buf, DL_LINII, plik_we) == NULL) // Przeczytaj ja do bufora
                koniec = 1;                            // Zapamietaj ewentualny koniec danych
        } else {
            ungetc(znak, plik_we);}                    // Gdy przeczytany znak z poczatku linii nie jest '#' zwroc go
    } while (znak == '#' && !koniec);                  // Powtarzaj dopoki sa linie komentarza i nie nastapil koniec danych

    // Zapamietanie wymiarow obrazu i jego maksymalnej wartosci szarosci/skladowych kolorowych w strukturze
    fscanf(plik_we, "%d", &(obraz->wymx));
    fscanf(plik_we, "%d", &(obraz->wymy));
    fscanf(plik_we, "%d", &(obraz->szarosci));

    // Dynamiczne alkowanie pamieci przechowujacej obraz
    obraz->piksele = (int *)malloc(sizeof obraz->piksele * obraz->wymx * obraz->wymy);

    // Wczytanie obrazu
    if (obraz->typ == PGM) {
        for (i = 0; i < obraz->wymy; i++)
            for (j = 0; j < obraz->wymx; j++) {
                fscanf(plik_we, "%d", &r);
                *(obraz->piksele + ((i * obraz->wymx) + j)) = r;}
    } else {    
    // Jeśli obraz jest w formacie ppm to w funkcji wczytaj nastepuje jego konwersja do szarości,      
    // a nastepnie zapamietanie jego typu jako pgm
        for (i = 0; i < obraz->wymy; i++)
            for (j = 0; j < obraz->wymx; j++) {
                fscanf(plik_we, "%d", &r);
                fscanf(plik_we, "%d", &g);
                fscanf(plik_we, "%d", &b);
                *(obraz->piksele + ((i * obraz->wymx) + j)) = (r + g + b) / 3;}

        obraz->typ = PGM;}
}

/******************************************************/
/* Funkcja wyswietla przetworzony obraz wykorzystujac */
/* program display                                    */
/* PRE:                                               */
/*     popranwnie zainicjowany argument:              */
/*     n_pliku != NULL                                */
/* POST:                                              */
/*      wyswietlony obraz                             */
/******************************************************/

void wyswietl (char *n_pliku) {
    if (n_pliku != NULL) {
        char polecenie[1024];          // Bufor pomocniczy do zestawienia polecenia
        strcpy(polecenie, "display "); // Konstrukcja polecenia postaci
        strcat(polecenie, n_pliku);    // display "nazwa_pliku" &
        strcat(polecenie, " &");
        printf("%s\n", polecenie); // Wydruk kontrolny polecenia
        system(polecenie);         // Wykonanie polecenia
   } else {
        fprintf(stderr, "Blad: Nie podano nazwy pliku do wyswietlenia (nazwy pliku do zapisu)\n");}
}

/********************************************************************/
/* Funkcja bierze obraz i informacje o nim ze struktury wskazywanej */
/* przez obraz i zapisuje je w pliku wskazywanym przez plik_wy      */
/* PRE:                                                             */
/*     poprawnie zainicjowane argumenty:                            */
/*     plik_wy != NULL                                              */
/*     obraz != NULL                                                */
/* POST:                                                            */
/*      obraz i informacje o nim zapisane w pliku                   */
/*      wskazywanym przez plik_wy                                   */
/********************************************************************/

void zapisz (FILE *plik_wy, t_obraz *obraz) {
    int koniec = 0; // Czy napotkano koniec danych w pliku
    int i, j;

    // Sprawdzenie czy podano prawidlowy uchwyt pliku
    if (plik_wy == NULL) {
        fprintf(stderr, "Blad: Nie podano uchwytu do pliku\n");
        return;}

    //Sprawdzenie czy powiodla sie dynamiczna alokacja pamieci
    if (obraz == NULL) {
    fprintf(stderr, "Blad: Nie powiodla sie dynamiczna alokacja pamieci dla t_obraz *obraz\n");}

    // Wpisanie "numeru magicznego"
    if (obraz->typ == PGM) {
        fputs("P2\n", plik_wy);}

    // Wpisanie wymiaru
    fprintf(plik_wy, "%d %d\n%d\n", obraz->wymx, obraz->wymy, obraz->szarosci);

    // Zapis obrazu
    for (i = 0; i < obraz->wymy; i++) {
        for (j = 0; j < obraz->wymx; j++) {
            if (j != obraz->wymx) {
                fprintf(plik_wy, "%d ", *(obraz->piksele + ((i * obraz->wymx) + j)));
            } else {
                fprintf(plik_wy, "%d\n", *(obraz->piksele + ((i * obraz->wymx) + j)));}
        }
    }
}

/*****************************************************************/
/* Funkcja tworzy negatyw obrazu przechowywanego pod wskaznikiem */
/* piksele i opisywanego przez jej argumenty                     */
/* PRE:                                                          */
/*      poprawnie zainicjowane argumenty:                        */
/*      piksele != NULL                                          */
/*      wymx > 0                                                 */
/*      wymy > 0                                                 */
/*      szarosci > 0                                             */
/* POST:                                                         */
/*      negatyw obrazu przechowywany pod wskaznikiem piksele     */
/*****************************************************************/

void negatyw (int *piksele, int wymx, int wymy, int szarosci) {
    int i, j;

    if (piksele == NULL || wymx < 0 || wymy < 0 || szarosci < 0) {
        fprintf(stderr, "Blad: Niepoprawne argumenty funkcji negatyw\n");
        return;
        } else {
        for (i = 0; i < wymy; i++)
            for (j = 0; j < wymx; j++) {
                *(piksele + i * wymx + j) = szarosci - *(piksele + i * wymx + j);
        }
    }
}

/***************************************************************/
/* Funkcja proguje obraz przechowywany pod wskaznikiem piksele */
/* i opisywany przez jej argumenty                             */
/* PRE:                                                        */
/*      poprawnie zainicjowane argumenty:                      */
/*      piksele != NULL                                        */
/*      wymx > 0                                               */
/*      wymy > 0                                               */
/*      szarosci > 0                                           */
/*      prog > 0 && prog < szarosci                            */
/* POST:                                                       */
/*      sprogowany obraz przechowywany pod wskaznikiem piksele */
/***************************************************************/

void progowanie (int *piksele, int wymx, int wymy, int szarosci, int prog) {
    int i, j;

    if (piksele == NULL || wymx < 0 || wymy < 0 || szarosci < 0 || prog < 0 || prog > szarosci) {
        fprintf(stderr, "Blad: Niepoprawne argumenty funkcji progowanie\n");
        return;
        } else {
        for (i = 0; i < wymy; i++) {
            for (j = 0; j < wymx; j++) {
                if (*(piksele + i * wymx + j) <= prog) {
                    *(piksele + i * wymx + j) = 0;
                    } else {
                    *(piksele + i * wymx + j) = szarosci;
                }
            }
        }
    }
}

/*****************************************************************/
/* Funkcja konturuje obraz przechowywany pod wskaznikiem piksele */
/* i opisywany przez jej argumenty. Jest zrealizowana poprzez    */
/* jego zmniejszenie.                                             */
/* PRE:                                                          */
/*      poprawnie zainicjowane argumenty:                        */
/*      piksele != NULL                                          */
/*      wymx > 0                                                 */
/*      wymy > 0                                                 */
/*      szarosci > 0                                             */
/* POST:                                                         */
/*      skonturowany obraz przechowywany pod wskaznikiem piksele */
/*****************************************************************/

void konturowanie (int *piksele, int wymx, int wymy, int szarosci) {
    int i, j;

    if (piksele == NULL || wymx < 0 || wymy < 0 || szarosci < 0) {
        fprintf(stderr, "Blad: Niepoprawne argumenty funkcji konturowanie\n");
        return;
        } else {
        for (i = 0; i < wymy - 1; i++) {
            for (j = 0; j < wymx - 1; j++) {
                *(piksele + i * wymx + j) = abs(*(piksele + (i+1) * wymx + j) - *(piksele + i * wymx + j)) + abs(*(piksele + i * wymx + j+1) - *(piksele + i * wymx + j));
            }
        }
    }
}

/********************************************************************/
/* Funkcja rozmywa poziomo obraz przechowywany pod wskaznikiem      */
/* piksele i opisywany przez jej argumenty. Jest zrealizowana       */
/* poprzez jego zmniejszenie                                        */
/* PRE:                                                             */
/*      poprawnie zainicjowane argumenty:                           */
/*      piksele != NULL                                             */
/*      wymx > 0                                                    */
/*      wymy > 0                                                    */
/*      szarosci > 0                                                */
/* POST:                                                            */
/*      rozmyty poziomo obraz przechowywany pod wskaznikiem piksele */
/********************************************************************/

void rozmycie (int *piksele, int wymx, int wymy, int szarosci) {
    int i, j;

    if (piksele == NULL || wymx < 0 || wymy < 0 || szarosci < 0) {
        fprintf(stderr, "Blad: Niepoprawne argumenty funkcji rozmycie\n");
        return;
        } else {
        for (i = 0; i < wymy - 1; i++) {
            for (j = 1; j < wymx - 1; j++) {
                 *(piksele + i * wymx + j) = ( *(piksele + i * wymx + j-1) +  *(piksele + i * wymx + j) +  *(piksele + i * wymx + j+1)) / 3;
            }
        }
    }
}