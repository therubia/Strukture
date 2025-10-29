/* Definirati strukturu osoba(ime, prezime, godina rodenja) i napisati program koji :
A.dinamicki dodaje novi element na pocetak liste,
B.ispisuje listu,
C.dinamicki dodaje novi element na kraj liste,
D.pronalazi element u listi(po prezimenu),
E.briše odredeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*//* Prethodnom zadatku dodati funkcije:
A. dinamicki dodaje novi element iza odredenog elementa,
B. dinamicki dodaje novi element ispred odredenog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. cita listu iz datoteke.*/#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50

// Struktura koja predstavlja jednu osobu u listi
typedef struct Person {
    char name[MAX];
    char surname[MAX];
    int birthYear;
    struct Person* next;  // pokazivac na sljedecu osobu u listi
} Person;

// Deklaracija funkcija
Person* addToStart(Person* head, char* name, char* surname, int birthYear);  // Dodaje osobu na pocetak liste
Person* addToEnd(Person* head, char* name, char* surname, int birthYear);  // Dodaje osobu na kraj liste
void printList(Person* head);  // Ispisuje sve osobe iz liste
Person* findBySurname(Person* head, char* surname);  // Traži osobu po prezimenu
Person* deleteBySurname(Person* head, char* surname);  // Briše osobu po prezimenu
void freeList(Person* head);  // Oslobada svu memoriju zauzetu listom
Person* addAfter(Person* head, char* name, char* surname, int birthYear); //Dodaje novu osobu iza zadane osobe
Person* addBefore(Person* head, char* name, char* surname, int birthYear); //Dodaje novu osobu ispred zadane osobe
Person* sortListBySurname(Person* head);  //Sortira listu po prezimenima osoba
void saveListToFile(Person* head, const char* filename);  //Upisuje lisu u datoteku
Person* readListFromFile(const char* filename);  //Cita listu iz datoteke



int main() {
    Person* head = NULL;
    char name[MAX], surname[MAX];
    int birthYear;
    int numOfPpl;
    int choice;
    char filename[MAX];

    printf("Koliko osoba zelite unijeti u pocetnu listu? ");
    scanf("%d", &numOfPpl);

    // unos pocetnih clanova liste
    for (int i = 0; i < numOfPpl; i++) {
        printf("\nUnesite podatke za osobu %d (ime, prezime, godina rodjenja): ", i + 1);
        if (scanf("%49s %49s %d", name, surname, &birthYear) != 3) {
            printf("Neispravan unos.\n");
            --i; // ponovi unos za taj indeks
            while (getchar() != '\n'); // isprazni stdin
            continue;
        }
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
        printf("6. Dodaj osobu iza zadane osobe\n");
        printf("7. Dodaj osobu ispred zadane osobe\n");
        printf("8. Sortiraj listu po prezimenima\n");
        printf("9. Spremi listu u datoteku\n");
        printf("10. Ucitaj listu iz datoteke (zamijeni trenutnu)\n");
        printf("0. Izlaz\n");
        printf("Odaberite opciju: ");
        scanf("%d", &choice);  //korisnik bira koju operaciju želi izvršiti

        switch (choice) {
        case 1:
            printf("Unesite ime, prezime i godinu rodenja: ");
            if (scanf("%49s %49s %d", name, surname, &birthYear) != 3) {
                printf("Neispravan unos.\n");
                while (getchar() != '\n');
                break;
            }
            head = addToStart(head, name, surname, birthYear);
            printf("Osoba dodana na pocetak liste.\n");
            break;

        case 2:
            printf("Unesite ime, prezime i godinu rodenja: ");
            if (scanf("%49s %49s %d", name, surname, &birthYear) != 3) {
                printf("Neispravan unos.\n");
                while (getchar() != '\n');
                break;
            }
            head = addToEnd(head, name, surname, birthYear);
            printf("Osoba dodana na kraj liste.\n");
            break;

        case 3:
            printf("\nTrenutna lista osoba:\n");
            printList(head);
            break;

        case 4:
            printf("Unesite prezime osobe koju zelite pronaci: ");
            if (scanf("%49s", surname) != 1) { printf("Neispravan unos.\n"); break;}
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
            if (scanf("%49s", surname) != 1) { printf("Neispravan unos.\n"); break; }
            head = deleteBySurname(head, surname);
            break;

        case 6:
            printf("Unesite ime prezime godinu rodenja nove osobe: ");
            if (scanf("%49s %49s %d", name, surname, &birthYear) != 3) { printf("Neispravan unos.\n"); break; }
            head = addAfter(head, name, surname, birthYear);
            break;

        case 7:
            printf("Unesite ime prezime godinu rodenja nove osobe: ");
            if (scanf("%49s %49s %d", name, surname, &birthYear) != 3) { printf("Neispravan unos.\n"); break; }
            head = addBefore(head, name, surname, birthYear);
            break;

        case 8:
            head = sortListBySurname(head);
            break;

        case 9:
            printf("Unesite ime datoteke za spremanje: ");
            if (scanf("%49s", filename) != 1) { printf("Neispravan unos.\n"); break; }
            saveListToFile(head, filename);
            break;

        case 10:
            printf("Unesite ime datoteke za ucitavanje: ");
            if (scanf("%49s", filename) != 1) {
                printf("Neispravan unos.\n");
                break;
            }

            Person* temp = readListFromFile(filename);
            if (temp) {
                freeList(head);
                head = temp;
                printf("Lista uspjesno ucitana iz datoteke '%s'.\n", filename);
            }
            else {
                printf("Greska pri ucitavanju liste iz datoteke '%s'.\n", filename);
            }
            break;

        case 0:
            freeList(head);  // oslobadanje memorije prije završetka
            printf("Izlaz.\n");
            return 0;

        default:
            printf("Nepostojeca opcija. Pokusajte ponovno.\n");
        }

    } while (choice);

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

    newPerson->next = head;  // nova osoba pokazuje na trenutni pocetak
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

    // inace prolazimo do kraja i dodajemo novi element
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

    // tražimo odgovarajucu osobu
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
    printf("Memorija uspjesno oslobodena.\n");

}

