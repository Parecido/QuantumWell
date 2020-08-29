#include "Well.hpp"

Well::Well(double wellV, int N, WellType wType) {
    this->wellV = wellV;
    this->N = N;
    this->wType = wType;
}

double Well::getAlpha(int x) {
    return res[x].first;
}

double Well::getPsi(int x) {
    return res[x].second;
}

void Well::addPotential(double wellL, int wellN, double potL) {
    potL = (potL / wellL) * 10000.0;
    double delta = (10000.0 - (wellN - 1) * potL) / wellN;
    double start = -5000.0 + delta;
    double end = start + potL;

    for (int i = 1; i <= wellN - 1; i++) {
        if (end > 0) pot.push_back(make_pair(start, end));
        start += potL + delta;
        end += delta + potL;
    }

    pot.push_back(make_pair(5000, N));
}

void Well::calculate(ofstream& output) {
    int index = 0;
    vector<int> matrix;
    scanAlpha(index, matrix, 10000, output);
}

void Well::scanAlpha(int& index, vector<int>& matrix, int N, ofstream& output) {
    if (index >= matrix.size()) matrix.push_back(0);
    
    for (matrix[index] = 0; matrix[index] <= N; matrix[index]++) {
        double alpha = 0;          
        for (int i = 0; i < matrix.size(); i++)
            alpha += (index > i ? matrix[i] - 1 : matrix[i]) * 0.0001 * pow(0.01, i);

        cout << setprecision(2) << fixed << "Trwa obliczanie: " << alpha * 100 << "%\r" << flush;
        calculatePsi(alpha, matrix[index], false, output);

        if (matrix[index] > 0 && getPsi(matrix[index]) * getPsi(matrix[index] - 1) <= 0) {
            double precision = getAlpha(matrix[index]) - getAlpha(matrix[index] - 1);
            if (precision < 0.0000001)
                if (abs(getPsi(matrix[index])) < abs(getPsi(matrix[index] - 1)))
                    saveAlpha(matrix[index], output);
                else
                    saveAlpha(matrix[index] - 1, output);
            else
                scanAlpha(++index, matrix, 100, output);
        }
    }

    matrix[index] = 0;
    index--;
}

void Well::saveAlpha(int x, ofstream& output) {
    cout << setprecision(8) << fixed << "Trwa zapisywanie: " << getAlpha(x) << endl;
    calculatePsi(getAlpha(x), x, true, output);
}

void Well::calculatePsi(double alpha, int x, bool saveToFile, ofstream& output) {
    double psi, deltaPsi, deltaPsi_deltaU, c_psi_deltaU;
    vector<psiData> pointList;

    psi = wType == EVEN ? 1 : 0;
    deltaPsi = wType == EVEN ? 0 : 1;

    deltaPsi_deltaU = deltaPsi * 0.0001;
    c_psi_deltaU = calculateC(0, alpha) * psi * 0.0001;

    if (saveToFile) pointList.push_back(make_pair(0, psi));

    for (int u = 1; u <= N; u++) {
        psi = psi + deltaPsi_deltaU;
        deltaPsi = deltaPsi + c_psi_deltaU;

        deltaPsi_deltaU = deltaPsi * 0.0001;
        c_psi_deltaU = calculateC(u, alpha) * psi * 0.0001;

        if (saveToFile) pointList.push_back(make_pair(u * 0.0001, psi));
    }

    if (saveToFile) {
        reduce(pointList);
        
        double norm = sqrt(2 * trapezoidal(pointList) * 0.0001);
        save(alpha, norm, pointList, output);
    }
    
    if (x >= res.size()) res.push_back(make_pair(0, 0));

    res[x] = make_pair(alpha, psi);
}

double Well::calculateC(int u, double alpha) {
    for (int i = 0; i < pot.size(); i++)
        if (u >= pot[i].first && u <= pot[i].second)
            return wellV * (1 - alpha);

    return -wellV * alpha;
}

void Well::save(double alpha, double norm, vector<psiData>& pointList, ofstream& output) {
    output << "\nPsi:" << " ";
    output << setprecision(8) << fixed << alpha << " ";
    output << wType << " ";
    output << pointList.size() << "\n";

    for (int i = 0; i < pointList.size(); i++) {
        output << setprecision(4) << fixed << pointList[i].first << " ";
        output << setprecision(8) << fixed << pointList[i].second / norm << "\n";
    }
}

void Well::reduce(vector<psiData>& points) {
    int toReduce = 0;

    for (int i = 0; i < points.size(); i++) {
        double u = points[i].first * 10000;

        if (u >= pot.back().first) {
            double Y1 = points[i - 1].second;
            double Y2 = points[i].second;
            double powY1 = pow(Y1, 2);
            double powY2 = pow(Y2, 2);
                
            if (powY1 <= powY2 || Y1 * Y2 <= 0)
                toReduce++;
        }
    }

    int size = points.size() - toReduce;
    points.resize(size);
}

double Well::trapezoidal(vector<psiData>& points) {
    double P = pow(points.front().second, 2) / 2.0;
    
    for (int i = 1; i < points.size() - 1; i++)
        P += pow(points[i].second, 2);
    
    P += pow(points.back().second, 2) / 2.0;
    
    return P;
}