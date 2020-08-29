import matplotlib.pyplot as plt

class Psi:
    alpha = 0
    isOdd = False
    points = []

    def __init__(self, alpha, isOdd, points):
        self.alpha = alpha
        self.isOdd = isOdd
        self.points = points
        
    def getX(self):
        return [point.X for point in self.points]
            
    def getY(self):
        return [point.Y for point in self.points]        
        
class Point:
    X = 0
    Y = 0

    def __init__(self, X, Y):
        self.X = X
        self.Y = Y
        
def chunks(l, n):
    for i in range(0, len(l), n):
        yield l[i:i + n]

def printWell():
    fig, ax = plt.subplots()
    
    L = (wellN * wellL + (wellN - 1) * potL) / 2
    potU = [-L - (0.5 * L), -L, -L]
    potE = [wellV, wellV, 0]

    start = -L + wellL
    end = start + potL

    for x in range(wellN - 1):
        potU += [start, start, end, end]
        potE += [0, wellV, wellV, 0]

        start += potL + wellL
        end += wellL + potL

    potU += [L, L, L + (0.5 * L)]
    potE += [0, wellV, wellV]

    ax.plot(potU, potE, color="black")
    ax.set(xlabel='Szerokość [nm]', ylabel='Energia [eV]', title='Studnia potencjału')
    fig.savefig('well_clean.png', dpi=300)

    for psi in psi_normal:
        potU = [-L, L]
        potE = [wellV * psi.alpha, wellV * psi.alpha]
        ax.plot(potU, potE, label=str(psi.alpha))

    ax.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0., title=r'$\alpha$')
    fig.savefig('well_energy.png', bbox_inches='tight', dpi=300)
    plt.close()

def printEveryPsi():
    for psi in psi_normal:
        fig, ax = plt.subplots()
        ax.plot(psi.getX(), psi.getY())
        ax.set(xlabel='u', ylabel=r'$\mathrm{\psi}$', title=r'$\mathrm{\psi(u)}$')
        fig.savefig('normal_' + str(psi.alpha) + '.png', bbox_inches='tight', dpi=300)
        plt.close()
        
def printEveryProb():
    for psi in psi_prob:
        fig, ax = plt.subplots()
        ax.plot(psi.getX(), psi.getY())
        ax.set(xlabel='u', ylabel=r'$\mathrm{|\psi|^{2}}$', title=r'$\mathrm{|\psi(u)|^{2}}$')
        fig.savefig('prob_' + str(psi.alpha) + '.png', bbox_inches='tight', dpi=300)
        plt.close()        
        
def printPsi():
    i = 0
    for reduced in list(chunks(psi_normal, 4)):
        fig, ax = plt.subplots()
        for psi in reduced:
            ax.plot(psi.getX(), psi.getY(), label=str(psi.alpha))
                
        ax.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0., title=r'$\alpha$')
        ax.set(xlabel='u', ylabel=r'$\mathrm{\psi}$', title=r'$\mathrm{\psi(u)}$')
        fig.savefig('all_normal' + str(i) + '.png', bbox_inches='tight', dpi=300)
        plt.close()
        i = i + 1

def printProb():
    i = 0
    for reduced in list(chunks(psi_prob, 4)):
        fig, ax = plt.subplots()
        for psi in reduced:
            ax.plot(psi.getX(), psi.getY(), label=str(psi.alpha))
            
        ax.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0., title=r'$\alpha$')
        ax.set(xlabel='u', ylabel=r'$\mathrm{|\psi|^{2}}$', title=r'$\mathrm{|\psi(u)|^{2}}$')
        fig.savefig('all_prob' + str(i) + '.png', bbox_inches='tight', dpi=300)
        plt.close()
        i = i + 1          

wellV = 0
wellL = 0
wellN = 0
potL = 0
psi_normal = []
psi_prob = []

print('Wczytuję plik data.dat')
with open('data.dat', 'r') as file:
    for line in file:
        if 'Glebokosc studni' in line:
            wellV = float(line.split(':')[1])
        if 'Szerokosc studni' in line:
            wellL = float(line.split(':')[1])
        if 'Krotnosc studni' in line:
            wellN = int(line.split(':')[1])
        if 'Szerokosc bariery' in line:
            potL = float(line.split(':')[1])
        if 'Psi' in line:
            alpha = float(line.split()[1])
            isOdd = bool(int(line.split()[2]))
            size = int(line.split()[3])

            points = []
            for x in range(size):
                psiData = next(file)
                u = float(psiData.split()[0])
                psi = float(psiData.split()[1])

                points.append(Point(u, psi))
                if isOdd == False:
                    points.append(Point(-u, psi))
                else:
                    points.append(Point(-u, -psi))

            points.sort(key=lambda point: point.X)
            psi_normal.append(Psi(alpha, isOdd, points))
            psi_prob.append(Psi(alpha, isOdd, [Point(point.X, point.Y**2) for point in points]))

psi_normal.sort(key=lambda psi: psi.alpha)
psi_prob.sort(key=lambda psi: psi.alpha)

print('Zaczynam generować wykresy studni')
printWell()

print('Zaczynam generować wykresy funkcji')
printPsi()
printProb()
printEveryPsi()
printEveryProb()
