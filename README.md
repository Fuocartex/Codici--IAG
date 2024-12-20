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
### Alcune funzioni utili:
Qui raccoglierò le funzioni che penso utilizzeremo più spesso, in modo da semplificarci l'utilizzo della libreria. Non le inserisco tutte perché diventerebbe troppo lungo, quindi se incontrate qualche problema
oppure vi sembra di non avere la funzione che vi serve potete guardare nella documentazione o chiedere a me.

Tipicamente le funzioni hanno come primi argomenti le variabili dove verranno salvati i risultati delle operazioni, e successivamente gli argomenti da usare per le operazioni.
Dove non scrivo alcun valore di ritorno della funzione, è perché la funzione è di tipo `void`.

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


## da completare