Person* addAfter(Person* head, char* name, char* surname, int birthYear) {

    printf("Unesite prezime osobe iza koje zelite unijeti zadanu osobu: ");
    char wanted[MAX];
    if (scanf("%49s", wanted) != 1) { printf("Neispravan unos.\n"); return head;}

    Person* target = findBySurname(head, wanted);
    if (target == NULL) {
        printf("\nOsoba s prezimenom '%s' nije pronadena!\n", wanted);
        return head;
    }

    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return head;
    }

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;

    newPerson->next = target->next;
    target->next = newPerson;

    printf("\nNova osoba dodana iza %s.\n", wanted);
    return head;
}

Person* addBefore(Person* head, char* name, char* surname, int birthYear) {
    char wanted[MAX];
    printf("Unesite prezime osobe ispred koje zelis dodati novu osobu: ");
    if (scanf("%49s", wanted) != 1) { printf("Neispravan unos.\n"); return head; }

    if (!head) {
        printf("Lista je prazna.\n");
        return head;
    } 

    if (strcmp(head->surname, wanted) == 0)
        return addToStart(head, name, surname, birthYear);  //ako je wanted prva osoba u listi

    Person* current = head;
    while (current->next && strcmp(current->next->surname, wanted) != 0)  //usporeduje s next-om pa se nova osoba upisuje ispred next-a (tj ispred zadane osobe)
        current = current->next;

    if (!current->next) {
        printf("Osoba s prezimenom '%s' nije pronadena.\n", wanted);
        return head;
    }

    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Greska pri alokaciji memorije!\n");
        return head;
    }

    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;

    newPerson->next = current->next;
    current->next = newPerson;

    printf("Nova osoba dodana ispred '%s'.\n", wanted);
    return head;
}

Person* sortListBySurname(Person* head) {
    if (!head || !head->next) return head;

    Person* i, * j;
    char tempName[MAX], tempSurname[MAX];
    int tempYear;

    for (i = head; i->next; i = i->next) {
        for (j = i->next; j; j = j->next) {
            if (strcmp(i->surname, j->surname) > 0) {  //usporeduje ASCII vrijednosti slovo po slovo
                // zamjena svih polja
                strcpy(tempName, i->name);
                strcpy(tempSurname, i->surname);
                tempYear = i->birthYear;

                strcpy(i->name, j->name);
                strcpy(i->surname, j->surname);
                i->birthYear = j->birthYear;

                strcpy(j->name, tempName);
                strcpy(j->surname, tempSurname);
                j->birthYear = tempYear;
            }
        }
    }
    printf("Lista sortirana po prezimenima.\n");
    return head;
}

void saveListToFile(Person* head, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Greska pri otvaranju datoteke!\n");
        return;
    }

    Person* current = head;
    while (current) {
        fprintf(f, "%s %s %d\n", current->name, current->surname, current->birthYear);  //unosi elemente u datoteku jednog po jednog
        current = current->next;
    }

    fclose(f);
    printf("Lista uspjesno spremljena u '%s'.\n", filename);
}

Person* readListFromFile(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Datoteka '%s' nije pronadena.\n", filename);
        return NULL;
    }

    Person* head = NULL;
    char name[MAX], surname[MAX];
    int year;

    while (fscanf(f, "%s %s %d", name, surname, &year) == 3)
        head = addToEnd(head, name, surname, year);  // Poziva funkciju koja dodaje novu osobu na kraj liste i vra?a (mogu?e novi) pokaziva? na po?etak liste


    fclose(f);
    printf("Lista uspjesno ucitana iz '%s'.\n", filename);
    return head;
}

