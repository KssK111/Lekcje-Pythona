#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

typedef struct
{
    int x;
    int y;
} Punkt;
Punkt Punkt_new(int x, int y)
{
    Punkt p =
    {
        .x = x,
        .y = y
    };
    return p;
}

typedef struct
{
    Punkt* punkty;
    size_t dlugosc;
    size_t pojemnosc;
} VecPunkt;
VecPunkt VecPunkt_new()
{
    VecPunkt vp =
    {
        .punkty = malloc(sizeof(Punkt)),
        .dlugosc = 0,
        .pojemnosc = 1,
    };
    return vp;
}
void VecPunkt_push(Punkt p, VecPunkt* vec)
{
    if(vec->pojemnosc == vec->dlugosc)
    {
        vec->pojemnosc *= 2;
        vec->punkty = realloc(vec->punkty, sizeof(Punkt) * vec->pojemnosc);
    }
    vec->punkty[vec->dlugosc] = p;
    vec->dlugosc++;
}

typedef struct
{
    VecPunkt* listy;
    size_t dlugosc;
    size_t pojemnosc;
} VecVecPunkt;
VecVecPunkt VecVecPunkt_new()
{
    VecVecPunkt vvp =
    {
        .listy = malloc(sizeof(VecPunkt)),
        .dlugosc = 0,
        .pojemnosc = 1,
    };
    return vvp;
}
void VecVecPunkt_push(VecPunkt vec, VecVecPunkt* vecvec)
{
    if(vecvec->pojemnosc == vecvec->dlugosc)
    {
        vecvec->pojemnosc *= 2;
        vecvec->listy = realloc(vecvec->listy, sizeof(VecPunkt) * vecvec->pojemnosc);
    }
    vecvec->listy[vecvec->dlugosc] = vec;
    vecvec->dlugosc++;
}

typedef struct
{
    enum
    {
        LISTA,
        TUPEL,
    } typ;
    union
    {
        VecPunkt lista;
        Punkt tupel;
    };
} VecPunkt_lub_Punkt;
VecPunkt_lub_Punkt vplp_vec(VecPunkt vp)
{
    VecPunkt_lub_Punkt vplp =
    {
        .typ = LISTA,
        .lista = vp
    };
    return vplp;
}
VecPunkt_lub_Punkt vplp_punkt(Punkt p)
{
    VecPunkt_lub_Punkt vplp =
    {
        .typ = TUPEL,
        .tupel = p
    };
    return vplp;
}

typedef struct
{
    char* znaki;
    size_t dlugosc;
    size_t pojemnosc;
} String;
String String_new_empty()
{
    String str =
    {
        .znaki = malloc(sizeof(char)),
        .dlugosc = 1,
        .pojemnosc = 1
    };
    str.znaki[0] = '\0';
    return str;
}
String String_new(char* znaki_startowe)
{
    size_t dlugosc = strlen(znaki_startowe) + 1;
    String str =
    {
        .dlugosc = dlugosc,
        .pojemnosc = dlugosc
    };
    str.znaki = malloc(sizeof(char) * dlugosc);
    memcpy(str.znaki, znaki_startowe, dlugosc);
    return str;
}
void String_push_char(char znak, String* str)
{
    if(str->pojemnosc == str->dlugosc)
    {
        str->pojemnosc *= 2;
        str->znaki = realloc(str->znaki, sizeof(char) * str->pojemnosc);
    }

    str->znaki[str->dlugosc - 1] = znak;
    str->znaki[str->dlugosc] = '\0';
    str->dlugosc++;
}
void String_push_chars(char* znaki, String* str)
{
    size_t dodatkowa_dlugosc = strlen(znaki) + 1;
    size_t nowa_dlugosc = str->dlugosc - 1 + dodatkowa_dlugosc;
    if(str->pojemnosc < nowa_dlugosc)
    {
        str->znaki = realloc(str->znaki, sizeof(char) * (nowa_dlugosc));
        str->pojemnosc = nowa_dlugosc;
    }
    memcpy(&(str->znaki[str->dlugosc - 1]), znaki, dodatkowa_dlugosc);
    str->dlugosc = nowa_dlugosc;
}
void String_clear(String* str)
{
    str->dlugosc = 1;
    str->znaki[0] = '\0';
}
void String_trim(String* str)
{
    size_t przesuniecie_start = 0;
    size_t przesuniecie_end = 0;
    while(isspace((unsigned char)str->znaki[przesuniecie_start])) przesuniecie_start++;
    while(isspace((unsigned char)str->znaki[str->dlugosc - 2 - przesuniecie_end])) przesuniecie_end++;
    str->dlugosc = str->dlugosc - (przesuniecie_start + przesuniecie_end) - 1/*null byte uwzgędniony później*/;
    str->znaki = memmove(str->znaki, &(str->znaki[przesuniecie_start]), str->dlugosc);
    str->znaki[str->dlugosc] = '\0';
    str->dlugosc++;
}

