/* name: trie.c
 *
 * author: Michal Tešnar (m.tesnar@student.rug.nl, s4740556)
 * author: Wojciech Trejter (w.trejter@student.rug.nl, s4296850)
 *
 * date: 06.03.2022
 *
 * description: Implementation of trie for a dictionary.
 * It is possible to add a node with a letter and search
 * whether a node with letter exists.
 */

#include "trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Trie newTrie() {
  Trie output = malloc(sizeof(struct TrieNode));
  if (output == NULL) {
    return NULL;
  }
  //initialise the elements of the trie
  output->endNode = 0;
  output->children = malloc(sizeof(Trie) * 26);
  output->hasChild = malloc(sizeof(int) * 26);
  for (int i = 0; i < 26; i++) { //initialising hasChild to 0
    output->hasChild[i] = 0;
  }
  return output;
}

// return the child or NULL if the desired child does not exist
Trie getChild(Trie t, char ch) {
  if (ch == '\0') { //make sure we are looking for a real solution
    return NULL;
  }
  if ((t->hasChild[ch - 'a']) != 0) { //return a child if it exists
    return (t->children[ch - 'a']);
  }
  // if not found return NULL
  return NULL;
}

//this function returns the node of the new child, or one which already exists with the char ch
Trie addOrGetChild(Trie t, char ch) {
  if (t->hasChild[ch - 'a'] != 0) { //if the node exists return it
    return (t->children[ch - 'a']);
  }
  t->children[ch - 'a'] = newTrie(); //otherwise create a new child
  t->hasChild[ch - 'a'] = 1; //and indicate it exist
  return t->children[ch - 'a'];
}

//recursively frees every Trie from the very bottom
void freeTrie(Trie t) { 
  for (int i = 0; i < 26; i++) {
    if (t->hasChild[i] != 0) {
      freeTrie(t->children[i]);
    }
  }
  free(t->children);
  free(t->hasChild);
  free(t);
}
