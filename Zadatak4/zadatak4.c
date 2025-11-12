/* Napisati program za zbrajanje i množenje polinoma.
   Koeficijenti i eksponenti se ?itaju iz datoteke*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _poly* Position;
typedef struct _poly {
    int coeff;
    int pow;
    Position next;
} poly;

int sortedInsert(Position head, int coeff, int pow);  //sortirani unos
int loadFromFile(char* filename, Position first, Position second);  //u?itava dva polinoma
int addPoly(Position first, Position second, Position result);  //zbraja dva polinoma
int multiplyPoly(Position first, Position second, Position result);  //množi dva polinoma
void printPoly(Position head);  //ispis polinoma
void freeList(Position head);  //osloba?a listu

int main() {
    poly head1 = { .coeff = 0, .pow = 0, .next = NULL };
    poly head2 = { .coeff = 0, .pow = 0, .next = NULL };
    poly sumHead = { .coeff = 0, .pow = 0, .next = NULL };
    poly prodHead = { .coeff = 0, .pow = 0, .next = NULL };

    Position firstPoly = &head1;
    Position secondPoly = &head2;
    Position resultSum = &sumHead;
    Position resultProd = &prodHead;

    if (loadFromFile("polinomi.txt", firstPoly, secondPoly) != 0) {
        return EXIT_FAILURE;
    }

    printf("Prvi polinom:\n");
    printPoly(firstPoly);

    printf("Drugi polinom:\n");
    printPoly(secondPoly);

    addPoly(firstPoly, secondPoly, resultSum);
    printf("Zbroj polinoma:\n");
    printPoly(resultSum);

    multiplyPoly(firstPoly, secondPoly, resultProd);
    printf("Umnozak polinoma:\n");
    printPoly(resultProd);

    freeList(firstPoly);
    freeList(secondPoly);
    freeList(resultSum);
    freeList(resultProd);

    return 0;
}

int sortedInsert(Position head, int coeff, int pow) {
    Position newEl = NULL;
    Position prev = head;
    Position p = prev->next;

    if (coeff == 0) return 0;  //ignorira unos s koeficijentom 0

    while (p != NULL && p->pow > pow) {
        prev = p;
        p = p->next;
    }

    if (p != NULL && p->pow == pow) {  //ako postoji element s istom potencijom, koeficijenti se zbrajaju
        p->coeff += coeff;
        if (p->coeff == 0) {  //ako novi koeficijent postane 0, ukloni cvor
            prev->next = p->next;
            free(p);
        }
        return 0;
    }

    newEl = (Position)malloc(sizeof(poly));  //ako nema elementa s jednakom potencijom, alocira novi cvor i umece ga izmedu prev i p 

    if (!newEl) {
        printf("Greska pri alokaciji memorije!\n");
        return -1;
    }
    newEl->coeff = coeff;
    newEl->pow = pow;
    newEl->next = p;
    prev->next = newEl;

    return 0;
}

int loadFromFile(char* filename, Position p1, Position p2) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Greska pri otvaranju datoteke!\n");
        return -1;
    }

    char buffer = '0';
    int c = 0, p = 0;

    //prvi polinom
    while (buffer != '\n' && !feof(f)) {
        if (fscanf(f, "%d %d", &c, &p) == 2)
            sortedInsert(p1, c, p);
        buffer = fgetc(f);
    }

    buffer = '0';

    //drugi polinom
    while (buffer != feof(f)) {
        if (fscanf(f, "%d %d", &c, &p) == 2)
            sortedInsert(p2, c, p);
        buffer = fgetc(f);
    }

    fclose(f);
    return 0;
}

int addPoly(Position first, Position second, Position result) {
    Position p1 = first->next;
    Position p2 = second->next;

    while (p1 != NULL && p2 != NULL) {
        if (p1->pow > p2->pow) {
            if (sortedInsert(result, p1->coeff, p1->pow) != 0) return -1;
            p1 = p1->next;
        }
        else if (p1->pow < p2->pow) {
            if (sortedInsert(result, p2->coeff, p2->pow) != 0) return -1;
            p2 = p2->next;
        }
        else {
            int sum = p1->coeff + p2->coeff;
            if (sum != 0) {
                if (sortedInsert(result, sum, p1->pow) != 0) return -1;
            }
            p1 = p1->next;
            p2 = p2->next;
        }
    }

    //kopira ostatak (ako se jedan polinom zavrsi prije drugoga, ucitava ostatk onoga kojem nije jos dosao do kraja)
    while (p1 != NULL) {
        if (sortedInsert(result, p1->coeff, p1->pow) != 0) return -1;
        p1 = p1->next;
    }
    while (p2 != NULL) {
        if (sortedInsert(result, p2->coeff, p2->pow) != 0) return -1;
        p2 = p2->next;
    }

    return 0;
}

int multiplyPoly(Position first, Position second, Position result) {
    Position p1 = first->next;
    Position p2 = NULL;

    while (p1 != NULL) {
        p2 = second->next;  //vraca ga uvijek na prvog clana da bi se iduci clan p1 pomnozio sa svakim p2 clanom
        while (p2 != NULL) {
            int newCoeff = p1->coeff * p2->coeff;
            int newPow = p1->pow + p2->pow;
            if (newCoeff != 0) {
                if (sortedInsert(result, newCoeff, newPow) != 0) return -1;
            }
            p2 = p2->next;
        }
        p1 = p1->next;
    }
    return 0;
}

void printPoly(Position head) {
    Position p = head->next;
    int first = 1;

    if (p == NULL) {
        printf("Lista je prazna!\n");
        return;
    }

    //ispis predznaka
    while (p != NULL) {
        int c = p->coeff;
        int a = (c < 0) ? -c : c;  //a je apsolutna vrijednost od c
        if (!first) {  //ako nije prvi clan
            if (c > 0) printf(" + ");
            else printf(" - ");
        }
        else {  //ako je prvi clan
            if (c < 0) printf("-");
            first = 0;  //nakon ispisa predznaka (ukoliko je negativan) prvog clana first se postavlja na nulu
        }

        //ispis vrijednosti elementa
        if (p->pow == 0) {
            printf("%d", a);
        }
        else if (p->pow == 1) {
            if (a == 1) printf("x");
            else printf("%dx", a);
        }
        else {
            if (a == 1) printf("x^%d", p->pow);
            else printf("%dx^%d", a, p->pow);
        }

        p = p->next;
    }
    printf("\n");
}

void freeList(Position head) {
    Position tmp = head->next;
    Position next = NULL;
    while (tmp != NULL) {
        next = tmp->next;
        free(tmp);
        tmp = next;
    }
    head->next = NULL;
}