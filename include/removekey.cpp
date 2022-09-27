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
                //left node is null since we are concerned with left most key so it has no left neighbout 
                stack.push(NULL); 
                //push in the right neighbor of the actual node 
                stack.push(((Node **)current->childrenNodes)[1]); 
            }else{
                //set the parent as the current node 
                parent = ((Node **)current->childrenNodes)[1];
                index = 0;
                //left node is pushed into the stack 
                stack.push(((Node **)current->childrenNodes)[0]);
                //right node is pushed into the stack 
                stack.push(((Node **)current->childrenNodes)[2]);
            }
        }
        else{
            //run for loop until the correct index is found 
            for (int i =0; i<current->currentKeySize;i++){
                //if statement checks if the key is more than the left and less than the right --> stopping condition 
                if(key>=current->keys[i] && key < current->keys[i+1]){    
                    //set parent node to the current child 
                    parent = ((Node **)current->childrenNodes)[i+1];
                    //push the left node into the stack 
                    stack.push(((Node **)current->childrenNodes)[i]); 
                    stack.push(((Node **)current->childrenNodes)[i+2]); 
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
            //make space for the key and pointer in the current node
            for(int k=current->currentKeySize;k>0;k--){
                current->keys[k]=current->keys[k-1];
                ((Node **)current->childrenNodes)[k]=((Node **)current->childrenNodes)[k-1];
            }
            current->keys[0]=left->keys[left->currentKeySize-1];
            left->currentKeySize--;
            current->currentKeySize++;
            ((Node **)current->childrenNodes)[0] =((Node **)left->childrenNodes)[left->currentPointerSize-1];
            current->currentPointerSize++;
            left->currentPointerSize--;
            //update parent in the next while loop automatically

        }
        
        else if (right && right->currentKeySize > right->minkeys){
            std::cout<<"borrowing from right node..."<<std::endl;
            current->keys[current->currentKeySize]=right->keys[0];
            ((Node **)current->childrenNodes)[current->currentPointerSize]=((Node **)right->childrenNodes)[0];
            for (int k=0;k<right->currentPointerSize;k++){
                right->keys[k]=right->keys[k+1];
                ((Node **)right->childrenNodes)[k]=((Node **)right->childrenNodes)[k+1];
            }

            //update parent 

        }

        //Requires Merging --------------------------------------------
        else if (left && left->currentKeySize <= left->minkeys){
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

