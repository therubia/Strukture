/*. Definirati strukturu osoba(ime, prezime, godina rođenja) i napisati program koji :
A.dinamički dodaje novi element na početak liste,
B.ispisuje listu,
C.dinamički dodaje novi element na kraj liste,
D.pronalazi element u listi(po prezimenu),
E.briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50

// Struktura koja predstavlja jednu osobu u listi
typedef struct Person {
    char name[MAX];
    char surname[MAX];
    int birthYear;
    struct Person* next;  // pokazivač na sljedeću osobu u listi
} Person;

// Deklaracija funkcija
Person* addToStart(Person* head, char* name, char* surname, int birthYear);  // Dodaje osobu na početak liste
Person* addToEnd(Person* head, char* name, char* surname, int birthYear);  // Dodaje osobu na kraj liste
void printList(Person* head);  // Ispisuje sve osobe iz liste
Person* findBySurname(Person* head, char* surname);  // Traži osobu po prezimenu
Person* deleteBySurname(Person* head, char* surname);  // Briše osobu po prezimenu
void freeList(Person* head);  // Oslobađa svu memoriju zauzetu listom


int main() {
    Person* head = NULL;
    char name[MAX], surname[MAX];
    int birthYear;
    int numOfPpl;
    int choice;

    printf("Koliko osoba zelite unijeti u pocetnu listu? ");
    scanf("%d", &numOfPpl);

    // unos početnih članova liste
    for (int i = 0; i < numOfPpl; i++) {
        printf("\nUnesite podatke za osobu %d (ime, prezime, godina rodjenja): ", i + 1);
        scanf("%s %s %d", name, surname, &birthYear);
        head = addToEnd(head, name, surname, birthYear); // dodaje osobu na kraj liste da redoslijed ostane isti
    }

    printf("\nPocetna lista osoba:\n");
    printList(head);

    do {
        printf("1. Dodaj osobu na pocetak liste\n");
        printf("2. Dodaj osobu na kraj liste\n");
        printf("3. Ispisi listu osoba\n");
        printf("4. Pronadi osobu po prezimenu\n");
        printf("5. Obrisi osobu po prezimenu\n");
        printf("Odaberite opciju: ");
		scanf("%d", &choice);  //korisnik bira koju operaciju želi izvršiti

        switch (choice) {
        case 1:
            printf("Unesite ime, prezime i godinu rođenja: ");
            scanf("%s %s %d", name, surname, &birthYear);
            head = addToStart(head, name, surname, birthYear);
            printf("Osoba dodana na pocetak liste.\n");
            break;

        case 2:
            printf("Unesite ime, prezime i godinu rođenja: ");
            scanf("%s %s %d", name, surname, &birthYear);
            head = addToEnd(head, name, surname, birthYear);
            printf("Osoba dodana na kraj liste.\n");
            break;

        case 3:
            printf("\nTrenutna lista osoba:\n");
            printList(head);
            break;

        case 4:
            printf("Unesite prezime osobe koju zelite pronaci: ");
            scanf("%s", surname);
            {
                Person* found = findBySurname(head, surname);
                if (found)
                    printf("Pronadena osoba: %s %s (%d)\n", found->name, found->surname, found->birthYear);
                else
                    printf("Osoba s prezimenom %s nije pronadena.\n", surname);
            }
            break;

        case 5:
            printf("Unesite prezime osobe koju zelite obrisati: ");
            scanf("%s", surname);
            head = deleteBySurname(head, surname);
            break;

        default:
            printf("Nepostojeca opcija. Pokusajte ponovno.\n");
        }

    } while (choice != 0);

    freeList(head);  // oslobađanje memorije prije završetka
    return 0;
}


Person* addToStart(Person* head, char* name, char* surname, int birthYear) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return head;
    }

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;

    newPerson->next = head;  // nova osoba pokazuje na trenutni početak
    return newPerson;       // novi element postaje head
}

Person* addToEnd(Person* head, char* name, char* surname, int birthYear) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return head;
    }

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;  // zadnji element pokazuje na NULL

    // ako je lista prazna, novi element postaje head
    if (!head)
        return newPerson;

    // inače prolazimo do kraja i dodajemo novi element
    Person* current = head;
    while (current->next)
        current = current->next;

    current->next = newPerson;
    return head;
}

void printList(Person* head) {
    Person* current = head;

    if (!current) {
        printf("Lista je prazna.\n");
        return;
    }

    while (current) {
        printf("%s %s, %d\n", current->name, current->surname, current->birthYear);
        current = current->next;
    }
}

Person* findBySurname(Person* head, char* surname) {
    Person* current = head;
    while (current) {
        if (strcmp(current->surname, surname) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

Person* deleteBySurname(Person* head, char* surname) {
    if (!head) {
        printf("Lista je prazna.\n");
        return NULL;
    }

    // ako je prva osoba ta koju brišemo
    if (strcmp(head->surname, surname) == 0) {
        Person* temp = head->next;
        free(head);
        printf("Osoba '%s' obrisana.\n", surname);
        return temp;
    }

    // tražimo odgovarajuću osobu
    Person* current = head;
    while (current->next && strcmp(current->next->surname, surname) != 0)
        current = current->next;

    if (current->next) {
        Person* temp = current->next;
        current->next = current->next->next;
        free(temp);
        printf("Osoba '%s' obrisana.\n", surname);
    }
    else {
        printf("Osoba s prezimenom '%s' nije pronadena.\n", surname);
    }

    return head;
}

void freeList(Person* head) {
    Person* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
