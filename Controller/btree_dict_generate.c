/* 
 * File:   soundex.h
 * Author: sakura
 *
 * Created on April 29, 2015, 11:12 PM
 */

#include "dict_generate.h"
#include "string.h"

main()
{
    btinit ();
    BTA *btree = create_btree ("Data/Anhviet.dict");
    read_file (btree);
    btcls(btree);
}
