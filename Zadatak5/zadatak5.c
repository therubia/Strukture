#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX 100

typedef Postfix* Position;

typedef struct _postfix {
    double number;
    struct _postfix* next;
} Postfix;

void PUSH(Position head, double value);  //dodaje novi element na pocetak
int POP(Position head, Position* first, Position* second);  //uzima dva elementa s vrha stacka
void performOperation(Position head, char op);  //funkcija za izvršavanje operacije

int main() {
    FILE* f = fopen("postfix.txt", "r");
    if (!f) {
        printf("Ne mogu otvoriti datoteku!\n");
        return EXIT_FAILURE;
    }

    Postfix headStruct;
    headStruct.next = NULL;
    Position head = &headStruct;

    char token[MAX];
    while (fscanf(f, "%s", token) != EOF) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            PUSH(head, atof(token));
        }
        else {
            performOperation(head, token[0]);
        }
    }

    if (!head->next) {      // Provjera da li je ostao samo jedan element
        printf("Greska: nema rezultata na stacku!\n");
        fclose(f);
        return EXIT_FAILURE;
    }
    else if (head->next->next != NULL) {
        printf("Greska: vise od jednog elementa na stacku!\n");
        fclose(f);
        return EXIT_FAILURE;
    }
    else {
        printf("Rezultat postfiks izraza je: %.2lf\n", head->next->number);
    }

    fclose(f);
    return EXIT_SUCCESS;
}

void PUSH(Position head, double value) {
    Position newElem = (Position)malloc(sizeof(Postfix));
    if (!newElem) {
        printf("Greska pri alokaciji memorije!\n");
        exit(EXIT_FAILURE);
    }
    newElem->number = value;
    newElem->next = head->next;
    head->next = newElem;
}

int POP(Position head, Position* first, Position* second) {
    if (!head->next || !head->next->next) {
        printf("Greska: premalo elemenata na stacku!\n");
        return 0;
    }
    *first = head->next;
    *second = head->next->next;
    return 1;
}

void performOperation(Position head, char op) {
    Position first, second, temp;

    if (!POP(head, &first, &second)) return;

    temp = (Position)malloc(sizeof(Postfix));
    if (!temp) {
        printf("Greska pri alokaciji memorije!\n");
        exit(EXIT_FAILURE);
    }

    switch (op) {
    case '+': temp->number = second->number + first->number; break;
    case '-': temp->number = second->number - first->number; break;
    case '*': temp->number = second->number * first->number; break;
    case '/':
        if (first->number == 0) {
            printf("Greska: dijeljenje nulom!\n");
            free(temp);
            return;
        }
        temp->number = second->number / first->number;
        break;
    default:
        printf("Nepoznat operator: %c\n", op);
        free(temp);
        return;
    }

    // Dodavanje rezultata na stack
    temp->next = head->next->next->next;  // preskacemo prva dva elementa
    head->next = temp;                  

    first->next = NULL;
    second->next = NULL;
    free(first);
    free(second);
}