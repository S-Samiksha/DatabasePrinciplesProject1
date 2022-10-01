#ifndef DATABASEPROJ_TYPES_H
#define DATABASEPROJ_TYPES_H

#include <vector>
#include <iostream>
// Defines a single movie record.
struct Record
{
    char *tconst = new char[11]; // 11B. Primary key. Total we need is 9bits for the current data we have. + 1 bit to store the null value.

    float averageRating; // 4B. Was thinking of using char but end up will take the same amount of byte (i.e. 4 bytes),
                         // since 2 for the numbers, 1 for the "." and 1 for the null value

    unsigned int numVotes; // 4B. was thinking of using short unsigned, but will be limited to 65,535, which is not sufficient.
                           // Largest numVote value we have will go to a few hundred thousands.

}; // Total B used is 19B.

// Defines an address of a record that's stored as a block address with an offset,
class Node;
struct Address
{
    void *blockAddress;        // 8B. A pointer that points to the block address
    unsigned short int offset; // 4B. An offset used to calculate the position of the address within the block. Apparently same as unsigned int
public:
    Node *getAddressNode(){
        return (Node *)((char *)blockAddress + offset);
    };
}; // Total B used is 12B.
class Node
{
    friend class BPTree;

public:
    // current number of keys and pointers a Node is holding
    int currentKeySize{0}; 
    int currentPointerSize{0}; 
    int maxKeySize{0}; 
    int maxPointerSize{0};
    // switch all vectors to arrays
    // keys instantiated to 0 initially
    int *keys;
    // childrenNodes can either be &Record &Node or nullptr(initially)
    Address* childrenNodes{NULL};

    bool isLeaf;
    Address addressInDisk{NULL};

    Node();

    Node(int nodeSize, bool isLeaf);
    void printNode();

    void insertInitialInNonLeafNode(int key, Address leftPointer, Address rightPointer);

    void insertInitialInLeafNode(int key, Address recordPointer, Address neighbourNode);

    void insertSubsequentPair(int key, Address NodeOrRecordPointer);

    void insertKeyInKeyArray(int key, int index);

    Address remove(int remove);

    int binarySearch(int remove);

    void insertChildInPointerArray(Address child, int index);

    void linkToAnotherLeafNode(Address child);

    bool hasRightNeighbour(int index);

    bool hasLeftNeighbour(int index);

    int removeFirstKeyFromNode();

    int returnSize();

    bool isFull();

    // binary search the index to slot in the incoming key
    int binarySearchInsertIndex(int key);
};

#endif // DATABASEPROJ_TYPES_H