bool czy_nalezy(Punkt C, VecPunkt_lub_Punkt Figura)
{
    switch(Figura.typ)
    {
        case TUPEL:
            return (C.x == Figura.tupel.x) && (C.y == Figura.tupel.y);
        case LISTA:
            VecPunkt figura = Figura.lista;
            if(figura.dlugosc == 1)
                return (C.x == figura.punkty[0].x) && (C.y == figura.punkty[0].x);
            for(size_t i = 0; i < figura.dlugosc - 1; i++)
            {
                Punkt A = figura.punkty[i];
                Punkt B = figura.punkty[i + 1];
                if((B.x - A.x) * (C.y - A.y) == (B.y - A.y) * (C.x - A.x))
                    return true;
            }
            return false;
        default:
            exit(EXIT_FAILURE);
    }
}

bool next_line(FILE* plik, String* buffer)
{
    int znak = fgetc(plik);
    if(znak == EOF) return false;
    if(znak == '\n') return true;
    do
    {
        String_push_char(znak, buffer);
        znak = fgetc(plik);
    } while((znak != '\n') && (znak != EOF));
    return true;
}
VecVecPunkt wczytaj_figury(char* nazwa_pliku)
{
    FILE* plik = fopen(nazwa_pliku, "r");
    VecVecPunkt wynik = VecVecPunkt_new();
    VecPunkt wynik_temp = VecPunkt_new();
    String linia = String_new_empty();
    while(next_line(plik, &linia))
    {
        String_trim(&linia);
        char* spacja;
        if(spacja = strstr(linia.znaki, " "))
        {
            //Chyba można też sscanf, aleee
            *spacja = '\0';
            int x = atoi(linia.znaki);
            int y = atoi(spacja + 1);
            VecPunkt_push(Punkt_new(x, y), &wynik_temp);
        }
        else if(wynik_temp.dlugosc)
        {
            VecVecPunkt_push(wynik_temp, &wynik);
            wynik_temp = VecPunkt_new();
        }
        String_clear(&linia);
    }
    if(wynik_temp.dlugosc) VecVecPunkt_push(wynik_temp, &wynik);
    fclose(plik);
    return wynik;
}

int main()
{
    VecVecPunkt figury = wczytaj_figury("dane.txt");
    for(size_t i = 0; i < figury.dlugosc; i++)
    {
        for(size_t j = 0; j < figury.listy[i].dlugosc; j++)
        {
            Punkt p = figury.listy[i].punkty[j];
            printf("A = %d, B = %d\n", p.x, p.y);
        }
        printf("\n");
    }
    for(size_t i = 0; i < figury.dlugosc; i++)
    {
        bool sprawdzmy = czy_nalezy(Punkt_new(3, 2), vplp_vec(figury.listy[i]));
        printf("%s\n", sprawdzmy ? "True" : "False");
        free(figury.listy[i].punkty);
    }
    free(figury.listy);
    assert(czy_nalezy(Punkt_new(5, 5), vplp_punkt(Punkt_new(5, 5))));
    VecPunkt vp = VecPunkt_new();
    VecPunkt_push(Punkt_new(5, 5), &vp);
    assert(czy_nalezy(Punkt_new(5, 5), vplp_vec(vp)));
    free(vp.punkty);
    return EXIT_SUCCESS;
}