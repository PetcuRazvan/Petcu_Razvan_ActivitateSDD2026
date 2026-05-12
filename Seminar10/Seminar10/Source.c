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


void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	Nod* nodNou = malloc(sizeof(Nod));
	nodNou->dreapta = NULL;
	nodNou->stanga = NULL;
	nodNou->masina = masinaNoua;

	if (*radacina == NULL) {
		*radacina = nodNou;

		return;
	}

	Nod* aux = *radacina;
	while (aux->stanga != NULL || aux->dreapta != NULL) {
		if (masinaNoua.id < aux->masina.id) {
			if (aux->stanga != NULL) {
				aux = aux->stanga;
			}
			else {
				aux->stanga = nodNou;
				return;
			}
		}

		if (masinaNoua.id > aux->masina.id) {
			if (aux->dreapta != NULL) {
				aux = aux->dreapta;
			}
			else {
				aux->dreapta = nodNou;
				return;
			}
		}
	}
}

void adaugaMasinaInArboreRecursiv(Nod** radacina, Masina masinaNoua) {
	if (*radacina == NULL) {
		Nod* nodNou = malloc(sizeof(Nod));
		nodNou->dreapta = NULL;
		nodNou->stanga = NULL;
		nodNou->masina = masinaNoua;

		*radacina = nodNou;
	}
	else {
		if (masinaNoua.id < (*radacina)->masina.id) {
			adaugaMasinaInArboreRecursiv((&(*radacina)->stanga), masinaNoua);
		}
		else {
			adaugaMasinaInArboreRecursiv((&(*radacina)->dreapta), masinaNoua);
		}
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;

	while (!feof(file)) {
		adaugaMasinaInArboreRecursiv(&radacina, citireMasinaDinFisier(file));
	}
	fclose(file);

	return radacina;
}

void afisareMasiniDinArboreInordine(Nod* radacina) {
	//SRD  inordine
	//RSD  ordine
	//SDR  postordine

	if (radacina != NULL) {   //inordine
		afisareMasiniDinArboreInordine(radacina->stanga);     //S
		afisareMasina(radacina->masina);                      //R
		afisareMasiniDinArboreInordine(radacina->dreapta);    //D
	}
}

void afisareMasiniDinArborePostOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareMasiniDinArborePostOrdine(radacina->stanga);   //S
		afisareMasiniDinArborePostOrdine(radacina->dreapta);  //D
		afisareMasina(radacina->masina);                      //R
	}
}

void dezalocareArboreDeMasini(Nod** radacina) {
	//sunt dezalocate toate masinile si arborele de elemente
	if (*radacina != NULL) {
		Nod* stanga = (*radacina)->stanga;
		Nod* dreapta = (*radacina)->dreapta;

		free((*radacina)->masina.model);
		free((*radacina)->masina.numeSofer);
		free(*radacina);
		*radacina = NULL;

		dezalocareArboreDeMasini(&stanga);
		dezalocareArboreDeMasini(&dreapta);
	}
}

Masina getMasinaByID(Nod* radacina, int id) {
	Masina m;

	if (radacina == NULL) {
		m.id = -1;
		return m;
	}

	if (radacina->masina.id == id) {
		m = radacina->masina;

		m.model = malloc((strlen(radacina->masina.model) + 1) * sizeof(char));
		strcpy(m.model, radacina->masina.model);

		m.numeSofer = malloc((strlen(radacina->masina.numeSofer) + 1) * sizeof(char));
		strcpy(m.numeSofer, radacina->masina.numeSofer);

		return m;
	}

	if (id < radacina->masina.id) {
		return getMasinaByID(radacina->stanga, id);
	}
	else {
		return getMasinaByID(radacina->dreapta, id);
	}
}

int determinaNumarNoduri(Nod* radacina) {
	//calculeaza numarul total de noduri din arborele binar de cautare
	if (radacina != NULL) {
		int nrNoduriStanga = determinaNumarNoduri(radacina->stanga);
		int nrNoduriDreapta = determinaNumarNoduri(radacina->dreapta);

		return 1 + nrNoduriDreapta + nrNoduriStanga;
	}

	return 0;
}


int calculeazaInaltimeArbore(Nod* radacina) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (radacina != NULL) {
		return 1 + max(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta));
	}

	return 0;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina != NULL) {
		return radacina->masina.pret + calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta);
	}

	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina != NULL) {
		if (strcmp(radacina->masina.numeSofer, numeSofer)) {
			return radacina->masina.pret + calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);
		}
	}

	return 0;
}

int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArboreInordine(arbore);

	Masina masina = getMasinaByID(arbore, 5);
	afisareMasina(masina);

	printf("%d\n", determinaNumarNoduri(arbore));

	//dezalocareArboreDeMasini(&arbore);

	printf("%d\n", calculeazaInaltimeArbore(arbore));


	printf("%.2f\n", calculeazaPretTotal(arbore));

	return 0;
}