/* CZ4031 Database System */

#include <iostream>
#include <stdio.h>
#include <string.h>

typedef struct record
{
    char tconst[10];        // 10B. Primary key. Total we need is 9bits for the current data we have. + 1 bit to store the null value.
    float averageRating;    // 4B. Was thinking of using char but end up will take the same amount of byte (i.e. 4 bytes), 
                            // since 2 for the numbers, 1 for the "." and 1 for the null value
    unsigned int numVotes;  // 4B. was thinking of using short unsigned, but will be limited to 65,535, which is not sufficient. 
                            // Largest numVote value we have will go to hundred thousands.
}record;                    // Total B used is 18B.

typedef struct block{
  //  not sure how to automate the malloc process. probably don't need a block struct but as and when a block is created, 
  //  follow that starting pointer to the rest of the blocks
};

int main() {
//  Have to design a function that automatically creates a block and malloc memory to it.
//    struct record *block_1 = (struct record*)malloc(200);
//    struct record *ptr_block;
//    ptr_block = block_1;
    return 0;
}


