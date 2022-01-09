#include "naglowek.h"

int main (int argc, char *argv[]) {
    int i;

    t_obraz *obraz =  malloc(sizeof(t_obraz));
    w_opcje *wybor = malloc(sizeof(w_opcje));

    int wynik = przetwarzaj_opcje(argc,argv,wybor);

    if (wynik != W_OK) {
        printf("Błędne argumenty wywołania programu\n" );
        return 2;}

    if (wybor->plik_we != NULL) {
        wczytaj(wybor->plik_we, obraz);}

    if (wybor->negatyw == 1) {
        negatyw(obraz->piksele, obraz->wymx, obraz->wymy, obraz->szarosci);}

    if (wybor->konturowanie == 1) {
        konturowanie(obraz->piksele, obraz->wymx, obraz->wymy, obraz->szarosci);}

    if (wybor->progowanie == 1) {
        progowanie(obraz->piksele, obraz->wymx, obraz->wymy, obraz->szarosci, wybor->w_progu);}

    if (wybor->rozmycie == 1) {
        rozmycie(obraz->piksele, obraz->wymx, obraz->wymy, obraz->szarosci);}

    if (wybor->plik_wy != NULL) {
        zapisz(wybor->plik_wy, obraz);}

    if (wybor->wyswietlenie == 1) {
        for (i = 1; i < argc ; i++) {
            if ((*(argv + i))[1] == 'o') {
                if (**(argv + i + 1) != '-') {
                    wyswietl(*(argv + i + 1));
                }
            }   
        }
    }
    wyzeruj_opcje(wybor);
    free(obraz->piksele);
    free(obraz);
    free(wybor);
    return 0;
}