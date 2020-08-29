#ifndef WELL_HPP
#define WELL_HPP

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

enum WellType {EVEN, ODD};

typedef pair<double, double> boundaryData;  // alpha - psi
typedef pair<double, double> potentialData; // start - end
typedef pair<double, double> psiData;       // u - psi

class Well {
    double wellV;
    int N;
    WellType wType;
    vector<potentialData> pot;
    vector<boundaryData> res;

public:
    Well(double, int, WellType);
    void addPotential(double, int, double);
    void calculate(ofstream&);

private:
    double getAlpha(int);
    double getPsi(int);
    void scanAlpha(int&, vector<int>&, int, ofstream&);
    void saveAlpha(int, ofstream&);
    void calculatePsi(double, int, bool, ofstream&);
    double calculateC(int, double);
    void save(double, double, vector<psiData>&, ofstream&);
    void reduce(vector<psiData>&);
    double trapezoidal(vector<psiData>&);
};

#endif
