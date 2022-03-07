/* name: dictionary.h
 *
 * author: Michal Tešnar (m.tesnar@student.rug.nl, s4740556)
 * author: Wojciech Trejter (w.trejter@student.rug.nl, s4296850)
 *
 * date: 06.03.2022
 *
 * description: Declaration of dictonary; header.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include "trie.h"

// maximum length for a word
#define LENGTH 45

// a dictionary is an array
typedef struct dict {
  Trie root;
} dict;

dict *newEmptyDict();

void addWord(char word[LENGTH + 1], dict *d);

bool check(const char *word, dict *d);

void freeDict(dict *d);

#endif // DICTIONARY_H