#ifndef HELPFULFUNCTIONS_H
#define HELPFULFUNCTIONS_H

/**
 * This function works like the standard 'strtok' function but is reentrant and doesn't modify the delimiters string.
 * It returns a pointer to the next token in 'str' that is delimited by a character from 'delim'.
 * If there are no more tokens, it returns NULL.
 */
char *my_strtok_r(char *str, const char *delim, char **saveptr);

/**
 * Duplicates a string by creating a new copy in the heap.
 * This function allocates memory for the new string, copies
 * the original string into the new memory, and returns a pointer to it.
 */
char *my_strdup(const char *s);

#endif /* HELPFULFUNCTIONS_H */