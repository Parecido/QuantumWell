#include "Well.hpp"

int main (int argc, char *argv[]) {
    int N, wellN;
    double wellV, wellL, potL;

    cout << "***         Symulacja czastki w studni potencjalu         ***" << endl;
    cout << "***     Poszukiwanie parametru alpha w zakresie 0 - 1     ***" << endl;
    cout << "***                autor: Michal Michalski                ***" << endl;

    cout << "\nPodaj glebokosc studni [eV]: ";
    cin >> wellV;
    cout << "Podaj szerokosc studni [nm]: ";
    cin >> wellL;
    cout << "Podaj liczbe pomiarow: ";
    cin >> N;
    cout << "Podaj krotnosc studni: ";
    cin >> wellN;
  
    if (wellN >= 2) {
        cout << "Podaj szerokosc bariery potencjalu [nm]: ";
        cin >> potL;
    }

    ofstream output("data.dat");
    output << "Glebokosc studni: " << wellV << "\n";
    output << "Szerokosc studni: " << wellL << "\n";
    output << "Krotnosc studni: " << wellN << "\n";
    
    if (wellN >= 2)
        output << "Szerokosc bariery: " << potL << "\n";
    
    wellL = wellN * wellL + (wellN - 1) * potL;
        
    wellV *= 0.0367490;     // eV -> Hartree
    wellL *= 18.8971616;    // nm -> Bohr
    potL *= 18.8971616;     // nm -> Bohr
        
    wellV = 2 * pow(wellL, 2) * wellV;

    cout << "\nZaczynam obliczac funkcje parzyste" << endl;
    Well wellEven(wellV, N, EVEN);
    wellEven.addPotential(wellL, wellN, potL);
    wellEven.calculate(output);

    cout << "\n\nZaczynam obliczac funkcje nieparzyste" << endl;
    Well wellOdd(wellV, N, ODD);
    wellOdd.addPotential(wellL, wellN, potL);
    wellOdd.calculate(output);

    output.close();

    cout << "\n\nZakonczono" << endl;
    return 0;
}
