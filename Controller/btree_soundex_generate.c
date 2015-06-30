/* 
 * File:   btree_soundex_generate.c
 * Author: sakura
 *
 * Created on April 29, 2015, 11:12 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lib/btree.h"
#include "dict_generate.h"
#include "soundex.h"

main ()
{
    char *sd, *oldString, newString[MAX_LEN];
    BTA *btree, *btsoundex;
    
    btinit();
    btree = btopn ("Data/Anhviet.dict",0,0);
    btsoundex = create_btree ("Data/Anhvietsoundex.dict");
    
    char mean[MAX_LEN];
    char word[200], *ptr;
    int rsize, status;
    char *soundCode = (char*)malloc(20), *soundWord = (char*) malloc(200);
    
    btpos (btree,1);
    int n=0;
    
    while (btseln (btree,word,mean,MAX_LEN,&rsize) == 0)
    {
        soundCode = soundex (word);
        ptr = search_btree (btsoundex, soundCode);
        strcpy (soundWord,word);
        if (ptr==NULL)
        {
            btins(btsoundex,soundCode,soundWord,MAX_LEN);
        }
        else
        {
            oldString = ptr;
            strcpy (newString,oldString);
            newString[strlen (newString)] = '\0';
            strcpy (newString+strlen(newString),"@");
            newString[strlen (newString)] = '\0';
            strcpy (newString+strlen(newString),soundWord);
            newString[strlen (newString)] = '\0';
            btupd (btsoundex,soundCode,newString,MAX_LEN);
        }
    }
    btcls (btree);
    btcls (btsoundex);
}
