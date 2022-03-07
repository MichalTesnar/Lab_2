/* name: trie.h
 *
 * author: Michal Tešnar (m.tesnar@student.rug.nl, s4740556)
 * author: Wojciech Trejter (w.trejter@student.rug.nl, s4296850)
 *
 * date: 06.03.2022
 *
 * description: Declaration of trie; header.
 */

#include <stdbool.h>

typedef struct TrieNode *Trie;

struct TrieNode {
  bool endNode;
  int *hasChild;
  Trie *children;
};

Trie newTrie();

Trie getChild(Trie t, char ch);

Trie addOrGetChild(Trie t, char ch);

void freeTrie(Trie t);
