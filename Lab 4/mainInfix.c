/* name: mainInfix.c
 *
 * author: Michal Tešnar (m.tesnar@student.rug.nl, s4740556)
 * author: Wojciech Trejter (w.trejter@student.rug.nl, s4296850)
 *
 * date: 09.03.2022
 *
 * description: Main file which calls functions from infixExp.c
 * which in parse token list into an expression tree and evaluate it.
 */

#include "scanner.h"
#include "prefixExp.h"
#include "infixExp.h"

int main(int argc, char *argv[]) {
  infExpTrees();
  return 0;
}
