#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <windows.h>
#include <limits> 
#include <set>
#include <utility> 
#include <algorithm>
#include <iterator>
#include <chrono> 
#include <random>


using namespace std;

typedef int vrh_t;
typedef double tezina_t;

const tezina_t maksimalna_tezina = std::numeric_limits<double>::infinity();

struct susjed {
    vrh_t cilj;
    tezina_t tezina;
    susjed(vrh_t arg_cilj, tezina_t arg_tezina)
        : cilj(arg_cilj), tezina(arg_tezina) {}
};

typedef std::vector<std::vector<susjed>> lista_susjedstva_t;

class NajkraciPut
{
private:
    bool zidovi = true;
    unsigned static const int redovi = 20;
    unsigned static const int stupci = 40;
    int mapa[redovi][stupci];
    int redak_a;
    int stupac_a;
    int redak_b;
    int stupac_b;
    std::list<vrh_t> put;
    int dobij_slucajni_broj(double sansa_od_1);
    void DijkstraIzracunajNajkracePutanje(vrh_t izvor, const lista_susjedstva_t& lista_susjedstva, std::vector<tezina_t>& min_udaljenost, std::vector<vrh_t>& prethodni);
    std::list<vrh_t> DijkstraDobijNajkraciPutDo(vrh_t vrh, const std::vector<vrh_t>& prethodni);
public:
    NajkraciPut(int redak_a, int stupac_a, int redak_b, int stupac_b, bool zidovi);
    void iscrtajMapu(const std::list<vrh_t>& prilagodjeni_put, int mapa[20][40], int redak_a, int stupac_a, int redak_b, int stupac_b);
};

