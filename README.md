## Informazioni sulla libreria gmp

La libreria gmp permette di gestire numeri arbitrariamente grandi, nel nostro caso interi; è scaricabile al sito [https://gmplib.org/](https://gmplib.org/), e lì si trova anche la documentazione completa:
[https://gmplib.org/manual/](https://gmplib.org/manual/). Riporto qui per comodità qualche comando utile per il nostro progetto e quello che ho capito facendo qualche esperimento
(potrei aggiornare questo documento in futuro).

Una volta installata (io l'avevo già, controllate anche voi), per usarla in un programma basta scrivere `#include <gmp.h>`. Se la utilizzate, scrivete `#include <stdio.h>` prima di includere la libreria gmp.
In fase di compilazione (almeno per me) bisogna linkare la libreria, con `gcc nomedelprogramma.c -lgmp`.

---

Il tipo che la libreria definisce per gli interi a precisione arbitraria è `mpz_t`. Questi oggetti sono definiti come **puntatore** ad una certa struttura (interna alla libreria, che gestisce le varie operazioni),
quindi tutte le volte in cui si va ad operare su questi interi, si passa automaticamente il riferimento, non solo il valore. Questo significa che le funzioni che operano su `mpz_t` non restituiscono un oggetto
di tipo `mpz_t`; il risultato viene salvato su uno degli argomenti della funzione.

Tutte le variabili di tipo `mpz_t`, oltre ad essere dichiarate, vanno anche inizializzate con la funzione `mpz_init`, dopodiché è possibile assegnare un valore alla variabile.
Tutte le variabili di tipo `mpz_t` vanno **deallocate** quando si ha finito di utilizzarle, con la funzione `mpz_clear`. Ad esempio:
```
  mpz_t z;
  mpz_init(z);
  // utilizzo della variabile
  ...
  mpz_clear(z);
```

Per quanto riguarda gli array, ho fatto un piccolo esperimento e mi sembra che trattarli come se fossero tipi soliti funzioni, quindi qualcosa del tipo:
```
  // voglio creare un array di n interi:
  mpz_t *integers;
  integers = calloc(n,sizeof(mpz_t));
  for (int i=0;i<n;i++) { mpz_init(integers[i]); } //inizializzazione
  ...
  for (int i=0;i<n;i++) { mpz_clear(integers[i]); } // deallocazione
```

---
## Alcune funzioni utili:
Qui raccoglierò le funzioni che penso utilizzeremo più spesso, in modo da semplificarci l'utilizzo della libreria. Non le inserisco tutte perché diventerebbe troppo lungo, quindi se incontrate qualche problema
oppure vi sembra di non avere la funzione che vi serve potete guardare nella documentazione o chiedere a me.

Tipicamente le funzioni hanno come primi argomenti le variabili dove verranno salvati i risultati delle operazioni, e successivamente gli argomenti da usare per le operazioni.
Dove non scrivo alcun valore di ritorno della funzione, è perché la funzione è di tipo `void`.

### Inizializzazione e assegnazione
```
  mpz_set (mpz_t a, mpz_t b)
  mpz_set_si (mpz_t a, signed long int b)
```
Assegna alla variabile a il valore di b.
```
  mpz_init_set (mpz_t a, mpz_t b)
  mpz_init_set_si (mpz_t a, signed long int b)
```
Combina le operazioni di inizializzazione della funzione `mpz_init` con l'assegnazione del valore (analoga alle due funzioni precedenti). **Non usare** funzioni di inizializzazione su interi già inizializzati.
```
  mpz_swap (mpz_t a, mpz_t b)
```
Scambia il valore di a e b.

### Operazioni aritmetiche
```
  mpz_add (mpz_t a, mpz_t b, mpz_t c)
  mpz_add_ui (mpz_t a, mpz_t b, unsigned long int c)
  mpz_sub (mpz_t a, mpz_t b, mpz_t c)
  mpz_sub_ui (mpz_t a, mpz_t b, mpz_t c)
```
Calcola rispettivamente la somma b+c / la differenza b-c, e salva il risultato in a.
```
  mpz_mul (mpz_t a, mpz_t b, mpz_t c)
  mpz_mul_si (mpz_t a, mpz_t b, signed long int c)
```
Calcola il prodotto b*c e salva il risultato in a.
```
  mpz_neg (mpz_t a, mpz_t b)
```
Cambia segno a b e salva il risultato in a.
```
  mpz_abs (mpz_t a, mpz_t b)
```
Calcola il valore assoluto di b e salva il risultato in a.
```
  mpz_fdiv_q (mpz_t q, mpz_t n, mpz_t d)
  mpz_fdiv_r (mpz_t r, mpz_t n, mpz_t d)
  mpz_fdiv_qr (mpz_t q, mpz_t r, mpz_t n, mpz_t d)
  unsigned long int mpz_fdiv_q_ui (mpz_t q, mpz_t n, unsigned long int d)
  unsigned long int mpz_fdiv_r_ui (mpz_t r, mpz_t n, unsigned long int d)
  unsigned long int mpz_fdiv_qr_ui (mpz_t q, mpz_t r, mpz_t n, unsigned long int d)
```
Calcola n diviso d, formando il quoziente q e il resto r: n=q*d+r, con |r|<|d|. Le funzioni q salvano il quoziente, r salvano il resto, qr salvano entrambi. Le funzioni ui agiscono in modo simile ma restituiscono
anche il resto r. In queste funzioni r avrà lo stesso segno di d (esistono altre varianti delle funzioni di divisione che gestiscono i segni di q,r diversamente: mi sembra che queste (con la f) siano le più
adatte a noi, ma per più informazioni rimando alla documentazione).
```
  mpz_mod (mpz_t r, mpz_t n, mpz_t d)
```
Calcola n modulo d e salva il risultato in r; il risultato prodotto è sempre >=0.

### Operazioni di confronto
```
  int mpz_cmp (mpz_t a, mpz_t b)
  int mpz_cmp_si (mpz_t a, signed long int b)
```
Restituisce un valore positivo se a>b, zero se a=b, negativo se a<b.
```
  int mpz_sgn (mpz_t a)
```
Restituisce +1 se a>0, 0 se a=0, -1 se a<0.
```
  int mpz_even_p (mpz_t a)
```
Restituisce un valore non-zero se a è pari, 0 se è dispari.

### Operazioni bitwise
```
  mpz_mul_2exp (mpz_t a, mpz_t b, unsigned long int exp)
  mpz_fdiv_q_2exp (mpz_t q, mpz_t n, unsigned long int exp)
  mpz_fdiv_r_2exp (mpz_t r, mpz_t n, unsigned long int exp)
```
Eseguono moltiplicazione e divisione analogamente alle funzioni precedenti, ma moltiplicano / dividono per 2^exp. A livello pratico è realizzato con rispettivamente left shift e right shift dei bit, quindi
sono molto efficienti: *è consigliato utilizzarle* al posto delle normali funzioni di moltiplicazione e divisione quando abbiamo a che fare con potenze di due.
```
  unsigned long int mpz_scan0 (mpz_t a, unsigned long int startingbit)
  unsigned long int mpz_scan1 (mpz_t a, unsigned long int startingbit)
```
Scannerizza i bit dell'intero a, a partire dall'indice specificato da startingbit, verso le cifre più significative ("verso sinistra"), finché non viene trovato il primo 0 (risp. 1). Ad esempio: se chiamo
`mpz_scan1(a, 0)` con a=3 ottengo 0 (3=11 in binario), con a=32 ottengo 5 (32=100000 in binario).
```
  int mpz_tstbit (mpz_t a, unsigned long int index)
```
Controlla il bit di a nella posizione specificata da index, e restituisce 0 o 1 a seconda di quale bit ci sia. Ad esempio, `mpz_tstbit(a,0)` restituirà 1 se a è dispari, 0 se è pari.

### Input e output
```
  gmp_printf
  gmp_scanf
```
Funzionano come printf e scanf in C, ma permettono di utilizzare anche lo specificatore di formato `%Zd` per il tipo `mpz_t`. Poiché `mpz_t` è un puntatore, non c'è bisogno dell'operatore `&` in `scanf`.

### Numeri casuali
Per generare numeri casuali la libreria richiede che sia inizializzato un oggetto di tipo `gmp_randstate_t` (che a fine programma dovrà essere deallocato con `gmp_randclear()`) e che gli venga assegnato un seed
iniziale. Questo si può fare con le funzioni:
```
  gmp_randinit_default (gmp_randstate_t state)
  gmp_randseed_ui (gmp_randstate_t state, unsigned long int seed)
```
Una volta fatto questo, è possibile chiamare le funzioni che producono numeri casuali, come:
```
  mpz_urandomm (mpz_t a, gmp_randstate_t state, mpz_t n)
  mpz_urandomb (mpz_t a, gmp_randstate_t state, unsigned long int exp)
```
La prima genera un numero intero casuale compreso tra 0 e n-1 inclusi, la seconda un numero tra 0 e 2^n -1 inclusi; il numero prodotto viene salvato in a.

Per ottenere sequenze di numeri sempre diversi ho trovato abbastanza soddisfacente usare il tempo come seed delle funzioni, quindi qualcosa del tipo:
```
  #include <time.h>
  #include <gmp.h>
  ...
  // inizializzazione e assegnazione del seed:
  gmp_randstate_t state;
  gmp_randinit_default(state);
  unsigned long int t = time(NULL);
  gmp_randseed_ui(state, t);
  // qui si possono chiamare le funzioni mpz_urandomm() e mpz_urandomb()
  ...
  gmp_randclear(randstate);
```

### Funzioni utili per test
Ho trovato qualche funzione aritmetica che ci può essere utile per fare dei test, verificare la correttezza dei nostri programmi e fare possibili confronti. Queste funzioni **non dovranno essere nella versione
finale** dato che sono algoritmi che dovremmo scrivere noi.
```
  mpz_gcd (mpz_t a, mpz_t b, mpz_t c)
  mpz_gcdext (mpz_t a, mpz_t x, mpz_t y, mpz_t b, mpz_t c)
```
Salvano in a il massimo comun divisore di b e c; la seconda funzione in aggiunta fornisce x e y tali che a = b`*`x + c*y.
```
  int mpz_invert (mpz_t a, mpz_t x, mpz_t n)
```
Calcola l'inverso di x modulo n e salva il risultato in a. Se l'inverso esiste la funzione restituisce un valore non nullo; altrimenti restituisce 0.
```
  int mpz_jacobi (mpz_t a, mpz_t b)
```
Calcola il simbolo di Jacobi (a/b); definita solo per b dispari.
```
  int mpz_probab_prime_p (mpz_t n, int iter)
```
Questo è un test di primalità (implementa, tra le altre cose, il test di Miller-Rabin!). Restituisce 2 se n è certamente primo, 1 se è probabilmente primo (con probabilità di errore inferiore a 4^(-iter)), 0 se
non è primo. (Consigliano valori di iter compresi tra 15 e 50).
