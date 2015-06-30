/* 
 * File:   dict_generate.h
 * Author: sakura
 *
 * Created on April 28, 2015, 10:28 PM
 */

#ifndef DICT_GENERATE_H
#define DICT_GENERATE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lib/btree.h"
#include <ctype.h>
#define MAX_LEN 15000

typedef struct
{
    char word[100];
    char position[10];
    char length[10];
} dict;

int get_position(char c, char *s)
{
    int i;
    for (i = 0; i < strlen (s); i++)
    {
        if (c == s[i])
            return i;
    }
}

int doi_co_so(char *s)
{
    char charMap[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int i, position, result = 0;
    for (i = 0; i < strlen (s); i++)
    {
        position = get_position (s[i], charMap);
        result += (int) pow (64, strlen (s) - i - 1) * position;
    }
    return result;
}

int test_char(char *s)
{
    int i;
    for (i = 0; i < strlen (s); i++)
        if (s[i] >= 'A' && s[i] <= 'Z')
            return 1;
    return 0;
}

BTA *create_btree(char *name)
{
    btinit ();
    return btcrt (name, 0, 0);
}

char *strlwr(char *s)
{
    int i;
    for (i = 0; i <= strlen (s); i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] += 32;
    }
    return s;
}

void read_file(BTA *btree)
{
    dict index;
    FILE *f_index = fopen ("Data/anhviet109K.index", "r");
    FILE *f_dict = fopen ("Data/anhviet109K.dict", "r");
    char *word = (char*) malloc (200);
    char *pch = (char*) malloc (100);
    int i = 0, j, k = 0, meanLen;
    long int position, length;
    char mean[15000], *meanx;
    while (!feof (f_index) && !feof (f_dict))
    {
        fgets (word, 200, f_index);
        word[strlen (word) - 1] = '\0';
        if (feof (f_index) || feof (f_dict))
            break;
        pch = strtok (word, "\t");
        strcpy (index.word, pch);
        pch = strtok (NULL, "\t");
        strcpy (index.position, pch);
        position = doi_co_so (index.position);
        pch = strtok (NULL, "\t");
        strcpy (index.length, pch);
        length = doi_co_so (index.length);
        fseek (f_dict, position, SEEK_SET);
        fread (mean, sizeof (char), length, f_dict);
        mean[length] = '\0';
        meanLen = strlen (mean);
        meanx = (char*) malloc (meanLen + 1);
        strcpy (meanx, mean);
        j = btins (btree, strlwr (index.word), meanx, meanLen + 1);
        while (j != 0)
        {
            index.word[strlen (index.word)] = ' ';
            index.word[strlen (index.word) + 1] = '\0';
            j = btins (btree, strlwr (index.word), meanx, meanLen + 1);
            k++;
            if (k == 5)
                break;
        }
        k = 0;
        i++;
    }
    fclose (f_index);
    fclose (f_dict);
}

void duyet_file(BTA *btree)
{
    int i = 0, rsize;
    char meanWord[MAX_LEN];
    char key[200];

    btpos (btree, 1);
    while (btseln (btree, key, meanWord, MAX_LEN, &rsize) == 0)
    {
        i++;
        printf ("%d\n", i);
        puts (key);
    }
}

char * search_btree(BTA *btree, char *word)
{
    char *fnumber = (char*) malloc (sizeof (char)*MAX_LEN);
    int rsize;
    int status = btsel (btree, word, fnumber, MAX_LEN, &rsize);

    if (status != 0)
        return NULL;
    else
        return fnumber;
}

int add_btree(BTA *btree, char *word, char *mean)
{
    int status;
    status = btins (btree,word,mean,MAX_LEN);
    if (status == 0)
        return 1;
    return 0; //update thanh cong
}

int delete_key(BTA *btree, char *word)
{
    int rsize, status;
    char fnumber[MAX_LEN];
    
    status = btdel (btree,word);
    if (status == 0)
        return 1; //xoa thanh cong
    return 0;
}

int update_key(BTA *btree, char *word, char *mean)
{
    if (btupd (btree,word, mean, MAX_LEN) == 0)
        return 1;
    return 0;
}

#endif  /* DICT_GENERATE_H */

