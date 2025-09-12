#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    int x;
    int y;
} Punkt;

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
        realloc(vec->punkty, sizeof(Punkt) * vec->pojemnosc);
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
        realloc(vecvec->listy, sizeof(VecPunkt) * vecvec->pojemnosc);
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
VecPunkt_lub_Punkt vplp_vec(Punkt p)
{
    VecPunkt_lub_Punkt vplp =
    {
        .typ = LISTA,
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
        .dlugosc = 0,
        .pojemnosc = 1
    };
    return str;
}
String String_new(char* znaki_startowe)
{
    String str =
    {
        .znaki = znaki_startowe,
        .dlugosc = strlen(znaki_startowe) - 1, //ingoruję \0, ta implementacja nie używa \0
        .pojemnosc = strlen(znaki_startowe)
    };
    return str;
}
void String_push_char(char znak, String* str)
{
    if(str->pojemnosc == str->dlugosc)
    {
        str->pojemnosc *= 2;
        realloc(str->znaki, sizeof(char) * str->pojemnosc);
    }
    str->znaki[str->dlugosc] = znak;
    str->dlugosc++;
}
void String_push_chars(char* znaki, String* str)
{
    size_t i = 0;
    while(znaki[i] != '\0')
        String_push_char(znaki[i], str);
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

VecVecPunkt wczytaj_figury(char* plik)
{
    
}

int main()
{

    return EXIT_SUCCESS;
}