
#include "bptree.h"
#include <stdio.h>
#include <iostream>
#include "node.h"
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
void BPTree::searchRange(int lowKey,int highKey){
    //No tree
    if (!rootNode){
            std::cout<<"The B+ Tree is Empty" << std::endl;
            return;
    }
    else{
            std::cout<<std::endl;
            std::cout<<"Searching~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
            std::stack <Node *> stack;
            Node *current = rootNode;
            stack.push(current);
            //Tranverse down the tree to find the value closest to the smaller key
            while(!current->isLeaf){
                for(int i=0;i<current->currentKeySize;i++){
                    if(lowKey<current->keys[i]){
                        current = ((Node **)current->childrenNodes)[i];
                        std::cout<<"currently accessing " << current  <<std::endl;
                        break;
                    }
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
            Address *start = queryWithNumVotesAsKey(currentKey,count);
            unsigned int startingBlock = (unsigned int)start->blockAddress;
            unsigned int startingOffset = start->offset;
            Record record = 
            // Keep accessing the key to the right, until its value is larger than the larger key
            while(!end){
                if(currentKey>highKey){
                    end = true;
                    break;
                }
                else if(currentKey<lowKey){
                    *
                }
            }
    }
    


    
} 
