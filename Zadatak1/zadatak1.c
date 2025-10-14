/*Napisati program koji prvo pročita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamički alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i učitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE 1024
#define MAX_SCORE 50
#define FILE_NOT_FOUND -1

typedef struct {
	char name[50];
	char surname[50];
	int points;
} stud;

int countStud(char*); //broji retke
int loadStud(char*, stud*); //ucitava podatke o studentima
int printStud(stud*, int); //ispisuje podatke o studentima
float relPoints(int);

int main() {
	int n = 0; //broj studenata

	char filename[100];
	printf("Upisite ime datoteke: ");
	scanf("%s", filename); // korisnik unosi ime datoteke iz koje zeli ucitati podatke


	n = countStud(filename);
	if (n == 0) printf("Datoteka je prazna.");
	else printf("Ima %d studenata.\n", n);

	stud* studs = (stud*)malloc(n * sizeof(stud)); //alociranje memorija za ucitavanje podataka o studentima

	if (studs == NULL) {
		printf("Allocation failed.\n"); //greska pri alokaciji
		return 1;
	}

	int loadStuds = loadStud(filename, studs); //ucitavanje podataka iz datoteke
	if (loadStuds == FILE_NOT_FOUND) {
		return -1;
	}

	printStud(studs, n);

	printf("\n");
	free(studs);

	return 0;
}

int countStud(char* file) {
	int n = 0;
	int c, lastChar = '\n'; //inicijaliziramo lastChar kako se ne bi povecao broj redaka u slucaju praznog fajla

	FILE* f = fopen(file, "r");

	//provjeravamo postoji li datoteka
	if (f == NULL) {
		printf("FILE NOT FOUND.\n");
		return FILE_NOT_FOUND;
	}

	//uvecavamo broj redaka za 1 svaki put kada predemo u novi red
	while ((c = fgetc(f)) != EOF) {
		if (c == '\n') n++;
		lastChar = c;
	}

	fclose(f); //zatvaranje datoteke nakon sto se izvrsi brojanje redaka

	//ako datoteka nije prazna i ne zavrsava novim redom, broj redaka uvecavamo za 1
	if (lastChar != '\n' && lastChar != EOF) n++;

	return n;
}

int loadStud(char* file, stud* s) {

	FILE* f = fopen(file, "r");
	if (f == NULL) {
		printf("FILE NOT FOUND.\n");
		return FILE_NOT_FOUND;
	}

	int i = 0;

	while (fscanf(f, "%s %s %d", s[i].name, s[i].surname, &s[i].points) == 3) i++; // čita ime, prezime i bodove dok god ih ima 

	fclose(f);
	return 0;
}

float relPoints(int n) {
	return (float)n / MAX_SCORE * 100;
}

int printStud(stud* s, int n) {
	for (int i = 0; i < n; i++)
		printf("%s %s %d %.2f%%\n", s[i].name, s[i].surname, s[i].points, relPoints(s[i].points));
	return 0;
}
