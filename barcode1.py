import sys
import json
import matplotlib.pyplot as plt


"""print("Argomenti ricevuti:", sys.argv)
"""
def plot_barcode(matrix, lambda_min, h):
    """
    Data una matrice quadrata (lista di liste) che rappresenta 
    il modulo di persistenza, estrae gli intervalli e disegna il barcode.
    
    Regole:
      - Per ogni riga non nulla (con almeno un valore non zero):
          * Si parte dalla riga (birth = numero della riga, 1-indexed).
          * Si individuano gli elementi non nulli, a partire dall'elemento più a destra
            e poi procedendo verso sinistra.
          * Per ciascun elemento non nullo, l’intervallo è [birth, death] dove:
              - birth = numero della riga (1-indexed);
              - death = numero della colonna (1-indexed).
      - Ogni intervallo viene disegnato come segmento orizzontale e posizionato sulla "prima quota libera"
        (cioè, sul livello y più basso in cui non si sovrappone ad un altro intervallo).
    """
    n = len(matrix)  # dimensione della matrice
    intervals = []   # lista di intervalli: ciascuno è una tupla (birth, death)
    
    # Scorriamo le righe (i indici partono da 0; aggiungiamo 1 per ottenere la 1-indexazione)
    for i, row in enumerate(matrix):
        if any(val != 0 for val in row):
            # Troviamo gli indici degli elementi non nulli e li ordiniamo decrescentemente (da destra a sinistra)
            nonzero_indices = [j for j, val in enumerate(row) if val != 0]
            nonzero_indices.sort(reverse=True)
            
            birth = lambda_min + i*h   # riga corrente in 1-indexing
            for j in nonzero_indices:
                death = lambda_min + j*h  # colonna in 1-indexing
                num_segments = row[j]
                # Aggiungiamo tante istanze quanto il valore in cella
                for _ in range(num_segments):
                    intervals.append((birth, death))

    
    # Ordiniamo gli intervalli per birth (e per death in ordine decrescente se la riga è la stessa)
    intervals.sort(key=lambda x: (x[0], -x[1]))
    
    # Assegniamo la quota (livello) ad ogni intervallo
    levels = []  # levels[l] contiene il valore di death dell'ultimo intervallo assegnato a quel livello
    assigned_intervals = []  # lista di tuple (birth, death, livello)
    
    for birth, death in intervals:
        assigned_level = None
        # Cerchiamo il livello l più basso per cui l'intervallo corrente non si sovrappone
        for l, last_death in enumerate(levels):
            if last_death < birth:
                assigned_level = l
                break
        if assigned_level is None:
            assigned_level = len(levels)
            levels.append(0)
        levels[assigned_level] = death
        assigned_intervals.append((birth, death, assigned_level + 1))  # usiamo 1-indexazione per il livello
    
    # Disegniamo il barcode con matplotlib
    fig, ax = plt.subplots()
    for birth, death, level in assigned_intervals:
        ax.hlines(level, birth, death, colors='blue', lw=2)
        ax.plot([birth, death], [level, level], 'bo')
    
    ax.set_xlabel("x (indice)")
    ax.set_ylabel("Quota (livello)")
    ax.set_title("Barcode del modulo di persistenza")
    # Impostiamo i limiti dell'asse x:
    # da un po' sotto lambda_min fino a lambda_min + n*h (o anche oltre, se necessario)
    ax.set_xlim(lambda_min, lambda_min + (n-1) * h)
    
    # Impostiamo i tick delle ascisse: ogni riga corrisponde a un tick
    xticks = [lambda_min + i * h for i in range(n)]
    ax.set_xticks(xticks)
    ax.set_xticklabels([f"{x:.2f}" for x in xticks])
    max_level = max((lev for (_, _, lev) in assigned_intervals), default=1)
    ax.set_ylim(0, max_level + 1)

    plt.grid(axis='x', linestyle='--', alpha=0.6)
    plt.show()

def main():
    # Se viene passato un argomento, lo interpretiamo come una stringa JSON contenente la matrice
    if len(sys.argv) > 1:
        matrix_str = sys.argv[1]
        try:
            matrix = json.loads(matrix_str)
        except json.JSONDecodeError as e:
            print("Errore nella decodifica della matrice:", e)
            sys.exit(1)
        lambda_min = float(sys.argv[2])  # Valore minimo della scala delle ascisse
        h = float(sys.argv[3])  # Passo della scala delle ascisse
    else:
        # Matrice di default (se non viene passato alcun argomento)
        matrix = [
            [0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0],
            [0, 0, 0, 1, 1],
            [0, 0, 0, 0, 0],
            [0, 0, 1, 1, 0]
        ]
    plot_barcode(matrix, lambda_min, h)

if __name__ == '__main__':
    main()
