/* name: speller.c
 *
 * author: Michal Tešnar (m.tesnar@student.rug.nl, s4740556)
 * author: Wojciech Trejter (w.trejter@student.rug.nl, s4296850)
 *
 * date: 02.03.2022
 *
 * description: This program reads in a dictionary terminated by '!'.
 * Then it reads input and counts how many words are not in the
 * dictionary and prints them.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "dictionary.h"


// remove non-alphabetic characters and convert to lower case
void trimWord(char *word) {
  int k = 0;
  for (int i = 0; i < (int) strlen(word); i++) {
    if (isalpha(word[i])) {
      word[k] = tolower(word[i]);
      k++;
    }
  }
  word[k] = '\0';
}

int main(int argc, char *argv[]) {
  char word[LENGTH + 1] = "";
  // read in the dictionary
  dict *dictionary = newEmptyDict();
  while (scanf("%45s", word) && word[0] != '!') {
    trimWord(word);
    addWord(word, dictionary);
  }
  getchar(); // removing the '!' at the end of dictionary
  int counter = 0; // number of unknown words
  int index = 0; // index in the string
  int c = EOF;
  while ((c = getchar()) && c != EOF) { // while there are characters on the line

    if (isalpha(c)) { //is c a letter of alphabet? Add it to the word
      word[index] = tolower(c); // convert and add to the word
      word[index + 1] = '\0'; // always add '\0' at the end so that each word ends with it
      index++;
    } else {
      if (word[0] != '\0' && !check(word, dictionary)) { // check if the word is in the dictionary if it is not empty
        counter++; // count unknown words
        printf("%s\n", word);
      }
      // reset reading of the word
      word[0] = '\0';
      index = 0;
    }
  }
  // print number of unknown words
  printf("%d\n", counter);
  freeDict(dictionary);
  return 0;
}