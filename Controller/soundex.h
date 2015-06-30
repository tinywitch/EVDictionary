/* 
 * File:   soundex.h
 * Author: sakura
 *
 * Created on April 29, 2015, 11:12 PM
 */

#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

char *soundex (char *in)
{
    static int code[]= {0,1,2,3,0,1,2,0,0,2,2,4,5,5,0,1,2,6,2,3,0,1,0,2,0,2 };
  /* a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z */
    static char key[5];
    char ch;
    int last, counter;
    
    strcpy (key, "Z000");
    while (*in != '\0' && !isalpha(*in))
        ++in;
    if (*in == '\0')
        return key;
    key[0] = toupper (*in);
    last = code[key[0] - 'A'];
    ++in;
    for (counter = 1; counter <4 && *in != '\0'; ++in)
    {
        if (isalpha(*in))
        {
            ch = tolower (*in);
            if (last != code[ch - 'a'])
            {
                last = code[ch - 'a'];
                if (last != 0)
                    key[counter++] = '0' + last;
            }
        }
    }
    return key;
}

//tim tu co cung ma soundex nhung o ngay ben phai cua tu do
//Neu no la tu cuoi cung thi tra tu ngay dang truoc no

char *next_position_in_soundex_code(char *word, char *code)
{
    char *result = NULL, *prev = NULL, *forward = NULL;
    char *ptr, *code_string = strdup (code);
    char *buffer[15000];
    int *compare;
    
    ptr = strtok (code_string,"@");
    
    if(ptr == NULL)
        return NULL; //truong hop xau dau vao rong
    
    int i=0,j,n;
    while(ptr)
    {
        buffer[i] = strdup (ptr);
        ptr = strtok (NULL,"@");
        i++;
    }
    n=i;
    if (n==1) 
        return buffer[0];
    compare = (int*) malloc (n*sizeof(int));
    for (j=0;j<n;j++)
    {
        if (strcmp (word,buffer[j]) >0)
            compare[j] = 1;
        else if (strcmp (word,buffer[j])<0) 
            compare[j] = -1;
        else compare[j] = 0;
    }
    
    if (compare[0] == 1 && compare[n-1] == -1)
        return buffer[n-1];
    if (compare[0] == -1 && compare[n-1] == -1)
        return buffer[0];
    for (i=0;i<n;i++)
        if (compare[i] == 1 && compare[i+1] == -1)
            return buffer[i+1];
    for (i=0;i<n;i++)
        printf("%d : %s : %d",i,buffer[i],compare[i]);
    free(compare);
}

#endif  /* SOUNDEX_H */

