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

int quadraticRoots(int a, int b, int c, double *root1, double *root2) {
    int numberOfSolutions;
    if (a == 0) { // the equation is actually linear
        *root1 = -(double) c / b;
        numberOfSolutions = 1;
    } else { //the equation is quadratic
        int discriminant = b * b - 4 * a * c; // Calculating teh discriminant for quadratic solution
        if (discriminant == 0) { // There is only one solution
            numberOfSolutions = 1;
            *root1 = -b / (2 * a);
        } else if (discriminant > 0) { // There are two real solutions
            numberOfSolutions = 2;
            *root1 = (-b + sqrt(discriminant)) / (2 * a); //first solution
            *root2 = (-b - sqrt(discriminant)) / (2 * a); //second solution
        } else {
            numberOfSolutions = 0;
        }
    }
    return numberOfSolutions;
}

int degree2Solution(List tl, double *solution1, double *solution2) {
    // if the function is called, in 'tl' there is only one variable of maximum degree 2
    // we are solving equation in form ax^2+bx+c = 0 counters refer to that
    int aCounter = 0; // The quantity of indentity of power 2
    int bCounter = 0; // The quantity of indentity of power 1
    int cCounter = 0; // The quantity of free variables in the equation
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
                    cCounter += left * minus;
                    minus = 1;
                } else if ((tl->next->next->t).number == 1) { // x^1
                    bCounter += left * minus;
                    minus = 1;
                } else if ((tl->next->next->t).number == 2) { // x^2
                    aCounter += left * minus;
                    minus = 1;
                }
                tl = tl->next->next;
            } else { // variable not followed by '^'
                bCounter += left * minus;
                minus = 1;
            }
        } else if (tl->tt == Number) { // number
            if (tl->next != NULL && tl->next->tt == Identifier) { //number followed by variable
                if (tl->next->next != NULL && tl->next->next->tt == Symbol &&
                    (tl->next->next->t).symbol == '^') { //variable followed by '^'
                    if ((tl->next->next->next->t).number == 0) { // x^0
                        cCounter += left * minus * (tl->t).number;
                        minus = 1;
                    } else if ((tl->next->next->next->t).number == 1) { // x^1
                        bCounter += left * minus * (tl->t).number;
                        minus = 1;
                    } else if ((tl->next->next->next->t).number == 2) { // x^2
                        aCounter += left * minus * (tl->t).number;
                        minus = 1;
                    }
                    tl = tl->next->next;
                } else { // variable not followed by '^'
                    bCounter += left * (tl->t).number * minus;
                    minus = 1;
                }
                tl = tl->next;
            } else {
                // number not followed by variable, we add to the freeCounter
                cCounter += left * (tl->t).number * minus;
                minus = 1;
            }
        }
        tl = tl->next;
    }
    // return the number of solutions while passing values into pointers
    return quadraticRoots(aCounter, bCounter, cCounter, solution1, solution2);
}

void solveEquation() { // This function is responsible for calling individual functions and evaluating the equaitons.
    char *ar;
    List tl, tl1;
    printf("give an equation: ");
    ar = readInput();
    while (ar[0] != '!') {
        tl = tokenList(ar);
        printList(tl);
        tl1 = tl;
        if (acceptEquation(&tl1) && tl1 == NULL) {
            int vars = numberOfVariables(tl); // number of variables in an equation
            int power = powerOfEquation(tl); // highest power of an equation
            printf("this is an equation");
            if (vars == 1) { // equation with one variable
                printf(" in 1 variable of degree %i\n", power);
                // equation of power 1 requiring a solution
                if (power == 1) { // linear equation
                    int solvable = 1;
                    double sol = degree1Solution(tl, &solvable);
                    if (solvable) {
                        if (sol <= 0 && sol > -0.0005) { // avoiding -0.000
                            printf("solution: 0.000\n");
                        } else {
                            printf("solution: %.3f\n", sol);
                        }
                    } else {
                        printf("not solvable\n");
                    }
                } else if (power == 2) { // quadratic equation
                    double solution1, solution2;
                    int numberOfSolutions = degree2Solution(tl, &solution1, &solution2);
                    if (numberOfSolutions == 1) {
                        printf("solution: %.3f\n", solution1);
                    } else if (numberOfSolutions == 2) {
                        printf("solution: %.3f and %.3f\n", solution1, solution2);
                    } else {
                        printf("not solvable\n");
                    }
                }
            } else {
                printf(", but not in 1 variable\n");
            }
        } else {
            printf("this is not an equation\n");
        }
        free(ar);
        freeTokenList(tl);
        printf("\ngive an equation: ");
        ar = readInput();
    }
    free(ar);
    printf("good bye\n");
}