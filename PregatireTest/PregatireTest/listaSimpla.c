#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Masina {
	int id;
	char* model;
	char* numeSofer;
	float pret;
	int nrUsi;
	unsigned char serie;
};


struct NodSimplu {
	struct Masina masina;
	struct NodSimplu* next;
};



struct Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);
	char* aux;

	struct Masina masina;

	aux = strtok(buffer, sep);
	masina.id = atoi(aux);

	masina.nrUsi = atoi(strtok(NULL, sep));
	
	masina.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	masina.model = malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.model, aux);

	aux = strtok(NULL, sep);
	masina.numeSofer = malloc(strlen(aux) + 1);
	strcpy(masina.numeSofer, aux);

	aux = strtok(NULL, sep);
	masina.serie = *aux;

	return masina;
}

void adaugareMasinaListaSimpla(struct NodSimplu** head, struct Masina masina) {
	struct NodSimplu* nodNou = (struct NodSimplu*)malloc(sizeof(struct NodSimplu));
	nodNou->next = NULL;
	nodNou->masina = masina;

	if (*head == NULL) {
		*head = nodNou;
		return;
	}

	struct NodSimplu* aux = *head;

	while (aux->next != NULL) {
		aux = aux->next;
	}

	aux->next = nodNou;
}

struct NodSimplu* citireListaSimplaDinFisier(const char* numeFisier) {
	struct NodSimplu* lista = NULL;

	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		struct Masina masina = citireMasinaDinFisier(file);
		adaugareMasinaListaSimpla(&lista, masina);
	}
	fclose(file);

	return lista;
}

void afisaremasina(struct Masina masina) {
	printf("\nID: %d\n", masina.id);
	printf("MODEL: %s\n", masina.model);
	printf("NUME SOFER: %s\n", masina.numeSofer);
	printf("NR USI: %d\n", masina.nrUsi);
	printf("PRET: %.2f\n", masina.pret);
	printf("SERIE: %c\n", masina.serie);
}

void afisareListaSimpla(struct NodSimplu* lista) {
	struct NodSimplu* aux = lista;

	while (aux != NULL) {
		afisaremasina(aux->masina);

		aux = aux->next;
	}
}

void dezalocareMasina(struct Masina* masina) {
	free(masina->model);
	free(masina->numeSofer);
}

void dezalocareMasiniDinLista(struct NodSimplu* lista) {
	if (lista == NULL) {
		return;
	}

	struct NodSimplu* aux = lista;

	while (aux != NULL) {
		struct NodSimplu* urmator = aux->next;

		dezalocareMasina(&(aux->masina));
		free(aux);

		aux = urmator;
	}
}

stergereNodSimpluDupaId(struct NodSimplu** lista, int id) {
	if (*lista == NULL) {
		return;
	}

	if ((*lista)->masina.id == id) {
		struct NodSimplu* victima = *lista;
		*lista = (*lista)->next;

		dezalocareMasina(&(victima->masina));
		free(victima);

		return;
	}

	struct NodSimplu* aux = *lista;
	struct NodSimplu* prev = NULL;
	while (aux != NULL) {
		if (aux->masina.id == id) {
			prev->next = aux->next;

			dezalocareMasina(&(aux->masina));
			free(aux);

			return;
		}
		
		prev = aux;
		aux = aux->next;
	}
}

void stergereDupaSerie(struct NodSimplu** lista, char serieCautata) {
	while ((*lista) != NULL && (*lista)->masina.serie == serieCautata) {
		struct NodSimplu* victima = (*lista);

		*lista = (*lista)->next;

		dezalocareMasina(&(victima->masina));
		free(victima);
	}

	struct NodSimplu* aux = *lista;
	while (aux != NULL && aux->next != NULL) {
		if (aux->next->masina.serie == serieCautata) {
			struct NodSimplu* victima = aux->next;

			aux->next = aux->next->next;

			dezalocareMasina(&(victima->masina));
			free(victima);
		}
		else {
			aux = aux->next;
		}
	}
}

int main() {
	struct NodSimplu* listaSimpla = citireListaSimplaDinFisier("masini.txt");
	stergereDupaSerie(&listaSimpla, 'A');
	afisareListaSimpla(listaSimpla);

	return 0;
}