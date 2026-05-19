#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

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
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

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

//1.
// structuri necesare
//dorim stocarea unui graf intr-o lista de liste
//astfel avem nod ListaPrincipala si NodListaSecundara
typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP {
	Masina masina;
	NodP* next;
	NodS* vecini;
};

struct NodS {
	NodP* info;
	NodS* next;
};

void inserareListaPrincipala(NodP** lista, Masina masina) {
	NodP* nodNou = malloc(sizeof(NodP));
	nodNou->next = NULL;
	nodNou->vecini = NULL;
	nodNou->masina = masina;

	if (*lista == NULL) {
		*lista = nodNou;
		return;
	}

	NodP* aux = *lista;
	while (aux->next != NULL) {
		aux = aux->next;
	}

	aux->next = nodNou;
}

void inserareListaSecundara(NodS** lista, NodP* vecin) {
	NodS* nodNou = malloc(sizeof(NodS));
	nodNou->next = NULL;
	nodNou->info = vecin;

	if (*lista == NULL) {
		*lista = nodNou;
		return;
	}

	NodS* aux = *lista;
	while (aux->next != NULL) {
		aux = aux->next;
	}

	aux->next = nodNou;
}

NodP* cautaNodDupaId(NodP* lista, int id) {
	while (lista != NULL) {
		if (lista->masina.id == id) {
			return lista;
		}

		lista = lista->next;
	}

	return NULL;
}

void inserareMuchie(NodP* lista, int idStart, int idSfarsit) {
	NodP* start = cautaNodDupaId(lista, idStart);
	NodP* sfarsit = cautaNodDupaId(lista, idSfarsit);

	if (start && sfarsit) {
		inserareListaSecundara(&start->vecini, sfarsit);
		inserareListaSecundara(&sfarsit->vecini, start);
	}
}

NodP* citireMasiniDinFisier(const char* numeFisier) {
	NodP* graf = NULL;

	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		inserareListaPrincipala(&graf, citireMasinaDinFisier(file));
	}
	fclose(file);

	return graf;
}

void citireMuchiiDinFisier(NodP* lista, const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");

	while (!feof(file)) {
		int idStart = 0;
		int idStop = 0;

		fscanf(file, "%d %d", &idStart, &idStop);
		inserareMuchie(lista, idStart, idStop);
	}
	fclose(file);
}

void afisareMasini(NodP* graf) {
	while (graf != NULL) {
		afisareMasina(graf->masina);
		graf = graf->next;
	}
}

void afisareVecini(NodP* graf, int id) {
	NodP* nod = cautaNodDupaId(graf, id);

	NodS* aux = nod->vecini;
	while (aux != NULL) {
		afisareMasina(aux->info->masina);
		aux = aux->next;
	}
}

int main() {
	NodP* graf = citireMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier(graf, "muchii.txt");
	//afisareMasini(graf);
	afisareVecini(graf, 8);
	
	return 0;
}