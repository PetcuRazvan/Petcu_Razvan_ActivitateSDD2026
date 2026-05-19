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

//creare structura pentru un nod dintr-un arbore binar de cautare
struct Nod {
	Masina masina;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	Masina masina;
	char* aux;

	fgets(buffer, 100, file);

	aux = strtok(buffer, sep);
	masina.id = atoi(aux);

	aux = strtok(NULL, sep);
	masina.nrUsi = atoi(aux);

	aux = strtok(NULL, sep);
	masina.pret = atof(aux);

	aux = strtok(NULL, sep);
	masina.model = malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.model, aux);

	aux = strtok(NULL, sep);
	masina.numeSofer = malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.numeSofer, aux);

	aux = strtok(NULL, sep);
	masina.serie = *aux;

	return masina;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Nod** radacina, Masina masina) {
	if (*radacina == NULL) {
		Nod* nodNou = malloc(sizeof(Nod));
		nodNou->masina = masina;
		nodNou->stanga = NULL;
		nodNou->dreapta = NULL;

		*radacina = nodNou;
	}
	else {
		if (masina.id < (*radacina)->masina.id) {
			adaugaMasinaInArbore(&(*radacina)->stanga, masina);
		}
		else {
			adaugaMasinaInArbore(&(*radacina)->dreapta, masina);
		}
	}
}

Nod* citireArboreDinFisier(const char* numeFisier) {
	Nod* arbore = NULL;

	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		adaugaMasinaInArbore(&arbore, citireMasinaDinFisier(file));
	}
	fclose(file);

	return arbore;
}

void afisareArboreInordine(Nod* arbore) {
	if (arbore == NULL) {
		return;
	}

	afisareArboreInordine(arbore->stanga);
	afisareMasina(arbore->masina);
	afisareArboreInordine(arbore->dreapta);
}

void afisareArboreOrdine(Nod* arbore) {
	if (arbore == NULL) {
		return;
	}

	afisareMasina(arbore->masina);
	afisareArboreOrdine(arbore->stanga);
	afisareArboreOrdine(arbore->dreapta);
}

void afisarePostOrdine(Nod* arbore) {
	if (arbore == NULL) {
		return NULL;
	}

	afisarePostOrdine(arbore->stanga);
	afisarePostOrdine(arbore->dreapta);
	afisareMasina(arbore->masina);
}

void dezalocareArbore(Nod** arbore) {
	if (*arbore != NULL) {
		Nod* stanga = (*arbore)->stanga;
		Nod* dreapta = (*arbore)->dreapta;

		free((*arbore)->masina.model);
		free((*arbore)->masina.numeSofer);
		free(*arbore);
		*arbore = NULL;

		dezalocareArbore(&stanga);
		dezalocareArbore(&dreapta);
	}
}

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore != NULL) {
		return 1 + max(calculeazaInaltimeArbore(arbore->stanga), calculeazaInaltimeArbore(arbore->dreapta));
	}

	return 0;
}

int calculeazaNrNoduri(Nod* arbore) {
	if (arbore != NULL) {
		return 1 + calculeazaNrNoduri(arbore->stanga) + calculeazaNrNoduri(arbore->dreapta);
	}

	return 0;
}

int main() {
	Nod* arbore = citireArboreDinFisier("masini.txt");
	/*afisareArboreInordine(arbore);
	printf("\n===============================\n");
	afisareArboreOrdine(arbore);
	printf("\n===============================\n");
	afisarePostOrdine(arbore);

	dezalocareArbore(&arbore);*/
	printf("inaltime: %d\n", calculeazaInaltimeArbore(arbore));
	printf("nr noduri: %d", calculeazaNrNoduri(arbore));

	return 0;
	//SRD  inordine
	//RSD  ordine
	//SDR  postordine

}