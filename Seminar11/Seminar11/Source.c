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
struct NodArbore {
	Masina masina;
	struct NodArbore* stanga;
	struct NodArbore* dreapta;
};
typedef struct NodArbore NodArbore;

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

int calculeazaInaltimeArbore(NodArbore* root) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza

	if (root) {
		return max(calculeazaInaltimeArbore(root->stanga), calculeazaInaltimeArbore(root->dreapta)) + 1;
	}

	return 0;
}

//ALTE FUNCTII NECESARE:
// - aici veti adauga noile functii de care aveti nevoie.

char calculeazaGradEchilibru(NodArbore* root) {
	if (root) {
		return calculeazaInaltimeArbore(root->stanga) - calculeazaInaltimeArbore(root->dreapta);
	}
	else return 0;
}

void rotireStanga(NodArbore** root) {
	NodArbore* aux = (*root)->dreapta;
	(*root)->dreapta = aux->stanga;
	aux->stanga = (*root);
	(*root) = aux;
}

void rotireDreapta(NodArbore** root) {
	NodArbore* aux = (*root)->stanga;
	(*root)->stanga = aux->dreapta;
	aux->dreapta = (*root);
	(*root) = aux;
}

void adaugaMasinaInArboreEchilibrat(NodArbore** root, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte principiile de arbore binar de cautare ECHILIBRAT
	//dupa o anumita cheie pe care o decideti - poate fi ID
	if (*root) {
		if (masinaNoua.id < (*root)->masina.id) {
			adaugaMasinaInArboreEchilibrat(&((*root)->stanga), masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&((*root)->dreapta), masinaNoua);
		}

		int gradEchilibru = calculeazaGradEchilibru(*root);
		if (gradEchilibru == 2) {  //dezechilibrat in partea stanga
			if (calculeazaGradEchilibru((*root)->stanga == -1)) {   //cazul mai simplu
				rotireStanga(&(*root)->stanga);
			}
			rotireDreapta(root);
		}
		if (gradEchilibru == -2) {
			if (calculeazaGradEchilibru((*root)->dreapta) == 1) {
				rotireDreapta(&(*root)->dreapta);
			}
			rotireStanga(root);
		}
	}
	else {
		NodArbore* nodNou = malloc(sizeof(NodArbore));
		nodNou->dreapta = NULL;
		nodNou->stanga = NULL;
		nodNou->masina = masinaNoua;

		(*root) = nodNou;
	}
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	NodArbore* root = NULL;

	while (!feof(file)) {
		Masina masina = citireMasinaDinFisier(file);
		adaugaMasinaInArboreEchilibrat(&root, masina);
	}
	fclose(file);

	return root;
}

void afisareMasiniDinArbore(NodArbore* root) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
	if (root) {
		afisareMasina(root->masina);
		afisareMasiniDinArbore(root->stanga);
		afisareMasiniDinArbore(root->dreapta);
	}
}

void dezalocareArboreDeMasini(NodArbore** root) {
	//sunt dezalocate toate masinile si arborele de elemente
	if (*root) {
		dezalocareArboreDeMasini(&(*root)->stanga);
		dezalocareArboreDeMasini(&(*root)->dreapta);
		
		free((*root)->masina.model);
		free((*root)->masina.numeSofer);
		free(*root);

		*root = NULL;
	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(NodArbore* arbore, int id) {
	if (arbore) {
		if (arbore -> masina.id == id) {
			return arbore->masina;
		}

		if (id < arbore->masina.id) {
			return getMasinaByID(arbore->stanga, id);
		}
		else {
			return getMasinaByID(arbore->dreapta, id);
		}
	}
}

int determinaNumarNoduri(/*arborele de masini*/);

float calculeazaPretTotal(/*arbore de masini*/);

float calculeazaPretulMasinilorUnuiSofer(NodArbore* root, const char* numeSofer) {
	float sum = 0;
	if (root) {
		sum += calculeazaPretulMasinilorUnuiSofer(root->stanga, numeSofer);
		sum += calculeazaPretulMasinilorUnuiSofer(root->dreapta, numeSofer);

		if (strcmp(root->masina.numeSofer, numeSofer) == 0) {
			sum += root->masina.pret;
		}
	}

	return sum;
}

int main() {
	NodArbore* arbore = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(arbore);
	
	printf("\n%.2f", calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu"));

	return 0;
}