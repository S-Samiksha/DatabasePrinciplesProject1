
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
                parent = ((Node **)current->childrenNodes)[0];
            }else{
                parent = ((Node **)current->childrenNodes)[1];
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
            parent = ((Node **)current->childrenNodes)[current->currentPointerSize-1];
            
        }else if (key<current->keys[0]){
            parent = ((Node **)current->childrenNodes)[0];
            
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
int BPTree::searchRange(int lowKey,int highKey,MemoryPool &disk){
    //No tree -> end function
    if (!rootNode){
            std::cout<<"The B+ Tree is Empty" << std::endl;
            return 0;
    }
    else{
            std::cout<<std::endl;
            std::cout<<"Searching~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
            std::stack <Node *> stack;
            Node *current = rootNode;
            stack.push(current);
            //Traverse down the tree to find the value closest to the smaller key
            //while current node is not a leaf node
            while(!current->isLeaf){
                for(int i=0;i<current->currentKeySize;i++){
                    //go left if current key is smaller than small value
                    if(lowKey<current->keys[i]){
                        current = ((Node **)current->childrenNodes)[i];
                        std::cout<<"currently accessing " << current  <<std::endl;
                        break;
                    }
                    //else go right
                    if(i==current->currentKeySize-1){
                        current = ((Node **)current->childrenNodes)[i+1];
                        std::cout<<"currently accessing " << current  <<std::endl;
                        break;
                    }
                }
            }
            bool end = false;
            unsigned int currentKey = current->keys[0];
            int count=0;
            //reach a leaf node, start going right until number is no longer in range
            Address *start = queryWithNumVotesAsKey(currentKey,count);
            //incrementing block address along the way
            long int startingBlock =(long int) start->blockAddress;
            long int startingOffset = (long int) start->offset;
            Record* record = (Record*) (disk.loadFromDisk(*start,sizeof(Record)));
            int blockCount = 0;
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
                        std::cout<<"NumVotes for current record: " << record->numVotes  <<std::endl;
                        std::cout<<"Rating for current record: " << record->averageRating  <<std::endl;
                    }
                    //go left
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
            return blockCount;
            
    }
    


    
} 
