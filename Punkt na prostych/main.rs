#![feature(file_buffered)]
use std::{fs::File, io::BufRead, iter::zip, path::PathBuf};

fn main()
{
    let figury = wczytaj_figury("dane.txt");
    println!("{figury:?}");
    for figura in figury
    {
        println!("{}", czy_nalezy(Punkt(3, 2), Figura::Listowa(figura)));
    }
    assert!(czy_nalezy(Punkt(5, 5), Figura::Tuplowa(Punkt(5, 5))));
    assert!(czy_nalezy(Punkt(5, 5), Figura::Listowa(vec![Punkt(5, 5)])));
}

#[derive(Debug, PartialEq, Eq)]
struct Punkt(isize, isize);
enum Figura
{
    Listowa(Vec<Punkt>),
    Tuplowa(Punkt)
}

fn czy_nalezy(c: Punkt, figura: Figura) -> bool
{
    let Punkt(c_x, c_y) = c; //Można zdestrukturyzować już w definicji funkcji, ale wtedy nie zadziała porównywanie niżej do c
    match figura
    {
        Figura::Tuplowa(punkt) => c == punkt,
        Figura::Listowa(lista) =>
        {
            if lista.len() == 1 { return c == lista[0]; }
            let czy_nalezzzy = |(&Punkt(a_x, a_y), &Punkt(b_x, b_y))| (b_x - a_x) * (c_y - a_y) == (b_y - a_y) * (c_x - a_x);
            zip(lista[..lista.len() - 1].iter(), lista[1..].iter()).any(czy_nalezzzy)
        }
    }
}

fn wczytaj_figury(plik: impl Into<PathBuf>) -> Vec<Vec<Punkt>>
{
    let mut wynik = Vec::new();
    let mut wynik_temp = Vec::new();
    for linia in File::open_buffered(plik.into()).unwrap().lines().map(|x| x.unwrap())
    {
        let linia = linia.trim();
        if linia.contains(" ")
        {
            let mut linia = linia.split_whitespace();
            wynik_temp.push(Punkt(linia.next().unwrap().parse().unwrap(), linia.next().unwrap().parse().unwrap()));
        }
        else if !wynik_temp.is_empty()
        {
            wynik.push(wynik_temp);
            wynik_temp = Vec::new();
        }
    }
    if !wynik_temp.is_empty() { wynik.push(wynik_temp) }
    wynik
}