#include "NajkraciPut.h"
using namespace std;

int main()
{
  
    bool zidovi = true;
    cout << "Pokrenuti simulaciju s zidovima (1) ili bez zidova (0)? > ";
    cin >> zidovi;

    int redak_a, stupac_a, redak_b, stupac_b;

    cout << "Redovi: 1 - 20" << endl;
    cout << "Stupci: 1 - 40" << endl;

    cout << "Unesite redak to�ke A: ";
    cin >> redak_a;
    cout << "Unesite stupac to�ke A: ";
    cin >> stupac_a;
    cout << "Unesite redak to�ke B: ";
    cin >> redak_b;
    cout << "Unesite stupac to�ke B: ";
    cin >> stupac_b;

    NajkraciPut putanja(redak_a, stupac_a, redak_b, stupac_b, zidovi);

    return 0;
}
