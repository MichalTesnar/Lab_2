/* name: recognizeEq.c
 *
 * author: Michal Tešnar (m.tesnar@student.rug.nl, s4740556)
 * author: Wojciech Trejter (w.trejter@student.rug.nl, s4296850)
 *
 * date 22.02.2022
 *
 * description: This program recognizes equations from the input
 * it checks whether it features only one variable,
 * if so it also outputs the degree of the equation.
 * If the equation has one real-valued solution is printed,
 * otherwise 'not solvable' is printed.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "scanner.h"
#include "recognizeEq.h"

int acceptNumber(List *lp) {
    if (*lp != NULL && (*lp)->tt == Number) {
        *lp = (*lp)->next;
        return 1;
    }
    return 0;
}

int acceptIdentifier(List *lp) {
    if (*lp != NULL && (*lp)->tt == Identifier) {
        *lp = (*lp)->next;
        return 1;
    }
    return 0;
}

int acceptCharacter(List *lp, char c) {
    if (*lp != NULL && (*lp)->tt == Symbol && ((*lp)->t).symbol == c) {
        *lp = (*lp)->next;
        return 1;
    }
    return 0;
}

int acceptFactor(List *lp) {
    return
            (acceptNumber(lp)
             || acceptIdentifier(lp)
             || (acceptCharacter(lp, '(')
                 && acceptExpression(lp)
                 && acceptCharacter(lp, ')')
             )
            );
}

int acceptTerm(List *lp) {
    // term optionally starts with a number
    if (acceptNumber(lp)) {
        // optionally followed by identifier
        if (acceptIdentifier(lp)) {
            // optionally followed by '^'
            if (acceptCharacter(lp, '^')) {
                // then it must be followed by a natural number
                return acceptNumber(lp);
            } else {
                return 1;
            }
        } else {
            return 1;
        }
    } else {
        // variable is not quantified
        if (acceptIdentifier(lp)) {
            // optionally followed by '^'
            if (acceptCharacter(lp, '^')) {
                // then it must be followed by a natural number
                return acceptNumber(lp);
            } else {
                return 1;
            }
        }
        return 0;
    }
}

int acceptExpression(List *lp) {
    // optionally starts with a '-'
    acceptCharacter(lp, '-');
    // first term
    if (!acceptTerm(lp)) {
        return 0;
    }
    // one or more other terms separated by '+' or '-'
    while (acceptCharacter(lp, '+') || acceptCharacter(lp, '-')) {
        if (!acceptTerm(lp)) {
            return 0;
        }
    }
    return 1;
}

int acceptEquation(List *lp) {
    // left part of the equation
    if (!acceptExpression(lp)) {
        return 0;
    }
    // equal sign
    if (!acceptCharacter(lp, '=')) {
        return 0;
    }
    // right part of the equation
    if (!acceptExpression(lp)) {
        return 0;
    }
    return 1;
}

// This function checks the number of variables and returns 1 if there is only one variable.
int numberOfVariables(List tl) {
    char *value = NULL;
    int oneVariable = 0; // bolean indicating whether there is one variable
    while (tl != NULL) {
        if (tl->tt == Identifier) {
            // the first identifier which later will be compared to
            if (value == NULL) {
                value = (tl->t).identifier;
                oneVariable = 1;
                continue;
            }
            // if there are two identifiers which are not the same, we can stop
            if (strcmp(value, ((tl->t).identifier))) {
                return 0;
            }
        }
        tl = tl->next;
    }
    return oneVariable;
}

// this function returns the degree of the equation
int powerOfEquation(List tl) {
    int highestPower = 1;
    while (tl->next != NULL) {
        // we always check the value after the '^'
        if (tl->tt == Symbol && (tl->t).symbol == '^') {
            int current = ((tl->next)->t).number;
            //if the new power is higher than the previous highest, save the new highest value
            highestPower = current > highestPower ? current : highestPower;
        }
        tl = tl->next;
    }
    return highestPower;
}

void
assignToVariable(List *tl, int value, char **nameFirst, char **nameSecond, double *idenCounter1, double *idenCounter2,
                 int *minus, const int *left) {
    if (*nameFirst == NULL || !strcmp(((*tl)->t).identifier, *nameFirst)) {
        *nameFirst = ((*tl)->t).identifier;
        *idenCounter1 += *left * *minus * value;
        *minus = 1;
    } else {
        *nameSecond = ((*tl)->t).identifier;
        *idenCounter2 += *left * *minus * value;
        *minus = 1;
    }
}

void getCoefficients(List tl, double *varA, double *varB, double *varFree, char **n1, char **n2) {
//    printf("gettingCoefficients\n");
    // if the function is called, in 'tl' there is only one variable of degree 1
    // we are solving equation in form idenCounter1*x + idenCounter2 + freeCounter = 0
    double idenCounter1 = 0; // The quantity of one of the identities of power 1
    double idenCounter2 = 0; // The quantity of the second of the identities of power 1
    double freeCounter = 0; // The quantity of free variables in the equation
    char *nameFirst = NULL;
    char *nameSecond = NULL;
    int left = 1; // multiplier when we pass '='
    int minus = 1; // multiplier after we pass '-'
    while (tl != NULL) {
        if (tl->tt == Symbol && (tl->t).symbol == '-') {
            minus = -1 * minus; // minus precedes the number or identifier
        } else if (tl->tt == Symbol && (tl->t).symbol == '=') {
            left = -1; // going to right side of equation
        } else if (tl->tt == Identifier) { //variable without preceding number
            if (tl->next != NULL && tl->next->tt == Symbol &&
                (tl->next->t).symbol == '^') {   // variable followed by '^'
                if ((tl->next->next->t).number == 0) { // x^0
                    freeCounter += left * minus;
                    minus = 1;
                }
                if ((tl->next->next->t).number == 1) { // x^1
                    assignToVariable(&tl, 1, &nameFirst, &nameSecond, &idenCounter1, &idenCounter2, &minus, &left);
                }
                tl = tl->next->next;
            } else { // variable not followed by '^'
                assignToVariable(&tl, 1, &nameFirst, &nameSecond, &idenCounter1, &idenCounter2, &minus, &left);
            }
        } else if (tl->tt == Number) { // number
            if (tl->next != NULL && tl->next->tt == Identifier) { //number followed by variable
                if (tl->next->next != NULL && tl->next->next->tt == Symbol &&
                    (tl->next->next->t).symbol == '^') { //variable followed by '^'
                    if ((tl->next->next->next->t).number == 0) { //x^0
                        freeCounter -= left * (tl->t).number * minus;
                        minus = 1;
                    } else { //x^1
                        assignToVariable(&(tl->next), tl->t.number, &nameFirst, &nameSecond, &idenCounter1,
                                         &idenCounter2, &minus, &left);
                    }
                    tl = tl->next->next;
                } else { // variable not followed by '^'
                    assignToVariable(&(tl->next), tl->t.number, &nameFirst, &nameSecond, &idenCounter1, &idenCounter2,
                                     &minus, &left);
                }
                tl = tl->next;
            } else {
                // number not followed by variable, we add to the freeCounter
                freeCounter += left * (tl->t).number * minus;
                minus = 1;
            }
        }
        tl = tl->next;
    }
    // If the sum of identifiers is 0 then we cannot solve the equation
//    if (idenCounter == 0) {
//        *solvable = 0;
//    }
    if (nameFirst != NULL && nameSecond != NULL) {
        *varA = strcmp(nameFirst, nameSecond) < 0 ? idenCounter1 : idenCounter2;
        *varB = strcmp(nameFirst, nameSecond) > 0 ? idenCounter1 : idenCounter2;
        *varFree = freeCounter;
        *n1 = strcmp(nameFirst, nameSecond) < 0 ? nameFirst : nameSecond;
        *n2 = strcmp(nameFirst, nameSecond) > 0 ? nameFirst : nameSecond;
    }else{
        *varA = idenCounter1;
        *varB = 0;
        *varFree = freeCounter;
        *n1 = nameFirst;
        *n2 = NULL;
    }
}

//it will fail for two equations of one variable. Name of variables may be incorrect
void twoEquationsSolution(List tl1, List tl2, double *varA, double *varB, char **nam1, char **nam2) {
//    printf("start twoEquationsSolution\n");
    double varA1 = 0;
    double varA2 = 0;
    double varB1 = 0;
    double varB2 = 0;
    double free1 = 0;
    double free2 = 0;
    char *varName1A, *varName1B;
    char *varName2A, *varName2B;
    getCoefficients(tl1, &varA1, &varB1, &free1, &varName1A, &varName1B);
    getCoefficients(tl2, &varA2, &varB2, &free2, &varName2A, &varName2B);
//    printf("x = %f, y = %f, c = %f\n", varA1, varB1, free1);
//    printf("x = %f, y = %f, c = %f\n", varA2, varB2, free2);
    // When the two varNames are not the same, one of the equation had only one variable.
    double temp;
    char * tempCh;
    if (varB2 == 0) { // If the second equation has only one variable, swap the equations
        temp = varA1;
        varA1 = varA2;
        varA2 = temp;
        temp = varB1;
        varB1 = varB2;
        varB2 = temp;
        temp = free1;
        free1 = free2;
        free2 = temp;
    }
    if (strcmp(varName1A, varName2A) != 0) { // The first variable is not the same
        // change the order of variables in 2nd equation so that the 1st variable in the equations is the same
        temp = varA2;
        varA2 = varB2;
        varB2 = temp;
        tempCh = varName2A;
        varName2A = varName2B;
        varName2B = tempCh;
    }
    //Values from first equation will be substituted for the first variable in the second equation
    varB2 -= varA2 * varB1 / varA1;
    free2 -= varA2 * free1 / varA1;

    *varB = -free2 / varB2;
    *varA = -(free1 + (varB1 * *varB)) / varA1;

    *nam1 = varName2A;
    *nam2 = varName2B;
}

double degree1Solution(List tl, int *solvable) {
    // if the function is called, in 'tl' there is only one variable of degree 1
    // we are solving equation in form idenCounter*x = freeCounter
    double idenCounter = 0; // The quantity of indentity of power 1
    double freeCounter = 0; // The quantity of free variables in the equation
    int left = 1; // multiplier when we pass '='
    int minus = 1; // multiplier after we pass '-'
    while (tl != NULL) {
        if (tl->tt == Symbol && (tl->t).symbol == '-') {
            minus = -1 * minus; // minus precedes the number or identifier
        } else if (tl->tt == Symbol && (tl->t).symbol == '=') {
            left = -1; // going to right side of equation
        } else if (tl->tt == Identifier) { //variable without preceding number
            if (tl->next != NULL && tl->next->tt == Symbol &&
                (tl->next->t).symbol == '^') {   // variable followed by '^'
                if ((tl->next->next->t).number == 0) { // x^0
                    freeCounter -= left * minus;
                    minus = 1;
                }
                if ((tl->next->next->t).number == 1) { // x^1
                    idenCounter += left * minus;
                    minus = 1;
                }
                tl = tl->next->next;
            } else { // variable not followed by '^'
                idenCounter += left * minus;
                minus = 1;
            }
        } else if (tl->tt == Number) { // number
            if (tl->next != NULL && tl->next->tt == Identifier) { //number followed by variable
                if (tl->next->next != NULL && tl->next->next->tt == Symbol &&
                    (tl->next->next->t).symbol == '^') { //variable followed by '^'
                    if ((tl->next->next->next->t).number == 0) { //x^0
                        freeCounter -= left * (tl->t).number * minus;
                        minus = 1;
                    } else { //x^1
                        idenCounter += left * (tl->t).number * minus;
                        minus = 1;
                    }
                    tl = tl->next->next;
                } else { // variable not followed by '^'
                    idenCounter += left * (tl->t).number * minus;
                    minus = 1;
                }
                tl = tl->next;
            } else {
                // number not followed by variable, we add to the freeCounter
                freeCounter -= left * (tl->t).number * minus;
                minus = 1;
            }
        }
        tl = tl->next;
    }
    // If the sum of identifiers is 0 then we cannot solve the equation
    if (idenCounter == 0) {
        *solvable = 0;
    }
    //3. Return the division of 1. and 2.
    return (double) freeCounter / idenCounter;
}


void solveEquation() { // This function is responsible for calling individual functions and evaluating the equaitons.
    char *ar1, *ar2;
    List tl1, tl11;
    List tl2, tl21;
    printf("give an equation: ");
    ar1 = readInput();
    tl1 = tokenList(ar1);
    printList(tl1);
    tl11 = tl1;
    printf("give an equation: ");
    ar2 = readInput();
    tl2 = tokenList(ar2);
    printList(tl2);
    tl21 = tl2;
    double A = 0;
    double B = 0;
    char *nam1 = NULL;
    char *nam2 = NULL;
    while (ar1[0] != '!') {
        if (acceptEquation(&tl11) && tl11 == NULL && acceptEquation(&tl21) && tl21 == NULL) {
            twoEquationsSolution(tl1, tl2, &A, &B, &nam1, &nam2);
            if(A != A || B != B) {
                printf("not solvable\n");
            }
            else {
                printf("solution: %s = %.3f and %s = %.3f\n", nam1, A, nam2, B);
            }
        } else {
            printf("this is not an equation\n");
        }
        free(ar1);
        free(ar2);
        freeTokenList(tl1);
        freeTokenList(tl2);

        printf("give an equation: ");
        ar1 = readInput();
        if(ar1[0] == '!'){
            break;
        }
        tl1 = tokenList(ar1);
        printList(tl1);
        tl11 = tl1;
        printf("give an equation: ");
        ar2 = readInput();
        tl2 = tokenList(ar2);
        printList(tl2);
        tl21 = tl2;
    }
    free(ar1);
    printf("good bye\n");
}