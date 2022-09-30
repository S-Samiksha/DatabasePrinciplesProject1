#ifndef DATABASEPROJ_TYPES_H
#define DATABASEPROJ_TYPES_H
// #include "node.h"
// Defines a single movie record.
class Node{};
struct Record
{
    char *tconst = new char[11]; // 10B. Primary key. Total we need is 9bits for the current data we have. + 1 bit to store the null value.

    float averageRating; // 4B. Was thinking of using char but end up will take the same amount of byte (i.e. 4 bytes),
                         // since 2 for the numbers, 1 for the "." and 1 for the null value

    unsigned int numVotes; // 4B. was thinking of using short unsigned, but will be limited to 65,535, which is not sufficient.
                           // Largest numVote value we have will go to a few hundred thousands.

}; // Total B used is 19B.

// Defines an address of a record that's stored as a block address with an offset,
struct Address
{
    void *blockAddress;        // 8B. A pointer that points to the block address
    unsigned short int offset; // 4B. An offset used to calculate the position of the address within the block. Apparently same as unsigned int
public:
    Node *getAddressNode();
}; // Total B used is 12B.

#endif // DATABASEPROJ_TYPES_H
