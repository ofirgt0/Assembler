#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *my_strtok_r(char *str, const char *delim, char **saveptr)
{
    char *token;
    if (str == NULL)
    {
        if (saveptr == NULL)
        {
            printf("Error: saveptr is NULL\n");
            return NULL;
        }
        str = *saveptr;
    }
    str += strspn(str, delim);
    if (*str == '\0')
    {
        *saveptr = str;
        return NULL;
    }
    token = str;
    str = strpbrk(token, delim);
    if (str == NULL)
    {
        *saveptr = (char *)strchr(token, '\0');
    }
    else
    {
        *str = '\0';
        *saveptr = str + 1;
    }
    return token;
}

char *my_strdup(const char *s)
{
    char *new;

    if (s == NULL)
    {
        printf("Error: Null pointer passed to my_strdup.\n");
        return NULL;
    }

    new = (char *)malloc(strlen(s) + 1);
    if (new == NULL)
    {
        printf("Memory allocation failed");
        return NULL;
    }

    strcpy(new, s);
    return new;
}