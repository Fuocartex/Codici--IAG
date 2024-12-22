#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

//Partiamo dai simplessi massimali e generiamo i loro sottoinsiemi di tutte le dimensioni minori o uguali.
//Aggiungiamo questi sottoinsiemi alla lista dei simplessi corrispondenti.

// Funzione per generare tutti i sottoinsiemi di un dato simplesso (escluso il vuoto)
//Utilizza la rappresentazione binaria per ogni possibile sottoinsieme: se un simplesso ha n di elementi, ogni sottoinsieme di questi elementi 
//può essere rappresentato come una sequenza di 0 e 1 (un numero binario). Ogni bit in questa sequenza rappresenta se un elemento è incluso nel 
//sottoinsieme o meno. In pratica, per ogni sottoinsieme, il codice crea una sequenza di bit (di lunghezza n, dove n è il numero di elementi nel 
//simplesso) e, per ogni bit che è uguale a 1, l'elemento corrispondente viene incluso nel sottoinsieme.
vector<vector<int>> generateSubsets(const vector<int>& simplesso) {
    vector<vector<int>> subsets;
    int n = simplesso.size();
    // Genera tutti i sottoinsiemi 
    for (int i = 1; i < (1 << n); ++i) { // i va da 1 a 2^n-1
        vector<int> subset;
        for (int j = 0; j < n; ++j) {
            if (i & (1 << j)) { // se al posto j c'è un 1 il corrispondente elemento viene inserito nell'insieme
                subset.push_back(simplesso[j]);
            }
        }
        subsets.push_back(subset);
    }
    return subsets;
}

// Funzione per ordinare i simplessi e eliminarli se sono duplicati
void removeDuplicatesAndSort(vector<vector<int>>& simplessi) {
    // Ordina ogni simplesso
    for (auto& simplesso : simplessi) {
        sort(simplesso.begin(), simplesso.end());
    }
    // Elimina i duplicati dai simplessi generati per ogni dimensione usando un set
    //Creazione di un set di simplessi unici:
    set<vector<int>> uniqueSimplessi(simplessi.begin(), simplessi.end()); //viene creato un set di vector<int>, il costruttore prende un
	//intervallo di elementi (in questo caso l'intervallo da simplessi.begin() a simplessi.end()) e li inserisce nel set.
	//Poiché un set in C++ non permette duplicati, qualsiasi duplicato presente nei simplessi originali verrà automaticamente rimosso.
	//Assegnazione degli elementi unici alla lista simplessi:
    simplessi.assign(uniqueSimplessi.begin(), uniqueSimplessi.end()); //Dopo che i duplicati sono stati rimossi, i simplessi unici vengono 
	//assegnati di nuovo alla lista originale simplessi. 
}

int main() {
    int k;
    cout << "Inserisci il numero massimo di dimensioni (k): ";
    cin >> k;

    // Lista di simplessi massimali per ogni dimensione da 0 a k
    vector<vector<vector<int>>> simplessiMassimali(k + 1);
	
	//L'utente fornisce i simplessi massimali di ogni dimensione, che vengono memorizzati in simplessiMassimali.
    // Input dei simplessi massimali
    for (int dim = 0; dim <= k; ++dim) {
        int numSimplessi;
        cout << "Inserisci il numero di " << dim << "-simplessi massimali: ";
        cin >> numSimplessi;

        simplessiMassimali[dim].resize(numSimplessi);
        for (int i = 0; i < numSimplessi; ++i) {
            int m = dim + 1;
            simplessiMassimali[dim][i].resize(m);
            cout << "Inserisci gli elementi del " << dim << "-simplesso " << i + 1 << ": ";
            for (int j = 0; j < m; ++j) {
                cin >> simplessiMassimali[dim][i][j];
            }
        }
    }

    // Lista per memorizzare tutti i simplessi generati
    vector<vector<vector<int>>> tuttiSimplessi(k + 1);
	//Per ogni simplesso massimale, vengono generati tutti i suoi sottoinsiemi e aggiunti alla lista dei simplessi di dimensione appropriata.
    // Genera tutti i sottoinsiemi per ogni simplesso massimale
    for (int dim = 0; dim <= k; ++dim) {
        for (const auto& simplesso : simplessiMassimali[dim]) {
            // Genera i sottoinsiemi
            vector<vector<int>> subsets = generateSubsets(simplesso);
            // Aggiungi i sottoinsiemi alla lista dei simplessi della dimensione corretta
            for (auto& subset : subsets) {
                int subsetDim = subset.size() - 1;
                if (subsetDim <= dim) {
                    tuttiSimplessi[subsetDim].push_back(subset);
                }
            }
        }
    }
	//I duplicati vengono rimossi e i simplessi vengono ordinati per garantire che i risultati siano consistenti.
    // Elimina i duplicati dai simplessi generati e ordina
    for (int dim = 0; dim <= k; ++dim) {
        removeDuplicatesAndSort(tuttiSimplessi[dim]);
    }
	
	//Vengono stampati tutti i simplessi generati per ogni dimensione, inclusi i sottoinsiemi derivanti dai simplessi massimali.
    // Output dei simplessi generati
    cout << "Tutti i simplessi generati sono:\n";
    for (int dim = 0; dim <= k; ++dim) {
        if (!tuttiSimplessi[dim].empty()) {
            cout << dim << "-simplessi generati:\n";
            for (const auto& simplesso : tuttiSimplessi[dim]) {
                cout << "{";
                for (size_t i = 0; i < simplesso.size(); ++i) {
                    if (i > 0) cout << ", ";
                    cout << simplesso[i];
                }
                cout << "}\n";
            }
        }
    }

    return 0;
}
