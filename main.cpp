#include <iostream>
#include "include/node.h"
#include "include/bptree.h"
#include "include/memorypool.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <functional>
#include <algorithm>
int BLOCKSIZE = 100;
int MEMORYPOOLSIZE;
struct tempRecord
{
    char tconst[11];            // 10B. Primary key. Total we need is 9bits for the current data we have. + 1 bit to store the null value.

    float averageRating;        // 4B. Was thinking of using char but end up will take the same amount of byte (i.e. 4 bytes),
                                // since 2 for the numbers, 1 for the "." and 1 for the null value

    unsigned int numVotes;      // 4B. was thinking of using short unsigned, but will be limited to 65,535, which is not sufficient.
                                // Largest numVote value we have will go to a few hundred thousands.
    bool operator < (const Record& rec) const
    {
        return (numVotes < rec.numVotes);
    }

}; 
int main()
{
    BPTree *testBPTree = new BPTree(3);
    testBPTree->rootNode = new Node(3, true);
    testBPTree->insert(testBPTree->rootNode, 1, nullptr);
    testBPTree->insert(testBPTree->rootNode, 4, nullptr);
    testBPTree->insert(testBPTree->rootNode, 7, nullptr);
    testBPTree->insert(testBPTree->rootNode, 10, nullptr);
    testBPTree->insert(testBPTree->rootNode, 17, nullptr);
    testBPTree->insert(testBPTree->rootNode, 21, nullptr);
    testBPTree->insert(testBPTree->rootNode, 31, nullptr);
    testBPTree->insert(testBPTree->rootNode, 25, nullptr);
    testBPTree->insert(testBPTree->rootNode, 19, nullptr);
    testBPTree->insert(testBPTree->rootNode, 20, nullptr);
    testBPTree->insert(testBPTree->rootNode, 28, nullptr);
    testBPTree->insert(testBPTree->rootNode, 42, nullptr);
    testBPTree->insert(testBPTree->rootNode, 5, nullptr);
    testBPTree->insert(testBPTree->rootNode, 6, nullptr);
    testBPTree->insert(testBPTree->rootNode, 8, nullptr);
    testBPTree->insert(testBPTree->rootNode, 90, nullptr);
    testBPTree->insert(testBPTree->rootNode, 91, nullptr);
    testBPTree->insert(testBPTree->rootNode, 26, nullptr);
    testBPTree->insert(testBPTree->rootNode, 27, nullptr);
    testBPTree->insert(testBPTree->rootNode, 3, nullptr);
    testBPTree->insert(testBPTree->rootNode, 2, nullptr);
    testBPTree->insert(testBPTree->rootNode, 9, nullptr);
    testBPTree->insert(testBPTree->rootNode, 36, nullptr);
    testBPTree->insert(testBPTree->rootNode, 38, nullptr);
    testBPTree->linkLeafNodes();

    testBPTree->display();

    int nodeCount=0;
    Address* result = testBPTree->queryWithNumVotesAsKey(36,nodeCount);
    std::cout<< "Address of record with key value: 36 :"<<result;
//     // testBPTree->printBPDetails();
//    // testBPTree->remove(42);

//     testBPTree->display();

    //loading data
    // MemoryPool disk(MEMORYPOOLSIZE, BLOCKSIZE);  // 150MB
    // std::ifstream file("./data/data.tsv");
    // int counter = 0;
    // std::vector<tempRecord> recordList;
    // // Insert data into database
    // if(file.is_open()){
    //     std::string line;
    //     while(std::getline(file,line)){
    //         tempRecord newRec;
    //         std::stringstream datastream(line);
    //         std::string data;
    //         std::getline(datastream,data, '\t');
    //         strcpy(newRec.tconst, data.c_str());
    //         datastream >> newRec.averageRating >> newRec.numVotes;
    //         recordList.push_back(newRec);

    //     }
    // }

    // std::sort(recordList.begin(), recordList.end());

    
    // for(int i=0;i<recordList.size();i++){
    //     Record newRec{recordList[i].tconst,recordList[i].averageRating,recordList[i].numVotes};
    //     Address recAddress = disk.saveToDisk(&newRec, sizeof(Record));
        
    //     if(i==0 || recordList[i].numVotes != recordList[i-1].numVotes){
    //         //insert to b++tree
    //     }
    // }

    //Final CLI copy down here once done!!

    
    // std::cout <<"------------------------------------------------------------------------------------------------"<<endl;
    // std::cout <<"Select Block size:   "<<endl;

    // int options = 0;
    // while (options != 1 && options != 2){
    //     std::cout << "Enter a choice: " <<endl;
    //     std::cout << "1. 200 B " <<endl;
    //     std::cout << "2. 500 B" <<endl;
    //     std::cin >> options;
    //     if (int(options) == 1){
    //         BLOCKSIZE = int(200);
    //     } else if (int(options) == 2){
    //         BLOCKSIZE = int(500);
    //     }else {
    //         std::cin.clear();
    //         std::cout << "Invalid input! Only 1 and 2 are allowed. " <<endl;
    //     }
    // }

    /*
    Experiment 1:
    store the data (which is about IMDb movives) on the disk and report the following statistics:
        - the number of blocks;
        - the size of database (in terms of MB);
    */





   /*
   Experiment 2:
   build a B+ tree on the attribute "numVotes" by inserting the records sequentially and report the following statistics:
        - the parameter n of the B+ tree;
        - the number of nodes of the B+ tree;
        - the height of the B+ tree, i.e., the number of levels of the B+ tree;
        - the content of the root node and its 1st child node;
   */


   /*
   Experiment 3:
   retrieve those movies with the “numVotes” equal to 500 and report the following statistics:
        - the number and the content of index nodes the process accesses; 
        (for the content, it would be sufficient to report for the first 5 index nodes or data blocks only if there are more than 5,
        - the number and the content of data blocks the process accesses;
        - the average of “averageRating’s” of the records that are returned;

   */

  /*
    Experiment 4: retrieve those movies with the attribute “numVotes” from 30,000 to 40,000, 
    both inclusively and report the following statistics:
    - the number and the content of index nodes the process accesses;
    - the number and the content of data blocks the process accesses;
    - the average of “averageRating’s” of the records that are returned;
  */


    /*
    Experiment 5:
    delete those movies with the attribute “numVotes” equal to 1,000, 
    update the B+ tree accordingly, and report the following statistics:
    - the number of times that a node is deleted (or two nodes are merged) during the process of the updating the B+ tree;
    - the number nodes of the updated B+ tree;
    - the height of the updated B+ tree;
    - the content of the root node and its 1st child node of the updated B+ tree; 
    */

    return 0;
}