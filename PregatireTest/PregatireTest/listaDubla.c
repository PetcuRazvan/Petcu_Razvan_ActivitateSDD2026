//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//struct Masina {
//	int id;
//	char* model;
//	char* numeSofer;
//	float pret;
//	int nrUsi;
//	unsigned char serie;
//};
//
//struct NodDublu {
//	struct Masina masina;
//	struct NodDublu* next;
//	struct NodDublu* prev;
//};
//
//struct ListaDubla {
//	struct NodDublu* start;
//	struct NodDublu* end;
//};
//
//typedef struct NodDublu NodDublu;
//typedef struct ListaDubla ListaDubla;
//typedef struct Masina Masina;
//
//struct Masina citireMasinaDinFisier(FILE* file) {
//	char buffer[100];
//	char sep[3] = ",\n";
//
//	fgets(buffer, 100, file);
//	char* aux;
//
//	struct Masina masina;
//
//	aux = strtok(buffer, sep);
//	masina.id = atoi(aux);
//
//	masina.nrUsi = atoi(strtok(NULL, sep));
//
//	masina.pret = atof(strtok(NULL, sep));
//
//	aux = strtok(NULL, sep);
//	masina.model = malloc((strlen(aux) + 1) * sizeof(char));
//	strcpy(masina.model, aux);
//
//	aux = strtok(NULL, sep);
//	masina.numeSofer = malloc(strlen(aux) + 1);
//	strcpy(masina.numeSofer, aux);
//
//	aux = strtok(NULL, sep);
//	masina.serie = *aux;
//
//	return masina;
//}
//
//void afisaremasina(struct Masina masina) {
//	printf("\nID: %d\n", masina.id);
//	printf("MODEL: %s\n", masina.model);
//	printf("NUME SOFER: %s\n", masina.numeSofer);
//	printf("NR USI: %d\n", masina.nrUsi);
//	printf("PRET: %.2f\n", masina.pret);
//	printf("SERIE: %c\n", masina.serie);
//}
//
//void dezalocareMasina(struct Masina* masina) {
//	free(masina->model);
//	free(masina->numeSofer);
//}
//
//void adaugareMasinaInListaDubla(ListaDubla** lista, Masina masina) {
//	NodDublu* nodNou = (NodDublu*)malloc(sizeof(NodDublu));
//	nodNou->masina = masina;
//	nodNou->next = NULL;
//	
//	if ((*lista)->start == NULL && (*lista)->end == NULL) {
//		(*lista)->start = nodNou;
//		(*lista)->end = nodNou;
//
//		nodNou->prev = NULL;
//	}
//	else {
//		(*lista)->end->next = nodNou;
//		nodNou->prev = (*lista)->end;
//
//		(*lista)->end = nodNou;
//	}
//}
//
//ListaDubla* citireListaDublaDinFisier(const char* numeFisier) {
//	ListaDubla* lista = (ListaDubla*)malloc(sizeof(ListaDubla));
//	lista->start = NULL;
//	lista->end = NULL;
//
//	FILE* file = fopen(numeFisier, "r");
//	while (!feof(file)) {
//		Masina masina = citireMasinaDinFisier(file);
//		adaugareMasinaInListaDubla(&lista, masina);
//	}
//	fclose(file);
//
//	return lista;
//}
//
//int main() {
//	ListaDubla* lista = citireListaDublaDinFisier("masini.txt");
//
//	return 0;
//}