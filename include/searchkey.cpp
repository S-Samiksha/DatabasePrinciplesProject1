
#include "bptree.h"
#include <stdio.h>
#include <iostream>
//#include "node.h"
#include <queue>
#include <vector>
#include <stack>
#include <cmath>
#include "memorypool.h"

//single key
void BPTree::search(int key){
if (!rootNode){
        std::cout<<"The B+ Tree is Empty" << std::endl;
        return;
    }

    std::cout<<std::endl;
    std::cout<<"Searching~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
    std::stack <Node *> stack;
    Node *current = rootNode;
    Node *parent = NULL;
    int index = -1;
    stack.push(current);
    //start while loop
    while(!current->isLeaf){
        index = -1;
        /*check if the current node has only one key
        if key is less than the value at index 0, traverse to left node 
        if key is more than the value at index 0, traverse to right node
        */
        if (current->currentKeySize==1){
            if (key<current->keys[0]){
                parent = current->childrenNodes[0].getAddressNode();
            }else{
                parent = current->childrenNodes[1].getAddressNode();
            }
        }
        else{
            //run for loop until the correct index is found 
            for (int i =0; i<current->currentKeySize;i++){
                if(key>=current->keys[i] && key < current->keys[i+1]){    
                    parent = ((Node **)current->childrenNodes)[i+1];
                    index = i;
                    break; //stop searching
                }
            }
        }
        
        std::cout<<"Found "<<index<< " address" << current<< std::endl;
        if (index == -1 && key>=current->keys[current->currentKeySize-1]){
            parent = current->childrenNodes[current->currentPointerSize-1].getAddressNode();
            
        }else if (key<current->keys[0]){
            parent = current->childrenNodes[0].getAddressNode();
            
        }   
        
        current = parent;
        

        
    }
    //end while loop

    std::cout<<"current " << current << "Index: " << index <<std::endl;

    index = current->binarySearch(key);
    if (index == -1){
        std::cout<<"Key not found"<<std::endl;
        return;
    }else{
        std::cout<<"Key Found in Node with Address: " << current << "at index: " << index <<std::endl;
    }
}

//multiple keys (TODO)
//Return the number of blocks accessed
int * BPTree::searchRange(int lowKey,int highKey,MemoryPool &disk){
    //No tree -> end function
    static int result[2];
    result[0] = 0;//index block counter
    result[1] = 0;//data block counter
    if (!rootNode){
            std::cout<<"The B+ Tree is Empty" << std::endl;
            return result;
    }
    else{
            std::cout<<std::endl;
            std::cout<<"Searching~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
            // std::stack <Node *> stack;
            Node *current = rootNode;
            // stack.push(current);
            // int indexBlockCounter = 0;
            //Traverse down the tree to find the value closest to the smaller key
            //while current node is not a leaf node
            // while(!current->isLeaf){
            //     for(int i=0;i<current->currentKeySize;i++){
            //         //go left if current key is smaller than small value
            //         if(lowKey<current->keys[i]){
            //             current = current->childrenNodes[i].getAddressNode();
            //             std::cout<<"currently accessing: " << current  <<std::endl;
            //             current->printNode(); //debug
            //             break;
            //         }
            //         //else go right
            //         if(i==current->currentKeySize-1){
            //             current = current->childrenNodes[i+1].getAddressNode();
            //             std::cout<<"currently accessing: " << current  <<std::endl; 
            //             current->printNode(); //debug
            //             break;
            //         }
            //         indexBlockCounter++;
            //     }
            // }
            bool end = false;
            unsigned int currentKey = lowKey;
            int indexNodesAcccessed=0;
            //reach a leaf node, start going right until number is no longer in range
            Address start = queryWithNumVotesAsKey(lowKey,indexNodesAcccessed);

            //incrementing block address along the way
            long int startingBlock =(long int) start.blockAddress;
            long int startingOffset = (long int) start.offset;
            Record* record = (Record*) (disk.loadFromDisk(start,sizeof(Record)));
            int blockCount = 1;
            int recordCount;
            float totalRating;
            // Keep accessing the key to the right, until its value is larger than the larger key
            while(!end){
                if(record->numVotes>highKey){
                    //end function once numVotes is higher than upper bound
                    end = true;
                    break;
                }
                else if(record->numVotes<=highKey){ 
                    //else print out the numVotes in record if numVotes of record is in range
                    if(record->numVotes>=lowKey ){
                        std::cout<<"**Record address in disk: " << &record  <<std::endl;
                        std::cout<<"NumVotes for current record: " << record->numVotes  <<std::endl;
                        std::cout<<"Average Rating for current record: " << record->averageRating  <<std::endl;
                        std::cout<<"tconst for current record: " << record->tconst  <<std::endl;

                        recordCount++;
                        totalRating+= record->averageRating;
                    }
                    //go right
                    long int newBlock = startingBlock;
                    unsigned short int newOffset;
                    if(startingOffset+sizeof(Record)>=disk.getBlockSize()){
                        newBlock += disk.getBlockSize();
                        newOffset += (startingOffset+sizeof(Record))%disk.getBlockSize();
                        blockCount++;
                    }else{
                        newOffset+=sizeof(Record);
                    }
                    Address newAddress = {(void *)newBlock,newOffset};
                    record = (Record*) (disk.loadFromDisk(newAddress,sizeof(Record)));
                }
            }

            result[0] = indexNodesAcccessed;
            result[1] = blockCount;
            std::cout<<"total number of records accessed: " << recordCount  <<std::endl;
            std::cout<<"Average avgRating of the records: " << float(totalRating/(float)recordCount)  <<std::endl;
            return result;
            
    }
    


    
}
 
