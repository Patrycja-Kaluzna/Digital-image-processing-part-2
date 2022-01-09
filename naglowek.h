#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define W_OK 0                //Wartosc oznaczajaca brak bledow
#define B_NIEPOPRAWNAOPCJA -1 //Kody bledow rozpoznawania opcji
#define B_BRAKNAZWY -2
#define B_BRAKWARTOSCI -3
#define B_BRAKPLIKU -4
#define DL_LINII 1024

//Strukura do zapamietywania opcji podanych w wywolaniu programu
typedef struct {
    FILE *plik_we, *plik_wy;                                        //Uchwyty do pliku wej. i wyj.
    int negatyw, progowanie, konturowanie, wyswietlenie, rozmycie;  //Opcje
    int w_progu;                                                    //Wartosc progu dla opcji progowanie
} w_opcje;

enum TypObrazu {PPM = 1, PGM = 0}; 

//Struktura do przechowywania informacji o obrazie
typedef struct {
  int wymx, wymy, szarosci;
  int *piksele;
  enum TypObrazu typ;
} t_obraz;

void wyzeruj_opcje (w_opcje *);

int przetwarzaj_opcje (int , char **, w_opcje *);

void wczytaj (FILE *, t_obraz *);

void zapisz (FILE *, t_obraz *);

void wyswietl (char *);

void negatyw (int *,int,int,int);

void progowanie (int* ,int,int,int,int);

void konturowanie (int*,int,int,int);

void rozmycie (int* ,int,int,int);