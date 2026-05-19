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
	struct NodArbore* stanga;
	struct NodArbore* dreapta;
};
typedef struct Nod Nod;

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

int calculeazaNivel(Nod* root) {
	if (root != NULL) {
		return 1 + max(calculeazaNivel(root->stanga), calculeazaNivel(root->dreapta));
	}

	return 0;
}

int gradEchilibru(Nod* root) {
	if (root != NULL) {
		return calculeazaNivel(root->stanga) - calculeazaNivel(root->dreapta);
	}
	
	return 0;
}

void rotireStanga(Nod** root) {
	Nod* aux = (*root)->dreapta;
	(*root)->dreapta = aux->stanga;
	aux->stanga = *root;
	*root = aux;
}

void rotireDreapta(Nod** root) {
	Nod* aux = (*root)->stanga;
	(*root)->stanga = aux->dreapta;
	aux->dreapta = (*root);
	*root = aux;
}

void adaugareinAVL(Nod** root, Masina masina) {
	if (*root != NULL) {
		if (masina.id < (*root)->masina.id) {
			adaugareinAVL(&(*root)->stanga, masina);
		}
		else {
			adaugareinAVL(&(*root)->dreapta, masina);
		}

		int grad = gradEchilibru(*root);
		if (grad == 2) {//mai mult in stanga
			if (gradEchilibru((*root)->stanga) == -1) {
				rotireStanga(&(*root)->stanga);
			}
			rotireDreapta(root);
		}
		if (grad == -2) {//mai mult in dreapta
			if (gradEchilibru((*root)->dreapta) == 1) {
				rotireDreapta(&(*root)->dreapta);
			}
			rotireStanga(root);
		}
	}
	else {
		Nod* nodNou = malloc(sizeof(Nod));
		nodNou->masina = masina;
		nodNou->stanga = NULL;
		nodNou->dreapta = NULL;

		*root = nodNou;
	}
}

Nod* citireAVLDinFisier(const char* numeFisier) {
	Nod* root = NULL;

	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		adaugareinAVL(&root, citireMasinaDinFisier(file));
	}
	fclose(file);

	return root;
}

void afisareInordine(Nod* root) {
	if (root != NULL) {
		afisareInordine(root->stanga);
		afisareMasina(root->masina);
		afisareInordine(root->dreapta);
	}
}

int main() {
	Nod* arbore = citireAVLDinFisier("masini.txt");
	afisareInordine(arbore);
	printf("inaltime %d", calculeazaNivel(arbore));
}