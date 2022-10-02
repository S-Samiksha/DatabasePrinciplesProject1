#include <iostream>
#include "include/types.h"
//#include "include/node.h"
#include "include/memorypool.h"
#include "include/bptree.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <functional>
#include <algorithm>
#include <math.h>
#include <inttypes.h>
// #include <types.h>

int BLOCKSIZE = 200;
int MEMORYPOOLSIZE = 500000000;
struct tempRecord
{
    char tconst[11]; // 10B. Primary key. Total we need is 9bits for the current data we have. + 1 bit to store the null value.

    float averageRating; // 4B. Was thinking of using char but end up will take the same amount of byte (i.e. 4 bytes),
                         // since 2 for the numbers, 1 for the "." and 1 for the null value

    unsigned int numVotes; // 4B. was thinking of using short unsigned, but will be limited to 65,535, which is not sufficient.
                           // Largest numVote value we have will go to a few hundred thousands.
    bool operator<(const tempRecord &rec) const
    {
        return (numVotes < rec.numVotes);
    }
};
int main()
{
    // Final CLI copy down here once done!!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Select Block size:   " << std::endl;
    int options = 0;
    while (options != 1 && options != 2)
    {
        std::cout << "Enter a choice: " << std::endl;
        std::cout << "1. 200 B " << std::endl;
        std::cout << "2. 500 B" << std::endl;
        std::cin >> options;
        if (int(options) == 1)
        {
            BLOCKSIZE = int(200);
        }
        else if (int(options) == 2)
        {
            BLOCKSIZE = int(500);
        }
        else
        {
            std::cin.clear();
            std::cout << "Invalid input! Only 1 and 2 are allowed. " << std::endl;
        }
    }

    std::cout << "Creating memory......" << std::endl;

    std::cout << "Creating B++ Tree" << std::endl;
    // calculation of Node side
    int NodeSize = 0;
    // calculating the size of the nodes

    NodeSize = floor((BLOCKSIZE * 8 - 80 - 127 - 64) / 96);

    // loading data
    MemoryPool disk(MEMORYPOOLSIZE, BLOCKSIZE); // 150MB
    std::ifstream file("./data/data.tsv");
    std::vector<tempRecord> recordList;

    // test b+ tree
    BPTree *tree = new BPTree(NodeSize, MEMORYPOOLSIZE, BLOCKSIZE);

    // todo: remove test counter
    int recordCounter = 0;
    int nodeCounter = 0;

    // Insert data into database
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            tempRecord newRec;
            std::stringstream datastream(line);
            std::string data;
            std::getline(datastream, data, '\t');
            strcpy(newRec.tconst, data.c_str());
            datastream >> newRec.averageRating >> newRec.numVotes;
            recordList.push_back(newRec);
        }
    }

    std::sort(recordList.begin(), recordList.end());

    std::vector<Address> addressList;
    std::vector<unsigned int> keyList;

    // Load record onto disk
    // std::vector<Address> addressList1;
    for (int i = 0; i < recordList.size(); i++)
    {
        // std::cout << recordList[i].numVotes << std::endl;
        // std::cout<<recordList[i].tconst<<std::endl;
        Record newRec;
        std::copy(std::begin(recordList[i].tconst), std::end(recordList[i].tconst), std::begin(newRec.tconst));
        newRec.averageRating = recordList[i].averageRating;
        newRec.numVotes = recordList[i].numVotes;
        Address recAddress = disk.saveToDisk(&newRec, sizeof(Record));

        // if(addressList1.size()!=0 && recordCounter < 20){
        //     if((intptr_t)(void*)((char*)recAddress.blockAddress + recAddress.offset) - (intptr_t)(void*)((char*)addressList1[addressList1.size()-1].blockAddress + addressList1[addressList1.size()-1].offset)!=20){
        //         std::cout << "Counter: " << recordCounter<< std::endl;
        //         std::cout << (intptr_t)(void*)((char*)addressList1[addressList1.size()-1].blockAddress + addressList1[addressList1.size()-1].offset) << std::endl;
        //         std::cout << (intptr_t)(void*)((char*)recAddress.blockAddress + recAddress.offset) << std::endl;
        //     }
        // }
        // addressList1.push_back(recAddress);
        if (i == 0 || recordList[i].numVotes != recordList[i - 1].numVotes)
        {
            addressList.push_back(recAddress);
            keyList.push_back(newRec.numVotes);
        }
        recordCounter++;
    }
    // insert to b++tree
    for (int i = 0; i < addressList.size(); i++)
    {
        tree->insert(tree->rootNode, keyList[i], addressList[i], disk);
    }

    tree->linkLeafNodes();
    // tree->display();
    //  int nodesUpdated = 0;
    //  Address queriedAddress = tree->queryWithNumVotesAsKey(500,nodesUpdated);
    //  std::cout<<"nodes updated: "<<nodesUpdated<<std::endl;
    //  Record* record = (Record*) ((char*)queriedAddress.blockAddress + queriedAddress.offset);

    // std::cout<<"Record tconst: "<<record->tconst<<std::endl;
    // std::cout<<"Record average rating: "<<record->averageRating<<std::endl;
    // std::cout<<"Record numvotes: "<<record->numVotes<<std::endl;

    // return 0;
    std::cout << "Node Size: " << NodeSize << std::endl;

    std::cout << "Maximum keys a B++ tree can hold: " << std::endl;

    /*
    Experiment 1:
    store the data (which is about IMDb movives) on the disk and report the following statistics:
        - the number of blocks;
        - the size of database (in terms of MB);
    */

    std::cout << "--------------------------------------Experiment 1------------------------------------------" << std::endl;
    std::cout << "Number of Records                   : " << recordCounter << std::endl;
    std::cout << "Total number of blocks              : " << disk.getAllocatedUsedWithoutPadding() << std::endl;
    std::cout << "Actual size of database             : " << disk.getActualSizeUsedWithoutPadding() << std::endl;

    //     /*
    //     Experiment 2:
    //     build a B+ tree on the attribute "numVotes" by inserting the records sequentially and report the following statistics:
    //          - the parameter n of the B+ tree;
    //          - the number of nodes of the B+ tree;
    //          - the height of the B+ tree, i.e., the number of levels of the B+ tree;
    //          - the content of the root node and its 1st child node;
    //     */
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "--------------------------------------Experiment 2---------------------------------------------" << std::endl;
    tree->printBPDetails();

    std::cout << std::endl;
    std::cout << std::endl;

    // todo: check that the node address in disk is not 0 when traversing
    // /*
    // Experiment 3:
    // retrieve those movies with the “numVotes” equal to 500 and report the following statistics:
    //      - the number and the content of index nodes the process accesses;
    //      (for the content, it would be sufficient to report for the first 5 index nodes or data blocks only if there are more than 5,
    //      - the number and the content of data blocks the process accesses;
    //      - the average of “averageRating’s” of the records that are returned;

    std::cout << "-----------------------------------Experiment 3-----------------------------------------------" << std::endl;
    std::cout << "Resetting block access before carrying on with the experiment" << std::endl;

    std::cout << "Retrieving movies with 'numVotes' equal to 500: " << std::endl;
    int *result;
    result = tree->searchRange(500, 500, disk);
    std::cout << std::endl;
    std::cout << "Number of index blocks accesses      : " << *(result + 0) << std::endl;
    std::cout << "Number of record blocks accesses     : " << *(result + 1) << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    // /*
    //   Experiment 4: retrieve those movies with the attribute “numVotes” from 30,000 to 40,000,
    //   both inclusively and report the following statistics:
    //   - the number and the content of index nodes the process accesses;  report the first 5 index nodes or data blocks ONLY if there are more than 5
    //   - the number and the content of data blocks the process accesses;
    //   - the average of “averageRating’s” of the records that are returned;
    // // */
    std::cout << "-------------------------------------Experiment 4---------------------------------------------" << std::endl;
    std::cout << "Resetting block access before carrying on with the experiment" << std::endl;

    std::cout << "Retrieving those movies with the attributes 'numVotes' from 30K to 40K" << std::endl;
    std::cout << std::endl;
    int *result4;

    result4 = tree->searchRange(30000, 40000, disk);
    std::cout << "Number of index blocks accesses  : " << *(result4 + 0) << std::endl;
    std::cout << "Number of data blocks accesses   : " << *(result4 + 1) << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    // /*
    // Experiment 5:
    // delete those movies with the attribute “numVotes” equal to 1,000,
    // update the B+ tree accordingly, and report the following statistics:
    // - the number of times that a node is deleted (or two nodes are merged) during the process of the updating the B+ tree;
    // - the number nodes of the updated B+ tree;
    // - the height of the updated B+ tree;
    // - the content of the root node and its 1st child node of the updated B+ tree;
    // */

    std::cout << "-------------------------------------Experiment 5---------------------------------------------" << std::endl;
    int numNodesDeleted = 0;
    int numNodesUpdated = 0;
    int height = 0;

    tree->remove(1000, numNodesDeleted, numNodesUpdated, height, disk);

    std::cout << "Deleting those movies with the attribute 'numVotes' equal to 1000-----------------------" << std::endl;
    std::cout << "No. of times that a node is deleted (or two nodes are merged): " << numNodesDeleted << std::endl;
    std::cout << "No. of nodes in updated B+ Tree                              : " << numNodesUpdated << std::endl;
    std::cout << "Height of updated B+ tree                                    : " << height << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "--------------------------------------End--------------------------------------------------" << std::endl;

    //     //    int key_to_be_removed = 0;
    //     //    std::cout <<"Select key to delete:   "<<endl;
    //     //    std::cin >> key_to_be_removed;
    //     //    Address* addressToBeDeleted = tree.remove(key_to_be_removed,0,0,0);
    //     //    Record* tempRecord = (Record*)addressToBeDeleted;
    //     //    size_t sizeCount = 0;
    //     //    while (tempRecord->numVotes == key_to_be_removed){
    //     //        sizeCount+=sizeof(Record);
    //     //        tempRecord += sizeof(Record);
    //     //    }
    //     //    MemoryPool.deallocate(addressToBeDeleted, sizeCount);

    return 0;
}
