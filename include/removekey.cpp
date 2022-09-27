#include "bptree.h"
#include <stdio.h>
#include <iostream>
#include "node.h"
#include <queue>
#include <vector>
#include <stack>
#include <cmath>
#include "memorypool.h"


Address* BPTree::remove(int key, int *nodesDeleted, int *nodesUpdated, int *height){

    *nodesDeleted=0; *nodesUpdated=0; *height = findHeight(rootNode);
    Address *AddressDeleted;
    //calculating the minimum number of keys for leaf and non leaf node 
    

    int minkeyLeaf, minkeyNonLeaf, minptLeaf, minptNonLeaf;
    
    minkeyLeaf = (int)floor((this->nodeSize + 1) / 2);
    minptLeaf = minkeyNonLeaf + 1;
    minkeyNonLeaf = (int)floor(this->nodeSize / 2);
    minptNonLeaf = minkeyLeaf + 1;

    //if there is no root, there is no tree
    if (!rootNode){
        std::cout<<"The B+ Tree is Empty" << std::endl;
        return NULL;
    }

    //there exists no children nodes for root nodes 
    if (rootNode->childrenNodes == 0){

        //use binary search to find the key 
        int index = rootNode->binarySearch(key);

        //if index is -1, key is not found
        if (index == -1){
            std::cout<<"Key not found!"<<std::endl;
            return NULL;
        }
        //else key is found 
        else{
            //remove the key 
            AddressDeleted = rootNode->remove(index); 
            

            //if there is nothing left in the root node remove the root node 
            if (rootNode->currentKeySize == 0){
                (*nodesDeleted)++;
                //must deallocate the block 
                MemoryPool.deallocate(rootNode, MemoryPool.blockSize);
            }
        }

    }


    //start of the removal 
    std::cout<<std::endl;
    std::cout<<"REMOVING~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;

    //declaring a stack to store left, right neighbours and then the node itself 
    std::stack <Node *> stack;
    Node *current = rootNode;
    Node *parent = NULL;

    //initialize index to -1 
    int index = -1;

    //push the root into the stack 
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
            }
            
            else{
                //set the parent as the current node 
                parent = ((Node **)current->childrenNodes)[1];
                index = 0;
                //left node is pushed into the stack 
                stack.push(((Node **)current->childrenNodes)[0]);
                //right node is pushed into the stack 
                stack.push(((Node **)current->childrenNodes)[2]);
            }
        }

        //if the node has more than one keys, must search which key and which pointer to go to 
        else{
            //run for loop until the correct index is found 
            for (int i =0; i<current->currentKeySize;i++){
                //if statement checks if the key is more than the left and less than the right --> stopping condition 
                if(key>=current->keys[i] && key < current->keys[i+1]){    
                    //set parent node to the current child 
                    parent = ((Node **)current->childrenNodes)[i+1];
                    //push the left node into the stack 
                    stack.push(((Node **)current->childrenNodes)[i]); 
                    //push the right node into the stack 
                    stack.push(((Node **)current->childrenNodes)[i+2]); 
                    index = i;
                    break; //no point looking further 
                }
            }
        }

        //if no index is found in the node such that the key is more than or equal to its left or less than its right
        //then special consideration 
        //1. key is more than all keys stored in the node 
        if (index == -1 && key>=current->keys[current->currentKeySize-1]){
            //make parent as current 
            parent = ((Node **)current->childrenNodes)[current->currentPointerSize-1];
            //push the left
            stack.push(((Node **)current->childrenNodes)[current->currentPointerSize-2]);
            //push the right since no right neighbour exists, push in NULL
            stack.push(NULL);
        }
        //2. if the key is less than all the keys stored in the node 
        else if (index == -1 && key<current->keys[0]){
            //make parent as current
            parent = ((Node **)current->childrenNodes)[0];
            //push in left, since no left neighbour exists, push in NULL
            stack.push(NULL);
            //push in right neighbor
            stack.push(((Node **)current->childrenNodes)[1]);
        }   

        //push the parent in 
        stack.push(parent);
        //make the new current the parent 
        current = parent;
        //stack has left neighbour then right neighbor then itself   
    }
    //end while loop----------------------------------------------------------------------------------------------------------




    //std::cout<<"current " << current << " Index: " << index <<std::endl;

    //Search within the leaf node 
    index = current->binarySearch(key);
    if (index == -1){
        std::cout<<"Key not found!"<<std::endl;
        return NULL;
    }
    
    //the first thing to do is remove from the leaf node
    AddressDeleted = current->remove(index);
    (*nodesDeleted)++;


    //go upwards 
    Node *left = NULL;
    Node *right = NULL;

    current = stack.top();
    stack.pop();
    int minkeys, minpt;
    while(!stack.empty()){
        //1. if the removing maintains minimum number of keys  
        if (current->isLeaf){
            minkeys = minkeyLeaf;
            minpt = minptLeaf;
        }else{
            minkeys = minkeyNonLeaf;
            minpt = minptNonLeaf;
        }
        if (current->currentKeySize >= minkeys){
            //update to the parent only if you are removing the minimum value in the node 
            if (key<current->keys[0]){
                std::cout<<"Updating parent...."<<std::endl;
                std::cout<<key<<std::endl;
                stack.push(current);
                updateParent(stack, key, nodesUpdated); 
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

    
        if (left && left->currentKeySize > minkeys){
            if (left->isLeaf){
                minkeys = minkeyLeaf;
                minpt = minptLeaf;
            }else{
                minkeys = minkeyNonLeaf;
                minpt = minptNonLeaf;
            }
            if (left->currentKeySize > minkeys){
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
            (*nodesUpdated)++;

        }
        
        
        else if (right){
            if (right->isLeaf){
                minkeys = minkeyLeaf;
                minpt = minptLeaf;
            }else{
                minkeys = minkeyNonLeaf;
                minpt = minptNonLeaf;
            }
            if (right->currentKeySize > minkeys){
            std::cout<<"borrowing from right node..."<<std::endl;
            current->keys[current->currentKeySize]=right->keys[0];
            ((Node **)current->childrenNodes)[current->currentPointerSize]=((Node **)right->childrenNodes)[0];
            for (int k=0;k<right->currentPointerSize;k++){
                right->keys[k]=right->keys[k+1];
                ((Node **)right->childrenNodes)[k]=((Node **)right->childrenNodes)[k+1];
            }
            }
            (*nodesUpdated)++;

        }

        //Requires Merging --------------------------------------------
        else if (left){

            if (left->isLeaf){
                minkeys = minkeyLeaf;
                minpt = minptLeaf;
            }else{
                minkeys = minkeyNonLeaf;
                minpt = minptNonLeaf;
            }

            if (left->currentKeySize <= minkeys){
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
            (*nodesDeleted)++;

            //must deallocate the block 
            MemoryPool.deallocate(current, MemoryPool.blockSize);
            (*nodesUpdated)++;
            
            delete current;
            current = parent;
            }
        }


        else if (right){
            if (right->isLeaf){
                minkeys = minkeyLeaf;
                minpt = minptLeaf;
            }else{
                minkeys = minkeyNonLeaf;
                minpt = minptNonLeaf;
            }
            if (right->currentKeySize<=minkeys){
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
            parent->currentPoi

            //must deallocate the block 
            MemoryPool.deallocate(rig,t MemoryPool.blockSize);nterSize--;
            current = parent;
            (*nodesDeleted)++;
            (*nodesUpdated)++;
            delete right; //delete in the parent 
            //find the pointer of the right node 
            }
        }


    }

    *height = findHeight(rootNode);

    std::cout<<"DONE REMOVING~~~~~~~~"<<std::endl<<std::endl;

    std::cout<<"Content of Parent Node:"<<std::endl:
    this->rootNode->printNode();
    Node * child = ((Node **)this->rootNode->childrenNodes)[0];
    std::cout<<"Concent of Child Node:"<<std::endl;
    child->printNode();

    return AddressDeleted;
}


//helper function to update parent all the way to the top 
void BPTree::updateParent(std::stack <Node *> stack, int key, int *nodesUpdated){

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
        
        for (int m = 0; m<parent->currentPointerSize;m++){       
            if (((Node **)parent->childrenNodes)[m] == current){
                (*nodesUpdated)++;
                parent->keys[m-1]=minimum;
            }
        }
        current = parent;
    }

   

}

bool BPT