/* name: dictionary.c
 *
 * author: Michal Tešnar (m.tesnar@student.rug.nl, s4740556)
 * author: Wojciech Trejter (w.trejter@student.rug.nl, s4296850)
 *
 * date: 06.03.2022
 *
 * description: The dictionary is a functional implementation for the tries. It can add words or check whether words are already in the dictionary
 */

// implements a dictionary

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

dict *newEmptyDict() {
  dict *d = malloc(sizeof(dict));
  if (d == NULL) {
    return NULL;
  }
  d->root = newTrie();
  return d;
}

// add word to the dictionary if it is is not already known
void addWord(char word[LENGTH + 1], dict *d) {
  int index = 0;
  Trie dest = d->root; //the root of the trie
  while (word[index] != '\0') { 
    dest = addOrGetChild(dest, word[index]); //add / go through the node in the trie
    index++;
  }
  dest->endNode = 1;
}

// check whether word is in dictionary
bool check(const char *word, dict *d) {
  int index = 0;
  Trie dest = NULL;
  dest = getChild(d->root, word[index]); //first letter 
  int isEndNode = 1;
  while (word[index] != '\0' && dest != NULL) { //end at the end of the word or when the letter is not found in the trie 
    isEndNode = dest->endNode; //update isEndNode
    index++;
    dest = getChild(dest, word[index]); //check the next letter of the word
  }
  // we went to the end of the word or dest == NULL
  if (word[index] == '\0' && isEndNode == 1) {
    return 1;
  }
  return 0;
}

void freeDict(dict *d) {
  freeTrie(d->root);
  free(d);
}