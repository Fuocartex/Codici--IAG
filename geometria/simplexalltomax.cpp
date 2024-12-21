#include <iostream> //Permette l'input/output standard (ad esempio cin e cout).
#include <vector> // Aggiunge il supporto per la struttura dati vector, usata per rappresentare liste di simplessi.
#include <algorithm> //Fornisce funzioni di utilità come sort e includes, utilizzate per l'ordinamento e il controllo dei sottoinsiemi.
#include <set>

using namespace std;

// Funzione per verificare se un simplesso subset (un insieme più piccolo) è un sottoinsieme di un altro simplesso superset (un insieme più grande).
// Utilizza std::includes, che richiede che entrambi gli input siano ordinati.
// Restituisce true se tutti gli elementi di subset sono contenuti in superset e false altrimenti.
bool isSubset(const vector<int>& subset, const vector<int>& superset) {
    return includes(superset.begin(), superset.end(), subset.begin(), subset.end());
}

int main() {
	//L'utente inserisce k, il numero massimo di dimensioni dei simplessi.
    int k;
    cout << "Inserisci il numero massimo di dimensioni (k): ";
    cin >> k;

    vector<vector<vector<int>>> simplessi(k + 1); // Lista di liste per ogni dimensione da 0 a k.

    // Costruzione automatica degli 0-simplessi
    int numZeroSimplessi;
    cout << "Inserisci il numero di 0-simplessi: ";
    cin >> numZeroSimplessi;
    //L'utente specifica il numero di 0-simplessi.
    simplessi[0].resize(numZeroSimplessi);
    for (int i = 0; i < numZeroSimplessi; ++i) {
        simplessi[0][i].push_back(i + 1); // Inserisce i numeri interi da 1 a numZeroSimplessi
    }

    // Input delle liste di simplessi per dimensioni maggiori di 0
    //Per ogni dimensione dim (da 1 a k): chiede il numero di simplessi di quella dimensione; per ciascun simplesso, l'utente inserisce gli elementi.
	//Gli elementi di ciascun simplesso vengono ordinati, requisito necessario per il funzionamento di isSubset.
    for (int dim = 1; dim <= k; ++dim) {
        int numSimplessi;
        cout << "Inserisci il numero di " << dim << "-simplessi: ";
        cin >> numSimplessi;

        simplessi[dim].resize(numSimplessi);
        for (int i = 0; i < numSimplessi; ++i) {
            int m = dim + 1; // Ogni simplesso ha (dim + 1) elementi
            simplessi[dim][i].resize(m);
            cout << "Inserisci gli elementi del " << dim << "-simplesso " << i + 1 << ": ";
            for (int j = 0; j < m; ++j) {
                cin >> simplessi[dim][i][j];
            }
            sort(simplessi[dim][i].begin(), simplessi[dim][i].end()); // Ordina gli elementi
        }
    }

    // Processo per trovare i simplessi massimali
    //Analizza i simplessi a partire da quelli di dimensione maggiore (k) fino agli 0-simplessi.
	//Per ogni simplesso verifica se è contenuto in un simplesso di dimensione maggiore:
	//se lo è, viene scartato; altrimenti, viene aggiunto alla lista dei nuovi simplessi.
    for (int dim = k - 1; dim >= 0; --dim) { // Partendo dalle dimensioni maggiori
        vector<vector<int>> nuoviSimplessi;
        for (const auto& simplesso : simplessi[dim]) {
            bool isMaximal = true;
            for (int superioreDim = dim + 1; superioreDim <= k; ++superioreDim) {
                for (const auto& superiore : simplessi[superioreDim]) {
                    if (isSubset(simplesso, superiore)) {
                        isMaximal = false;
                        break;
                    }
                }
                if (!isMaximal) break;
            }
            if (isMaximal) {
                nuoviSimplessi.push_back(simplesso);
            }
        }
        simplessi[dim] = nuoviSimplessi; // Aggiorna la lista dei simplessi
    }

    // Output dei complessi massimali
    //Per ogni dimensione dim stampa i simplessi massimali rimanenti, ogni simplesso viene stampato nel formato {elemento1, elemento2, ...}.
    cout << "I complessi massimali sono:\n";
    for (int dim = 0; dim <= k; ++dim) {
        if (!simplessi[dim].empty()) {
            cout << dim << "-simplessi massimali:\n";
            for (const auto& simplesso : simplessi[dim]) {
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
