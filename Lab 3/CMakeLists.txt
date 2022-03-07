cmake_minimum_required(VERSION 3.20)
project(Lab_3 C)

set(CMAKE_C_STANDARD 99)

add_executable(speller speller.c dictionary.h dictionary.c trie.h trie.c)
