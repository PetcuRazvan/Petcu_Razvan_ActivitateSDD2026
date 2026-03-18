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
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

//creare structura pentru un nod dintr-o lista dublu inlantuita
struct Nod {
	Masina info;
	Nod* next;
	Nod* prev;
};

//creare structura pentru Lista Dubla
struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};

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

void afisareListaMasini(ListaDubla lista) {
	Nod* p = lista.prim;
	while (p != NULL) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void afisareInversaListaMasini(ListaDubla lista) {
	Nod* p = lista.ultim;
	while (p != NULL) {
		afisareMasina(p->info);
		p = p->prev;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) { //tre sa dam pointer ca poate modificm adresa de inceput sau de sfarsit
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua; //shallow copy
	nou->next = NULL;
	nou->prev = lista;

	if (lista->ultim != NULL) {
		lista->ultim->next = nou;
	}
	else {
		lista->prim = nou;
	}

	lista->ultim = nou;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua; //shallow copy
	nou->next = lista->prim;
	nou->prev = NULL;

	if (lista->prim != NULL) {
		lista->prim->prev = nou;
	}
	else {
		lista->ultim = nou;
	}

	lista->prim = nou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");

	if(f) {
		ListaDubla ld;
		ld.prim = ld.ultim = NULL;

		while (!feof(f)) {
			adaugaMasinaInLista(&ld, citireMasinaDinFisier(f));
		}

		fclose(f);
		return ld;
	}
}

void dezalocareLDMasini(ListaDubla* ld) {
	while (ld->prim != NULL && ld->prim->next != NULL) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		
		ld->prim = ld->prim->next;

		free(ld->prim->prev);
	}

	if (ld->prim != NULL) {
		free(ld->prim->info.numeSofer);
		free(ld->prim->info.model);
		free(ld->prim);
	}

	ld->prim = NULL;
	ld->ultim = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
	Nod* p = lista.prim;
	float pretMediu = 0;
	int nr = 0;

	while (p != NULL) {
		nr++;
		pretMediu += p->info.pret;

		p = p->next;
	}

	if (nr == 0) {
		return 0;
	}
	
	return pretMediu / nr;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	if (lista->prim != NULL && lista->prim->info.id == id) {
		free(lista->prim->info.numeSofer);
		free(lista->prim->info.model);
		lista->prim = lista->prim->next;

		if (lista->prim != NULL) {
			free(lista->prim->prev);
		}
		else {
			free(lista->ultim);
			lista->ultim = NULL;
		}
		return;
	}
	
	Nod* p = lista->prim;
	while (p != NULL && p->info.id != id) {
		p = p->next;
	}

	if (p != NULL) {
		if (p->prev != NULL) {
			p->prev->next = p->next;
		}
		
		if (p->next != NULL) {
			p->next->prev = p->prev;
		}
		else {
			p->prev->next = NULL;
			lista->ultim = p->prev;
		}

		free(p->info.numeSofer);
		free(p->info.model);
		free(p);
	}
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	Nod* p = lista.prim;
	float maxPret = -1;
	char* nume = NULL;

	while (p) {
		if (p->info.pret > maxPret) {
			maxPret = p->info.pret;
			nume = p->info.numeSofer;
		}
		p = p->next;
	}

	return nume;
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);

	/*printf("\n%f", calculeazaPretMediu(lista));
	printf("\n%s", getNumeSoferMasinaScumpa(lista));*/
	stergeMasinaDupaID(&lista, 1);
	printf("\nMasini dupa stergere\n");
	afisareListaMasini(lista);
	return 0;
}