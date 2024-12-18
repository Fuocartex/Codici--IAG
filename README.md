## Informazioni sulla libreria gmp

La libreria gmp permette di gestire numeri arbitrariamente grandi, nel nostro caso interi; è scaricabile al sito [https://gmplib.org/](https://gmplib.org/), e lì si trova anche la documentazione completa:
[https://gmplib.org/manual/](https://gmplib.org/manual/). Riporto qui per comodità qualche comando utile per il nostro progetto e quello che ho capito facendo qualche esperimento
(potrei aggiornare questo documento in futuro).

Una volta installata (io l'avevo già, controllate anche voi), per usarla in un programma basta scrivere `#include <gmp.h>`.
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
(qui appena riesco scriverò una raccolta di funzioni che possono tornarci utili nei vari programmi; in ogni caso sono tutte disponibili nella documentazione)


