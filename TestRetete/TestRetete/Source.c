#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Serviciu {
	unsigned int idServiciu;
	char* numeClient;
	float sumaDePlata;
	unsigned int timpMinute;
};
typedef struct Serviciu Serviciu;

struct Nod {
	Serviciu serviciu;
	struct Nod* next;
};
typedef struct Nod Nod;

struct HashTable {
	int dimensiune;
	Nod** liste;
};
typedef struct HashTable HashTable;

Serviciu citireServiciuDinfisier(FILE* file) {
	Serviciu serviciu;
	char buffer[100];

	fgets(buffer, 100, file);

	char sep[3] = ",\n";
	char* aux;

	aux = strtok(buffer, sep);
	serviciu.idServiciu = atoi(aux);

	aux = strtok(NULL, sep);
	serviciu.numeClient = malloc(sizeof(char) * (strlen(aux) + 1));
	strcpy(serviciu.numeClient, aux);

	aux = strtok(NULL, sep);
	serviciu.sumaDePlata = atof(aux);

	aux = strtok(NULL, sep);
	serviciu.timpMinute = atoi(aux);

	return serviciu;
}

void afisareServiciu(Serviciu serviciu) {
	printf("ID: %d\n", serviciu.idServiciu);
	printf("NUME CLIENT: %s\n", serviciu.numeClient);
	printf("SUMA DE PLATA: %.2f\n", serviciu.sumaDePlata);
	printf("TIMP MINUTE: %d\n", serviciu.timpMinute);
}

HashTable initializareHashTable(int dimensiune) {
	HashTable hashTable;

	hashTable.dimensiune = dimensiune;
	hashTable.liste = malloc(dimensiune * sizeof(Nod*));
	for (int i = 0; i < dimensiune; i++) {
		hashTable.liste[i] = NULL;
	}

	return hashTable;
}

void adaugareNod(Nod** lista, Serviciu serviciu) {
	Nod* nodNou = malloc(sizeof(Nod));
	nodNou->serviciu = serviciu;
	nodNou->next = NULL;
	
	if (*lista == NULL) {
		*lista = nodNou;

		return;
	}

	Nod* aux = *lista;
	while (aux != NULL && aux->next != NULL) {
		aux = aux->next;
	}

	aux->next = nodNou;
}

int getHash(int lungime, char* numeClient) {
	int sum = 0;
	
	for (int i = 0; numeClient[i] != '\0'; i++) {
		sum += (unsigned char)numeClient[i];
	}

	return (sum * 17) % lungime;
}

void adaugaServiciuInhash(HashTable hashTable, Serviciu serviciu) {
	int hash = getHash(hashTable.dimensiune, serviciu.numeClient);

	adaugareNod(&(hashTable.liste[hash]), serviciu);
}

HashTable umpleHashTableDinFisier(const char* numeFisier) {
	HashTable hashTable = initializareHashTable(7);

	FILE* file = fopen(numeFisier, "r");

	while (!feof(file)) {
		adaugaServiciuInhash(hashTable, citireServiciuDinfisier(file));
	}
	fclose(file);

	return hashTable;
}

void afisareLista(Nod* lista) {
	Nod* aux = lista;

	while (aux != NULL) {
		afisareServiciu(aux->serviciu);

		aux = aux->next;
	}
}

void afisareHashPeclustere(HashTable hashTable) {
	int nr = 1;

	for (int i = 0; i < hashTable.dimensiune; i++) {
		if (hashTable.liste[i] != NULL) {
			printf("\n====CLUSTERUL %d====\n", nr++);

			afisareLista(hashTable.liste[i]);
		}
	}
}

int nrserviciiClient(HashTable hashTable, char* numeClient) {
	int nr = 0;
	
	for (int i = 0; i < hashTable.dimensiune; i++) {
		if (hashTable.liste[i] != NULL) {
			Nod* aux = hashTable.liste[i];

			while (aux != NULL) {
				if (strcmp(aux->serviciu.numeClient, numeClient) == 0) {
					nr++;
				}

				aux = aux->next;
			}
		}
	}

	return nr;
}

void stergereUltimNod(Nod** lista) {
	if ((*lista) == NULL) {
		return;
	}

	if ((*lista)->next == NULL) {
		free((*lista)->serviciu.numeClient);
		free(*lista);

		*lista = NULL;
		return;
	}

	Nod* aux = *lista;
	while (aux->next->next != NULL) {
		aux = aux->next;
	}

	free(aux->next->serviciu.numeClient);
	free(aux->next);
	aux->next = NULL;
}

void stergereDupaHash(HashTable hashTable, int hash) {
	stergereUltimNod(&(hashTable.liste[hash]));
}

Serviciu* getServiciiCuDurataMaiMare(HashTable hashTable, int* nr, unsigned int durataTimp) {
	Serviciu* servicii = malloc(hashTable.dimensiune * sizeof(Serviciu));
	*nr = 0;

	for (int i = 0; i < hashTable.dimensiune; i++) {
		if (hashTable.liste[i] != NULL) {
			Nod* aux = hashTable.liste[i];

			while (aux != NULL) {
				if (aux->serviciu.timpMinute >= durataTimp) {
					servicii[(*nr)++] = aux->serviciu;
				}

				aux = aux->next;
			}
		}
	}

	Serviciu* vectorReturnat = malloc(*nr * sizeof(Serviciu));
	for (int i = 0; i < *nr; i++) {
		vectorReturnat[i] = servicii[i];
	}
	free(servicii);

	return vectorReturnat;
}

int main() {
	HashTable hashTable = umpleHashTableDinFisier("servicii.txt");
	afisareHashPeclustere(hashTable);
	
	printf("\n ================================== \n");

	int nr;
	Serviciu* servicii = getServiciiCuDurataMaiMare(hashTable, &nr, 40);
	for (int i = 0; i < nr; i++) {
		afisareServiciu(servicii[i]);
	}

	return 0;
}