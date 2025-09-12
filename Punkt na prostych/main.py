from itertools import starmap
from os.path import join as join_path

def czy_nalezy(C: tuple[int, int], Figura: list[tuple[int, int]] | tuple[int, int]) -> bool:
    if isinstance(Figura, tuple):
        return C == Figura
    elif len(Figura) == 1:
        return C == Figura[0]
    czy_nalezzzy = lambda A, B: (B[0] - A[0]) * (C[1] - A[1]) == (B[1] - A[1]) * (C[0] - A[0])
    return any(starmap(czy_nalezzzy, zip(Figura[:-1], Figura[1:])))

def wczytaj_figury(plik: str) -> list[list[tuple[int, int]]]:
    wynik = []
    with open(plik) as file:
        wynik_temp = []
        for line in file:
            if " " in (line := line.strip()): wynik_temp.append(tuple(map(int, line.split(" "))))
            elif wynik_temp:
                wynik.append(wynik_temp)
                wynik_temp = []
    if wynik_temp: wynik.append(wynik_temp)
    return wynik

print(figury := wczytaj_figury(join_path("Punkt na prostych", "dane.txt")))
for figura in figury:
    print(czy_nalezy((3, 2), figura))
assert czy_nalezy((5, 5), (5, 5))
assert czy_nalezy((5, 5), [(5, 5)])