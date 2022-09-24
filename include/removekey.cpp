#include "bptree.h"
#include <stdio.h>
#include <iostream>
#include "node.h"
#include <queue>
#include <vector>
#include <stack>
#include <cmath>


void BPTree::remove(int key){
    if (!rootNode){
        std::cout<<"The B+ Tree is Empty" << std::endl;
        return;
    }

    std::cout<<"REMOVING~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
    std::stack <Node *> stack;
    Node *current = rootNode;
    Node *parent = NULL;
    int index = -1, left = -1, right =-1;
    stack.push(current);
    //start while loop
    while(!current->isLeaf){
        index = -1, left = -1, right =-1;
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
                    break; //no point looking further 
                }
            }
        }
        
        std::cout<<"Found "<<index<< " address" << current<< std::endl;
        if (index == -1 && key>=current->keys[current->currentKeySize-1]){
            parent = ((Node **)current->childrenNodes)[current->currentPointerSize-1];
        }else if (key<current->keys[0]){
            parent = ((Node **)current->childrenNodes)[0];
        }   
        stack.push(parent);
        current = parent;
        

        
    }
    //end while loop

    std::cout<<"current " << current <<std::endl;

    index = current->binarySearch(key);
    if (index == -1){
        std::cout<<"Key not found"<<std::endl;
        return;
    }

    current->remove(key);

    if (current->currentKeySize<current->minkeys){
        std::cout<<"merge keys"<<std::endl;
    }else{
        std::cout<<"No merging required"<<std::endl;
        return;
    }

    

 

}