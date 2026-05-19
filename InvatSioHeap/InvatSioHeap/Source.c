#define CRT_SEURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Heap {
	int lungime;
	int nrMasini;
	int* numere;
};
typedef struct Heap Heap;

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrMasini = 0;
	heap.numere = malloc(sizeof(int) * lungime);

	return heap;
}

int citesteNumarDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);

	char* aux = strtok(buffer, sep);
	return atoi(aux);
}

void filtrareHeap(Heap heap, int pozitieNod) {
	if (pozitieNod < 0 || pozitieNod >= heap.nrMasini) {
		return;
	}

	int pozFiuStanga = (pozitieNod * 2) + 1;
	int pozFiuDreapta = (pozitieNod * 2) + 2;

	int pozitieMax = pozitieNod;
	if (pozFiuStanga < heap.nrMasini && heap.numere[pozFiuStanga] > heap.numere[pozitieMax]) {
		pozitieMax = pozFiuStanga;
	}
	if (pozFiuDreapta < heap.nrMasini && heap.numere[pozFiuDreapta] > heap.numere[pozitieMax]) {
		pozitieMax = pozFiuDreapta;
	}

	if (pozitieMax != pozitieNod) {
		int aux = heap.numere[pozitieMax];
		heap.numere[pozitieMax] = heap.numere[pozitieNod];
		heap.numere[pozitieNod] = aux;

		if (pozitieMax < (heap.nrMasini - 2) / 2) {
			filtrareHeap(heap, pozitieMax);
		}
	}
}

Heap citireHeapDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");

	Heap heap = initializareHeap(10);
	int nr = 0;
	while (!feof(file)) {
		heap.numere[nr++] = citesteNumarDinFisier(file);
	}
	fclose(file);
	heap.nrMasini = nr;

	for (int i = (nr - 2) / 2; i >= 0; i--) {
		filtrareHeap(heap, i);
	}

	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrMasini; i++) {
		printf("%d\n", heap.numere[i]);
	}
}

int popHeap(Heap* heap) {
	if (heap->nrMasini == 0) {
		return NULL;
	}

	int aux = heap->numere[0];
	heap->numere[0] = heap->numere[heap->nrMasini - 1];
	heap->numere[heap->nrMasini - 1] = aux;
	heap->nrMasini--;

	filtrareHeap(*heap, 0);

	return heap->numere[heap->nrMasini];
}

int main() {
	Heap heap = citireHeapDinFisier("numere.txt");
	afisareHeap(heap);

	printf("popppp %d\n", popHeap(&heap));
	afisareHeap(heap);

	return 0;
}