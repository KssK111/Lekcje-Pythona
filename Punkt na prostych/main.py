def czy_nalezy(C: (int, int), Figura: [(int, int)]) -> bool:
    pass

def wczytaj_punkty(plik: str) -> [[(int, int)]]:
    wynik = []
    with open(plik) as file:

        for line in file:
            print(line.strip())

wczytaj_punkty("dane.txt")