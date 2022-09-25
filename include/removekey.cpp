#include "bptree.h"
#include <stdio.h>
#include <iostream>
#include "node.h"
#include <queue>
#include <vector>
#include <stack>
#include <cmath>


void BPTree::remove(int key){
    
    //if there is no root, there is no tree
    if (!rootNode){
        std::cout<<"The B+ Tree is Empty" << std::endl;
        return;
    }

    //start of the removal 
    std::cout<<std::endl;
    std::cout<<"REMOVING~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
    std::stack <Node *> stack;
    Node *current = rootNode;
    Node *parent = NULL;
    int index = -1;
    stack.push(current);
    //start while loop-----------------------------------------------------------------------------------------------------------
    while(!current->isLeaf){
        index = -1;
        /*check if the current node has only one key
        if key is less than the value at index 0, traverse to left node 
        if key is more than the value at index 0, traverse to right node
        */
        if (current->currentKeySize==1){
            if (key<current->keys[0]){
                parent = ((Node **)current->childrenNodes)[0];
                index = 0;
                stack.push(NULL); //left 
                stack.push(((Node **)current->childrenNodes)[1]); //right 
            }else{
                parent = ((Node **)current->childrenNodes)[1];
                index = 0;
                stack.push(((Node **)current->childrenNodes)[0]);
                stack.push(((Node **)current->childrenNodes)[2]);
            }
        }
        else{
            //run for loop until the correct index is found 
            for (int i =0; i<current->currentKeySize;i++){
                if(key>=current->keys[i] && key < current->keys[i+1]){    
                    parent = ((Node **)current->childrenNodes)[i+1];
                    stack.push(((Node **)current->childrenNodes)[i]); //left
                    stack.push(((Node **)current->childrenNodes)[i+2]); //right
                    index = i;
                    break; //no point looking further 
                }
            }
        }
        
        std::cout<<"Found "<<index<< " address" << current<< std::endl;
        if (index == -1 && key>=current->keys[current->currentKeySize-1]){
            parent = ((Node **)current->childrenNodes)[current->currentPointerSize-1];
            stack.push(((Node **)current->childrenNodes)[current->currentPointerSize-2]);
            stack.push(NULL);
        }else if (index == -1 && key<current->keys[0]){
            parent = ((Node **)current->childrenNodes)[0];
            stack.push(NULL);
            stack.push(((Node **)current->childrenNodes)[1]);
        }   
        stack.push(parent);
        current = parent;
        //stack has left neighbour then right neighbor then itself   
    }
    //end while loop----------------------------------------------------------------------------------------------------------




    std::cout<<"current " << current << " Index: " << index <<std::endl;
    //Search within the leaf node 
    index = current->binarySearch(key);
    if (index == -1){
        std::cout<<"Key not found!"<<std::endl;
        return;
    }


    //the first thing to do is remove from the leaf node
    current->remove(index);


    //go upwards 
    Node *left = NULL;
    Node *right = NULL;

    current = stack.top();
    stack.pop();
    while(!stack.empty()){
        //1. if the removing maintains minimum number of keys  
        if (current->currentKeySize>=current->minkeys){
            //update to the parent only if you are removing the minimum value in the node 
            if (key<current->keys[0]){
                std::cout<<"Updating parent...."<<std::endl;
                std::cout<<key<<std::endl;
                stack.push(current);
                updateParent(stack, key); 
                break;
            }
            break;
        }
        //else if we need to check if we can borrow from left or right neight 
        //std::cout<<stack.size()<<std::endl;
        
        right = stack.top();
        stack.pop();
        left = stack.top();
        stack.pop();
        //Borrowing NO MERGING ------------------------------------------
        if (left && left->currentKeySize > left->minkeys){
            std::cout<<"borrowing from left node..."<<std::endl;
            //update parent 
        }
        
        else if (right && right->currentKeySize > right->minkeys){
            std::cout<<"borrowing from right node..."<<std::endl;
            //update parent 
        }

        //Requires Merging --------------------------------------------
        if (left && left->currentKeySize <= left->minkeys){
            std::cout<<"Merging with left node..."<<std::endl;
            int i=0;
            //transfeer all to the left node
            for (int k=left->currentKeySize;k<left->maxKeySize;k++){
                left->keys[k]=current->keys[i];
                current->keys[i]=0;
                i++;
            }
            //since we are merging with left, only removal of the pointer and key is necessary from the parent 
            parent = stack.top();
            stack.pop();
            for(int m=0;m<parent->currentPointerSize;m++){
                if (((Node **)parent->childrenNodes)[m] == current){
                    for(int j=m;j<parent->currentPointerSize;j++){
                       ((Node **)parent->childrenNodes)[j]=((Node **)parent->childrenNodes)[j+1];
                       parent->keys[j-1]=parent->keys[j];
                    }
                    break; //remove the pointers 
                }
            }
            parent->currentKeySize--;
            parent->currentPointerSize--;
            delete current;
            current = parent;
        }


        else if (right && right->currentKeySize <= right->minkeys){
            std::cout<<"Merging with right node..."<<std::endl;
            //transfer all the keys to the current node
            int i=0;
            for(int k=current->currentKeySize;k<current->maxKeySize;k++){
                current->keys[k]=right->keys[i];
                right->keys[i]=0;
                i++;
            }
            parent = stack.top();
            stack.pop();
            //1.traverse parent childrennodes --> remove the address 
            //2.remove the key as well 
            //3. it is the current node must update the key for that 
            for(int m=0;m<parent->currentPointerSize;m++){
                if (((Node **)parent->childrenNodes)[m] == right){
                    for(int j=m;j<parent->currentPointerSize;j++){
                       ((Node **)parent->childrenNodes)[j]=((Node **)parent->childrenNodes)[j+1];
                       parent->keys[j-1]=parent->keys[j];
                    }
                    break; //remove the pointers 
                }
                
                if (((Node **)parent->childrenNodes)[m] == current){
                    parent->keys[m-1]=findMinimumKeyInBPTree(current);
                }
            }
            parent->currentKeySize--;
            parent->currentPointerSize--;
            current = parent;
            delete right; //delete in the parent 
            //find the pointer of the right node 
        }


    }


    std::cout<<"DONE REMOVING~~~~~~~~"<<std::endl<<std::endl;
}


//helper function to update parent all the way to the top 
void BPTree::updateParent(std::stack <Node *> stack, int key){

    Node* parent = NULL;
    Node* current = NULL;
    Node* left = NULL;
    Node* right = NULL;
    int index = -1, minimum = 9999;

    current = stack.top();
    stack.pop();
    while(!stack.empty()){
        stack.pop(); //right
        stack.pop(); //left
        parent = stack.top(); //parent 
        stack.pop();
        //within the parent must update the key only if it comes from the right subtree
        index = -1;
        minimum = findMinimumKeyInBPTree(current);
        std::cout<<"parent: "<<parent<<" current: " << current <<" minimum: "<<minimum<<std::endl;
        for (int m = 0; m<parent->currentPointerSize;m++){       
            if (((Node **)parent->childrenNodes)[m] == current){
                std::cout<<"debug 1: "<< m <<std::endl;
                parent->keys[m-1]=minimum;
            }
        }
        current = parent;
    }
    std::cout<<minimum<<std::endl;
    return;
}

