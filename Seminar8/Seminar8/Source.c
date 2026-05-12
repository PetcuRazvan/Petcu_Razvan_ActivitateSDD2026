#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru Heap
//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap {
	int lungime;
	int nrMasini;

	Masina* masini;
};
typedef struct Heap Heap;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

Heap initializareHeap(int lungime) {
	Heap heap;

	heap.lungime = lungime;
	heap.nrMasini = 0;
	heap.masini = malloc(sizeof(Masina) * heap.nrMasini);

	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	if (pozitieNod < 0 || pozitieNod >= heap.nrMasini) {
		return;
	}
	
	int pozFiuStanga = 2 * pozitieNod + 1;
	int pozFiuDreapta = 2 * pozitieNod + 2;

	int pozMaxim = pozitieNod;
	if (pozFiuStanga < heap.nrMasini && heap.masini[pozFiuStanga].pret > heap.masini[pozMaxim].pret) {
		pozMaxim = pozFiuStanga;
	}

	if (pozFiuDreapta < heap.nrMasini && heap.masini[pozFiuDreapta].pret > heap.masini[pozMaxim].pret) {
		pozMaxim = pozFiuDreapta;
	}

	if (pozMaxim != pozitieNod) {
		Masina aux = heap.masini[pozitieNod];
		heap.masini[pozitieNod] = heap.masini[pozMaxim];
		heap.masini[pozMaxim] = aux;

		if (pozMaxim < (heap.nrMasini - 2) / 2) {  //reapelam functia daca nodul maxim nu este frunza, si trebuie sa mergem sa mai verificam o data
			filtreazaHeap(heap, pozMaxim);
		}
	}

}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(12); // o lungime asa mai mare
	heap.nrMasini = 0;

	while (!feof(file)) {
		heap.masini[heap.nrMasini++] = citireMasinaDinFisier(file);
	}
	fclose(file);
	heap.lungime = heap.nrMasini;

	for (int i = (heap.nrMasini - 2) / 2; i >= 0; i--) { //incepem direct cu ultimul nod care este parinte
		filtreazaHeap(heap, i);
	}

	return heap;
}

void afisareHeap(Heap heap) {
	//afiseaza elementele vizibile din heap
	for (int i = 0; i < heap.nrMasini; i++) {
		afisareMasina(heap.masini[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	//afiseaza elementele ascunse din heap
	for (int i = heap.nrMasini; i < heap.lungime; i++) {
		afisareMasina(heap.masini[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	//extrage si returneaza masina de pe prima pozitie
	//elementul extras nu il stergem...doar il ascundem
	if (heap->nrMasini <= 0) {
		Masina masinaProasta;
		masinaProasta.id = -1;

		return masinaProasta;
	}

	Masina aux = heap->masini[0];
	heap->masini[0] = heap->masini[heap->nrMasini - 1];
	heap->masini[heap->nrMasini - 1] = aux;

	heap->nrMasini--;

	for (int i = (heap->nrMasini - 2) / 2; i >= 0; i--) {
		filtreazaHeap(*heap, i);
	}

	return heap->masini[heap->nrMasini];
}


void dezalocareHeap(Heap* heap) {
	//sterge toate elementele din Heap
}

int main() {
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");

	printf("\n==== EXRAGERE ====\n");
	afisareMasina(extrageMasina(&heap));
	afisareMasina(extrageMasina(&heap));

	printf("\n==== AFISARE MASINI ====\n");
	afisareHeap(heap);

	printf("\n==== AFISARE MASINI ASCUNSE ====\n");
	afiseazaHeapAscuns(heap);

	return 0;
}