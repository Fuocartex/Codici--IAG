#include <stdio.h>
#include <stdlib.h>

// Calcolo del MCD
long long __mcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Funzione per l'esponenziazione modulare binaria
long long moduloExponentiation(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp / 2;
        base = (base * base) % mod;
    }
    return result;
}

// Funzione ausiliaria per verificare se n è composto
// Se a^d mod n = x = 1,-1 cambio a, altrimenti eseguo r-1 iterazioni calcolando x=x^2 mod n.
// Se x non diventa mai -1 allora è sicuramente composto. 
int isComposite(long long n, long long a, long long d, long long h) {
    long long x = moduloExponentiation(a, d, n);

    // Controllo 1: Se a^d mod n = 1, -1, non posso concludere che sia composto (soddisfa la condizione di Fermat)
    // in tal caso nel test cambio testimone a.
    if (x == 1 || x == n - 1) {
        return 0; // Non è sicuramente composto.
    }

    // Itero attraverso le squadrature
    for (long long i = 1; i < h; i++) {
        long long prev = x;
        x = (x * x) % n;

        // Controllo 2: Se x == 1 e il precedente != -1, concludo composto
        if (x == 1) {
            if (prev != n - 1) {
                return 1; // Sicuramente composto
            } else {
                return 0; // Non è sicuramente composto, cambio a
            }
        }

        // Controllo 3: Se x == n - 1, esco dal ciclo, cambio a
        if (x == n - 1) {
            return 0; // Non è sicuramente composto
        }
    }

    // Controllo 4: Se x != 1 dopo tutte le iterazioni, concludo composto (fallisce Fermat)
    if (x != 1) {
        return 1; // Sicuramente composto
    }

    return 1; // Sicuramente composto
}

// Test di Miller-Rabin
int millerRabin(long long n, int iterations) {
    if (n < 2) {
        return 0; // I numeri < 2 non sono primi
    }
    if (n == 2 || n == 3) {
        return 1; // 2 e 3 sono primi
    }
    if (n % 2 == 0) {
        return 0; // I numeri pari > 2 non sono primi
    }

    // Scriviamo n-1 come 2^h * d dove d è dispari, serve per scrivere a^(n-1)=a^(d2^h)
	//Questo serve per eseguire il test sui valori di a^d mod n e successivi quadrati.
    long long d = n - 1;
    long long h = 0;
    while (d % 2 == 0) {
        d /= 2;
        h++;
    }

    // Eseguiamo il test per un certo numero di iterazioni
    for (int i = 0; i < iterations; i++) {
        long long a = 2 + rand() % (n - 4); // Base casuale tra [2, n-2]

        // Controllo MCD per evitare test inutili
        long long g = __mcd(a, n);
        if (g > 1) {
            return 0; // Sicuramente composto
        }

        if (isComposite(n, a, d, h)) {
            return 0; // Se è sicuramente composto, non è primo
        }
    }

    return 1; // Probabilmente primo
}

// Main per testare il codice
int main() {
    long long n;
    int iterations;

    printf("Inserisci un numero da verificare: ");
    scanf("%lld", &n);

    printf("Inserisci il numero di iterazioni del test: ");
    scanf("%d", &iterations);

    if (millerRabin(n, iterations)) {
        printf("%lld e' molto probabilmente primo.\n", n); // p=1/2^2k (più forte del test di Solovay-Strassen)
    } else {
        printf("%lld e' composto.\n", n);
    }

    return 0;
}
	