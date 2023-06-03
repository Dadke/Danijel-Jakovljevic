#include "NajkraciPut.h"

int NajkraciPut::dobij_slucajni_broj(double sansa_od_1)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    double slucajna_vrednost = dis(gen);

    if (slucajna_vrednost < sansa_od_1) {
        return 1;
    }
    else {
        return 0;
    }
}

NajkraciPut::NajkraciPut(int redak_a, int stupac_a, int redak_b, int stupac_b, bool zidovi)
{
    this->redak_a = redak_a;
    this->stupac_a = stupac_a;
    this->redak_b = redak_b;
    this->stupac_b = stupac_b;
    this->zidovi = zidovi;

    // Inicijalizacija mape sa zidovima (oznaèeni sa 1)
    for (int i = 0; i < redovi; ++i) {
        for (int j = 0; j < stupci; ++j) {
            if ((i == redak_a - 1 && j == stupac_a - 1) || (i == redak_b - 1 && j == stupac_b - 1)) {
                mapa[i][j] = 0; // Prazna æelija za pozicije A i B
            }
            else if (zidovi)
            {
                mapa[i][j] = dobij_slucajni_broj(0.10); // 10% šansa da æelija bude zid
            }
            else
            {
                mapa[i][j] = 0;
            }
        }
    }

    // Inicijalizacija liste susedstva
    lista_susjedstva_t lista_susjedstva(redovi * stupci + 1);  // Dodajemo dodatni vrh za zidove

    // Dodavanje grana izmeðu æelija
    for (int i = 0; i < redovi; ++i) {
        for (int j = 0; j < stupci; ++j) {
            // Preskaèi zidove
            if (mapa[i][j] == 1) {
                continue;
            }

            // Povezivanje sa gornjom æelijom
            if (i > 0 && mapa[i - 1][j] != 1) {
                lista_susjedstva[i * stupci + j].push_back(susjed((i - 1) * stupci + j, 1));  // Povezivanje sa gornjom æelijom
            }

            // Povezivanje sa levom æelijom
            if (j > 0 && mapa[i][j - 1] != 1) {
                lista_susjedstva[i * stupci + j].push_back(susjed(i * stupci + j - 1, 1));  // Povezivanje sa levom æelijom
            }

            // Povezivanje sa desnom æelijom
            if (j < stupci - 1 && mapa[i][j + 1] != 1) {
                lista_susjedstva[i * stupci + j].push_back(susjed(i * stupci + j + 1, 1));  // Povezivanje sa desnom æelijom
            }

            // Povezivanje sa donjom æelijom
            if (i < redovi - 1 && mapa[i + 1][j] != 1) {
                lista_susjedstva[i * stupci + j].push_back(susjed((i + 1) * stupci + j, 1));  // Povezivanje sa donjom æelijom
            }
        }
    }

    // Povezivanje zidova sa ostalim vrhovima sa beskonaènom težinom
    if (zidovi)
    {
        for (int i = 0; i < redovi; ++i) {
            for (int j = 0; j < stupci; ++j) {
                if (mapa[i][j] == 1) {
                    lista_susjedstva[redovi * stupci].push_back(susjed(i * stupci + j, maksimalna_tezina));  // Povezivanje zidova sa ostalim vrhovima sa beskonaènom težinom
                }
            }
        }
    }

    std::vector<tezina_t> min_udaljenost;
    std::vector<vrh_t> prethodni;
    DijkstraIzracunajNajkracePutanje((redak_a - 1) * stupci + stupac_a - 1, lista_susjedstva, min_udaljenost, prethodni);
    std::list<vrh_t> put = DijkstraDobijNajkraciPutDo((redak_b - 1) * stupci + stupac_b - 1, prethodni);

    std::list<vrh_t> prilagodjeni_put = put;

    // Iscrtavanje mape sa poèetnim pozicijama A i B
    iscrtajMapu(prilagodjeni_put, mapa, redak_a, stupac_a, redak_b, stupac_b);
    Sleep(100);

    // Iscrtavanje mape sa putanjom
    for (const vrh_t& vrh : prilagodjeni_put)
    {
        int red = vrh / stupci;
        int stupac = vrh % stupci;
        mapa[red][stupac] = 2; // Oznaèi putanju kao posetu
        iscrtajMapu(prilagodjeni_put, mapa, redak_a, stupac_a, redak_b, stupac_b);
        Sleep(100);
    }
}

void NajkraciPut::DijkstraIzracunajNajkracePutanje(vrh_t izvor, const lista_susjedstva_t& lista_susjedstva, std::vector<tezina_t>& min_udaljenost, std::vector<vrh_t>& prethodni)
{
    int n = lista_susjedstva.size();
    min_udaljenost.clear();
    min_udaljenost.resize(n, maksimalna_tezina);
    min_udaljenost[izvor] = 0;
    prethodni.clear();
    prethodni.resize(n, -1);
    std::set<std::pair<tezina_t, vrh_t>> red_vrhova;
    red_vrhova.insert(std::make_pair(min_udaljenost[izvor], izvor));

    while (!red_vrhova.empty())
    {
        tezina_t udaljenost = red_vrhova.begin()->first;
        vrh_t u = red_vrhova.begin()->second;
        red_vrhova.erase(red_vrhova.begin());

        // Poseti svaku granu koja izlazi iz vrha u
        const std::vector<susjed>& susjedi = lista_susjedstva[u];
        for (const susjed& susjed_iter : susjedi)
        {
            vrh_t v = susjed_iter.cilj;
            tezina_t tezina = susjed_iter.tezina;

            // Preskoèi zidove (èvorove sa težinom = maksimalna_tezina)
            if (tezina == maksimalna_tezina)
                continue;

            tezina_t udaljenost_preko_u = udaljenost + tezina;
            if (udaljenost_preko_u < min_udaljenost[v])
            {
                red_vrhova.erase(std::make_pair(min_udaljenost[v], v));

                min_udaljenost[v] = udaljenost_preko_u;
                prethodni[v] = u;
                red_vrhova.insert(std::make_pair(min_udaljenost[v], v));
            }
        }
    }
}

std::list<vrh_t> NajkraciPut::DijkstraDobijNajkraciPutDo(vrh_t vrh, const std::vector<vrh_t>& prethodni)
{
    std::list<vrh_t> put;
    for (; vrh != -1; vrh = prethodni[vrh])
        put.push_front(vrh);
    return put;
}

void NajkraciPut::iscrtajMapu(const std::list<vrh_t>& prilagodjeni_put, int mapa[20][40], int redak_a, int stupac_a, int redak_b, int stupac_b)
{
    system("cls");
    // Iscrtaj mapu sa putanjom
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 40; ++j)
        {
            if (i == redak_a - 1 && j == stupac_a - 1)
                cout << "A ";
            else if (i == redak_b - 1 && j == stupac_b - 1)
                cout << "B ";
            else if (mapa[i][j] == 1)
                cout << "[] ";
            else if (mapa[i][j] == 2)
                cout << "* ";
            else
                cout << ". ";
        }
        cout << endl;
    }
}
