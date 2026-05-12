#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Comanda {
	int cod;
	char* numeClient;
	int nrProduse;
	float sumaDePlata;
};
typedef struct Comanda Comanda;

struct NodDublu {
	Comanda comanda;
	struct NodDublu* next;
	struct NodDublu* prev;
};
typedef struct NodDublu NodDublu;

struct NodSimplu {
	Comanda comanda;
	struct NodSimplu* next;
};
typedef struct NodSimplu NodSimplu;

struct Stiva {
	NodSimplu* inceput;
	int lungime;
};
typedef struct Stiva Stiva;

struct Coada {
	NodDublu* inceput;
	NodDublu* sfarsit;
	int lungime;
};
typedef struct Coada Coada;

void afisareComanda(Comanda comanda) {
	printf("\nCOD: %d\n", comanda.cod);
	printf("NUME CLIENT: %s\n", comanda.numeClient);
	printf("NR PRODUSE: %d\n", comanda.nrProduse);
	printf("SUMA DE PLATA: %.2f\n", comanda.sumaDePlata);
}

Comanda citireComanda(FILE* file) {
	char buffer[100];
	fgets(buffer, 100, file);
	Comanda comanda;
	char* aux;
	char sep[3] = ",\n";

	aux = strtok(buffer, sep);
	comanda.cod = atoi(aux);

	aux = strtok(NULL, sep);
	comanda.numeClient = malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(comanda.numeClient, aux);

	aux = strtok(NULL, sep);
	comanda.nrProduse = atoi(aux);

	aux = strtok(NULL, sep);
	comanda.sumaDePlata = atof(aux);

	return comanda;
}

Coada initializareCoada() {
	Coada coada;
	coada.inceput = NULL;
	coada.sfarsit = NULL;
	coada.lungime = 0;

	return coada;
}

Stiva initializareStiva() {
	Stiva stiva;
	stiva.inceput = NULL;
	stiva.lungime = 0;

	return stiva;
}

void pushStiva(Stiva* stiva, Comanda comanda) {
	stiva->lungime++;
	NodSimplu* nodNou = malloc(sizeof(NodSimplu));
	nodNou->comanda = comanda;

	if (stiva->inceput == NULL) {
		stiva->inceput = nodNou;
		nodNou->next = NULL;
	}
	else {
		nodNou->next = stiva->inceput;
		stiva->inceput = nodNou;
	}
}

Comanda popStiva(Stiva* stiva) {
	Comanda comandaGasita;

	if (stiva->inceput == NULL) {
		comandaGasita.cod = -1;
		return comandaGasita;
	}

	stiva->lungime--;
	comandaGasita = stiva->inceput->comanda;
	stiva->inceput = stiva->inceput->next;

	return comandaGasita;
}

void push(Coada* coada, Comanda comanda) {  //pushez la inceput
	coada->lungime++;
	NodDublu* nodNou = malloc(sizeof(NodDublu));
	nodNou->comanda = comanda;
	nodNou->next = coada->inceput;
	nodNou->prev = NULL;
	
	if (coada->inceput == NULL) {
		coada->inceput = nodNou;
		coada->sfarsit = nodNou;

		return;
	}

	coada->inceput->prev = nodNou;
	coada->inceput = nodNou;
}

Comanda pop(Coada* coada) {
	Comanda comandaGasita;
	
	if (coada->sfarsit == NULL) {
		comandaGasita.cod = -1;

		return comandaGasita;
	}

	comandaGasita = coada->sfarsit->comanda;

	if (coada->inceput == coada->sfarsit) {
		free(coada->sfarsit);

		coada->sfarsit = NULL;
		coada->inceput = NULL;
	}
	else {
		NodDublu* victima = coada->sfarsit;
		coada->sfarsit = coada->sfarsit->prev;

		free(victima);
	}

	coada->lungime--;
	return comandaGasita;
}

Coada citesteComenziDinFisier(const char* numeFisier) {
	Coada coada = initializareCoada();

	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		push(&coada, citireComanda(file));
	}
	fclose(file);

	return coada;
}

Stiva citesteStivaDinFisier(const char* numeFisier) {
	Stiva stiva = initializareStiva();

	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		pushStiva(&stiva, citireComanda(file));
	}
	fclose(file);

	return stiva;
}

void afisareStiva(Stiva* stiva) {
	Stiva aux = initializareStiva();

	while (stiva->inceput != NULL) {
		Comanda comanda = popStiva(stiva);
		pushStiva(&aux, comanda);

		afisareComanda(comanda);
	}

	while (aux.inceput != NULL) {
		pushStiva(stiva, popStiva(&aux));
	}
}

void afisareCoada(Coada* coada) {
	Coada aux = initializareCoada();

	while (coada->inceput != NULL) {
		Comanda comanda = pop(coada);
		push(&aux, comanda);
		afisareComanda(comanda);
	}

	while (aux.inceput != NULL) {
		Comanda comanda = pop(&aux);
		push(coada, comanda);
	}
}

Comanda* getComenziPretMare(Coada* coada, int* nr) {
	int lungime = coada->lungime;
	Comanda* vector = malloc(lungime * sizeof(Comanda));
	*nr = 0;

	Coada aux = initializareCoada();
	while (coada->inceput != NULL) {
		Comanda comanda = pop(coada);
		push(&aux, comanda);
		
		if (comanda.sumaDePlata > 100) {
			vector[(*nr)++] = comanda;
		}
	}

	while (aux.inceput != NULL) {
		push(coada, pop(&aux));
	}

	Comanda* vectorReturnat = malloc(*nr * sizeof(Comanda));
	for (int i = 0; i < *nr; i++) {
		vectorReturnat[i] = vector[i];
	}
	free(vector);

	for (int i = 0; i < (*nr) - 1; i++) {
		for (int j = i + 1; j < *nr; j++) {
			if (vectorReturnat[i].nrProduse < vectorReturnat[j].nrProduse) {
				Comanda comandaAux = vectorReturnat[i];
				vectorReturnat[i] = vectorReturnat[j];
				vectorReturnat[j] = comandaAux;
			}
		}
	}

	return vectorReturnat;
}

int main() {
	Stiva stiva = citesteStivaDinFisier("comenzi.txt");
	afisareStiva(&stiva);
	afisareStiva(&stiva);
	
	return 0;
}