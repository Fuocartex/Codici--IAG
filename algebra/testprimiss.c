#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

// Funzione per calcolare il massimo comune divisore (MCD)
long long mcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Funzione per calcolare (base^exp) % mod utilizzando l'esponenziazione modulare binaria
//L'idea chiave è rappresentare l'esponente exp in forma binaria.
//Invece di calcolare ogni potenza da zero, utilizziamo una ricorsione interna:
//Calcoliamo le potenze precedenti raddoppiando la base (quadrati).
//Moltiplichiamo solo se il bit corrente dell'esponente è 1.
long long moduloExponentiation(long long base, long long exp, long long mod) {
    long long result = 1;     // Iniziamo il risultato con 1 (neutro della moltiplicazione)
    base = base % mod;        // Riduciamo la base a base % mod (per evitare numeri troppo grandi)

    while (exp > 0) {         // Ripetiamo finché exp non diventa 0
        if (exp % 2 == 1) {   // Se il bit corrente di exp è 1 ossia se l'esponente è dispari
            result = (result * base) % mod; // Moltiplichiamo il risultato corrente per la base
        }
        exp = exp / 2;        // Shift a destra: dividiamo exp per 2 (passiamo al prossimo bit)
        base = (base * base) % mod; // Quadrato della base (per il prossimo livello)
    }

    return result;            // Restituiamo il risultato finale
}

// Funzione per calcolare il simbolo di Jacobi (a/n)
int jacobiSymbol(long long a, long long n) {
    if (a == 0) return 0; // Se a è 0, il simbolo di Jacobi è 0
    if (a == 1) return 1; // Se a è 1, il simbolo di Jacobi è 1
    
    if (mcd(a, n) != 1) {
        return 0; // Se a e n non sono coprimi, il simbolo di Jacobi è 0
    }

    int result = 1;
	// Gestione numeri negativi
    if (a < 0) { // Se a è negativo
        a = -a;
        if (n % 4 == 3) {
            result = -result; // Cambia segno se n mod 4 == 3
        }
    }

    while (a != 0) { //regola di riduzione per i numeri pari
        while (a % 2 == 0) { // Rimuoviamo i fattori di 2 da a
            a /= 2;
            if (n % 8 == 3 || n % 8 == 5) {
                result = -result; // Cambia segno per proprietà modulari
            }
        }

        // Scambia a e n (legge di reciprocità)
        long long temp = a;
        a = n;
        n = temp;

        if (a % 4 == 3 && n % 4 == 3) {
            result = -result; // Cambia segno se entrambi a e n sono 3 mod 4
        }

        a = a % n; // Riduci a modulo n
    }

    return (n == 1) ? result : 0;
}

// Funzione principale del test di Solovay-Strassen
int solovayStrassen(long long n, int iterations) {
	// Gestione casi base
    if (n < 2) return 0;              // Numeri minori di 2 non sono primi
    if (n == 2) return 1;             // 2 è primo
    if (n % 2 == 0) return 0;         // Numeri pari > 2 non sono primi

	// Eseguiamo il test per un certo numero di iterazioni
    for (int i = 0; i < iterations; i++) {
        // Genera un numero casuale a nell'intervallo [2, n-1] (base casuale)
        long long a = 2 + rand() % (n - 3);

        // Controllo 1: Se mcd(a, n) != 1, n è composto, evitiamo di calcolare il simbolo di Jacobi inutilmente
        if (mcd(a, n) != 1) {
            return 0; // Sicuramente composto
        }

        // Calcola a^((n-1)/2) % n (esponenziazione modulare)
        long long modExp = moduloExponentiation(a, (n - 1) / 2, n);
        
         // Controllo 2: Se modExp != 1 e modExp != n-1, n è composto
        if (modExp != 1 && modExp != n - 1) {
            return 0; // Sicuramente composto
        }
        
        // Calcola il simbolo di Jacobi (a/n)
        int jacobi = jacobiSymbol(a, n);
        if (jacobi == 0) return 0;    // Se il simbolo di Jacobi è 0, n non è primo, sicuramente composto

		//// Controllo 3: Verifichiamo se il valore modulare coincide con il simbolo di Jacobi (mod n).
		//Se il test fallisce per un a, allora n non è primo.
		//Se passa tutte le iterazioni, n è probabilmente primo.
        // Verifica se il test fallisce
        if (modExp != (jacobi + n) % n) {
            return 0; // n non è primo
        }
    }

    return 1; // Probabilmente primo
}

// Main per testare il codice
int main() {
    long long n;
    int iterations;

    printf("Inserisci il numero da testare: ");
    scanf("%lld", &n);

    printf("Inserisci il numero di iterazioni k: ");
    scanf("%d", &iterations);

    if (solovayStrassen(n, iterations)) {
        printf("%lld e' molto probabilmente primo.\n", n); // probabilità pari a 1/2^k
    } else {
        printf("%lld non e' primo.\n", n);
    }

    return 0;
}
