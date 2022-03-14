/* name: infixExp.h
 *
 * author: Michal Tešnar (m.tesnar@student.rug.nl, s4740556)
 * author: Wojciech Trejter (w.trejter@student.rug.nl, s4296850)
 *
 * date: 09.03.2022
 *
 * description: Header file for infixExp.c.
 */

#ifndef INFIXEXP_H
#define INFIXEXP_H

void infExpTrees();
int treeExpression(List *lp, ExpTree *tp);
int treeTerm(List *lp, ExpTree *tp);
int treeFactor(List *lp, ExpTree *tp);
int checkOperator(List *lp, char *cp);
void simplify (ExpTree *tp);
void differentiate (ExpTree *tp);
ExpTree copyExpTree(ExpTree *tp);

#endif