#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date
typedef struct StructuraMasina Masina;


struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
struct Nod {
	Masina masina;
	struct Nod* next;
};


//creare structura pentru un nod dintr-o lista simplu inlantuita

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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


void adaugaMasinaInLista(struct Nod** lista,  Masina masinaNoua) {
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->masina = masinaNoua; //shallow copy
	nou->next = NULL;

	if (*lista == NULL) {
		*lista = nou;
		return;
	}

	struct Nod* curent = *lista;
	while (curent->next) {
		curent = curent->next;
	}

	curent->next = nou;
}

void afisareListaMasini(struct Nod* lista) {
	if (lista == NULL)
		return;

	struct Nod* curent = lista;
	while (curent){
		afisareMasina(curent->masina);
		curent = curent->next;
	}
}

void adaugaLaInceputInLista(struct Nod** lista, Masina masinaNoua) {
	struct Nod* nodNou;
	nodNou->masina = masinaNoua;
	nodNou->next = *lista;
	*lista = nodNou;
}

void* citireListaMasiniDinFisier(const char* numeFisier) { //void* este pointer la orice, putem lasa asa sau sa modificam in Nod*
	FILE* file = fopen(numeFisier, "r");
	struct Nod* lista = NULL;

	while (!feof(file)) {
		Masina masina = citireMasinaDinFisier(file);
		adaugaMasinaInLista(&lista, masina);
	}
	fclose(file);

	return lista;
}

void dezalocareListaMasini(struct Nod** lista) {
	struct Nod* head = *lista;
	struct Nod* next = NULL;

	while (head != NULL) {
		next = head->next;
		free(head->masina.model);
		free(head->masina.numeSofer);
		free(head);
		head = next;
	}

	*lista = NULL;
}

float calculeazaPretMediu(struct Nod* lista) {
	float pretMediu = 0;
	int nr = 0;

	while (lista) {
		pretMediu += lista->masina.pret;
		nr++;

		lista = lista->next;
	}

	if(nr) {
		return pretMediu / nr;
	}else {
		return pretMediu;
	}
}

void stergeMasiniDinSeria(struct Nod** lista, char serieCautata) {
	struct Nod* curent = *lista;
	struct Nod* prev = NULL;
	struct Nod* next = curent->next;

	if (curent->masina.serie == serieCautata) {
		*lista = next;
	}

	prev = curent;
	curent = curent->next;
	next = curent->next;

	while (curent != NULL) {

		if (curent->masina.serie == serieCautata) {
			prev->next = next;

			free(curent->masina.model);
			free(curent->masina.numeSofer);
			free(curent);

			curent = next;
		}
		else {
			prev = curent;
			curent = next;
		}

		if (curent != NULL) {
			next = curent->next;
		}
		else {
			next = NULL;
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(struct Nod* lista, const char* numeSofer) {
	float pretMediu = 0;
	int nr = 0;

	while (lista != NULL) {
		if (strcmp(lista->masina.numeSofer, numeSofer) == 0) {
			nr++;
			pretMediu += lista->masina.pret;
		}
	}

	if (nr) {
		return pretMediu / nr;
	} else {
		return pretMediu;
	}
}

int main() {
	struct Nod* lista = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	/*printf("PretMediu: %.2f\n", calculeazaPretMediu(lista));
	dezalocareListaMasini(&lista);*/
	stergeMasiniDinSeria(&lista, 'I');
	printf("Am sters\n");
	afisareListaMasini(lista);


	return 0;
}