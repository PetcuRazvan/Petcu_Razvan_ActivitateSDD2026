#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct Joc {
	int id;
	int nrJucatori;
	char* denumire;
	float pret;
	char rating;
};

struct Joc initializare(int _id, int _nrMin, char* _denumire, float _pret, char _rating) {
	struct Joc s;
	s.id = 1;
	s.nrJucatori = _nrMin;
	s.denumire = malloc(strlen(_denumire) * (sizeof(char) + 1));
	strcpy(s.denumire, _denumire);
	s.pret = _pret;
	s.rating = _rating;
	return s;
}

void afisare(struct Joc j) {
	printf("Nume : %s\n", j.denumire);
	printf("id : %d\n", j.id);
	printf("numar minim jucatori : %d\n", j.nrJucatori);
	printf("rating : %c\n", j.rating);
	printf("pret : %5.2f\n", j.pret);
}

void dezalocare(struct Joc* s) {
	if (s->denumire != NULL) {
		free(s->denumire);
	}
}

void modificaAtribut(struct Joc* s, char* _denumire) {
	free((*s).denumire);
	(*s).denumire = malloc((strlen(_denumire) + 1) * sizeof(char));
	strcpy((*s).denumire, _denumire);
}

float calculPretIndividual(struct Joc* j) {
	return (*j).pret / (*j).nrJucatori;
}

int main() {
	struct Joc s;
	s = initializare(1, 2, "Monopoly", 120, 'A');
	afisare(s);
	modificaAtribut(&s, "Secret Hitler");
	afisare(s);

	dezalocare(&s);

	printf("\n %f\n", calculPretIndividual(&s));

	int nrJocuri = 3;
	struct Joc* jocuri;
	jocuri = malloc(nrJocuri * sizeof(struct Joc));

	jocuri[0] = initializare(2, 3, "Saboteour", 50, 'R');
	jocuri[1] = initializare(3, 2, "Makaoa", 20, 'R');
	jocuri[2] = initializare(4, 2, "Catan", 200, 'G');

	afisare(*(jocuri + 2));

	return 0;
}