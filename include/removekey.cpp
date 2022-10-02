#include "bptree.h"
#include <stdio.h>
#include <iostream>
//#include "node.h"
#include <queue>
#include <vector>
#include <stack>
#include <cmath>
#include "memorypool.h"



void BPTree::remove(int key, int &nodesDeleted, int &nodesUpdated, int &height, MemoryPool &disk){
    
    nodesDeleted = 0; nodesUpdated = 0;
    
    height = findHeight(rootNode);
    Address AddressDeleted;
    //calculating the minimum number of keys for leaf and non leaf node 
    int minkeyLeaf, minkeyNonLeaf, minptLeaf, minptNonLeaf;
    
    minkeyLeaf = (int)floor((this->nodeSize + 1) / 2);
    minptLeaf = minkeyLeaf + 1;
    minkeyNonLeaf = (int)floor(this->nodeSize / 2);
    minptNonLeaf = minkeyNonLeaf + 1;
   
    
    //if there is no root, there is no tree
    if (!rootNode){
        std::cout<<"The B+ Tree is Empty" << std::endl;
        return;
    }

    //if root is the leaf
    if (rootNode->isLeaf){

        //use binary search to find the key 
        int index = rootNode->binarySearch(key);

        //if index is -1, key is not found
        if (index == -1){
            std::cout<<"Key not found!"<<std::endl;
            return;
        }
        //else key is found 
        else{

            //remove the key 
            AddressDeleted = rootNode->remove(index); 
            
            //if there is nothing left in the root node remove the root node 
            if (rootNode->currentKeySize == 0){
                
                //increment the nodesDeleted
                nodesDeleted++;

                //must deallocate the block that holds the root node 
                disk.deallocate(rootNode->addressInDisk, disk.getBlockSize());
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
                //must get the address of the node for the parent 
                parent = (current->childrenNodes[0]).getAddressNode();

                //set index to 0 since we found it in the first index
                index = 0;

                //left node is null since we are concerned with left most key so it has no left neighbour
                stack.push(NULL); 

                //push in the right neighbor of the actual node 
                stack.push(current->childrenNodes[1].getAddressNode());
            }
            
            else{
                //set the parent as the current node 
                parent = (current->childrenNodes[1]).getAddressNode();

                //set index to null since we found it at the first index 
                index = 0;

                //left node is pushed into the stack 
                stack.push((current->childrenNodes[0]).getAddressNode());

                //right node is pushed into the stack 
                stack.push(NULL);
            }
        }

        //if the node has more than one keys, must search which key and which pointer to go to 
        else{

            //run for loop until the correct index is found 
            if (!current->isLeaf){
            for (int i =0; i<current->currentKeySize;i++){

                //if statement checks if the key is more than the left and less than the right --> stopping condition 
                
                if(key> current->keys[i] && key <= current->keys[i+1]){   

                    //set parent node to the current child 
                    parent = (current->childrenNodes[i+1]).getAddressNode();

                    //push the left node into the stack 
                    stack.push((current->childrenNodes[i]).getAddressNode()); 

                    //push the right node into the stack 
                    stack.push((current->childrenNodes[i+2]).getAddressNode()); 

                    //set index to the i value
                    index = i;

                    break; //no point looking further 
                }
            }
            }
            else{
                index = current->binarySearch(key);
                stack.push(current->childrenNodes[index-1].getAddressNode()); //left
                stack.push(current->childrenNodes[index+1].getAddressNode()); //right
                parent = current->childrenNodes[index].getAddressNode();
                break;
            }

        }

        //if no index is found in the node such that the key is more than or equal to its left or less than its right
        //then special consideration 
        //1. key is more than all keys stored in the node 

        if (index == -1 && key>=current->keys[current->currentKeySize-1]){

            //make parent as current 
            parent = (current->childrenNodes[current->currentPointerSize-1]).getAddressNode();

            //push the left
            stack.push((current->childrenNodes[current->currentPointerSize-2]).getAddressNode());

            //push the right since no right neighbour exists, push in NULL
            stack.push(NULL);
        }

        //2. if the key is less than all the keys stored in the node 
        else if (index == -1 && key<current->keys[0]){

            //make parent as current
            parent = (current->childrenNodes[0]).getAddressNode();

            //push in left, since no left neighbour exists, push in NULL
            stack.push(NULL);

            //push in right neighbor
            stack.push((current->childrenNodes[1]).getAddressNode());
        }   

        stack.push(parent);

        //make the new current the parent 
        current = parent;

        //stack has left neighbour then right neighbor then itself   
    }
    //end while loop----------------------------------------------------------------------------------------------------------


    //Search within the leaf node 
    index = current->binarySearch(key);

    //if the binary search results in -1, there is not key found
    if (index == -1){
        std::cout<<"Key not found!"<<std::endl;
        return;
    }
    
    //the first thing to do is remove from the leaf node
    AddressDeleted = current->remove(index);

    //update the nodesDeleted 
    //nodesDeleted++;


    //go upwards 
    Node *left = NULL;
    Node *right = NULL;
    //Node *parent = NULL;

    current = stack.top();
    stack.pop();
   // current->printNode();
   

    int minkeys, minpt;

    //loop through the stack to traverse up the B+ tree
    while(!stack.empty()){

        //1. if the removing maintains minimum number of keys  
        //calculate the minimum number of keys for the current node 
        if (current->isLeaf){
            minkeys = minkeyLeaf;
            minpt = minptLeaf;
        }else{
            minkeys = minkeyNonLeaf;
            minpt = minptNonLeaf;
        }

        //if current node has sufficient keys, simply remove key 
        //and continue to update parents as no further merging is required
        if (current->currentKeySize >= minkeys || current == rootNode ){

            //update to the parent only if you are removing the minimum value in the node 
            if (key<current->keys[0]){
                std::cout<<"updating parent ..."<<std::endl;
                current->printNode();
                stack.push(current);
                updateParent(stack, key, nodesUpdated); 
                std::cout<<"updating parent"<<std::endl;
                
                break;
            }
            else if (current == rootNode){
                stack.push(current);
                updateParent(stack, key, nodesUpdated);
            }

            break;
        }
        //else if we need to check if we can borrow from left or right neighbour
        
        //right neighbour is on top of left neighbour 
        //since left was pushed before right 
        if (!stack.empty()){
            
        right = stack.top();
        stack.pop();
        }else{
            right = NULL;
            break;
        }
        if (!stack.empty()){
        left = stack.top();
        stack.pop();
        }else{
            left = NULL;
            break;
        }

        
        //Borrowing NO MERGING ------------------------------------------

        //if left neighbour exists and if it has enough number of keys 
        if (left){

            //recalculate the minimum number of keys and pointers the node can hold 
            if (left->isLeaf){
                minkeys = minkeyLeaf;
                minpt = minptLeaf;
            }else{
                minkeys = minkeyNonLeaf;
                minpt = minptNonLeaf;
            }

            //if it is sufficient proceed onto merging current and left 
            if (left->currentKeySize > minkeys && current->currentKeySize < current->maxKeySize){
                std::cout<<"borrowing from left"<<std::endl;
                //make space for the key and pointer in the current node
                //left current 
                if (!left->isLeaf){
                    for(int k=current->currentKeySize-1;k>0;k--){

                        //move keys to the right to make space 
                        current->keys[k]=current->keys[k-1];

                        //move pointers to the right to make space 
                    }

                    for (int k=current->currentPointerSize-1;k>0;k--){
                        current->childrenNodes[k]=current->childrenNodes[k-1];
                    }
                }
                else{
                    for (int k=current->currentKeySize;k>0;k--){
                        current->keys[k] = current->keys[k-1];
                        current->childrenNodes[k] = current->childrenNodes[k-1];
                    }
                }
                


                
                //move the key from left to current 
                //left | current 
                //last of the left and push it to current 
                current->keys[0]=left->keys[left->currentKeySize-1];
                left->keys[left->currentKeySize-1]=0;
                current->childrenNodes[0] = left->childrenNodes[left->currentPointerSize-1];
                left->childrenNodes[left->currentPointerSize-1] = {nullptr, 0};

                //update the sizes of the nodes 
                left->currentKeySize--;
                current->currentKeySize++;

                

                //update the sizes of the pointer array 
                current->currentPointerSize++;
                left->currentPointerSize--;

            }
            nodesUpdated++;
            //skip all the rest of the conditions 

        }
        
        //else check the right node to see if it exists 
        if (right){

            //calculate the minimum number keys for the right node 
            if (right->isLeaf){
                minkeys = minkeyLeaf;
                minpt = minptLeaf;
            }else{
                minkeys = minkeyNonLeaf;
                minpt = minptNonLeaf;
            }
            //if right has sufficient nodes simply borrow from the left
            right->printNode();
            current->printNode();
            if (right->currentKeySize > minkeys){
            std::cout<<"Borrowing from right"<<std::endl;
            current->keys[current->currentKeySize]=right->keys[0];
            current->childrenNodes[current->currentPointerSize]=right->childrenNodes[0];

                if (!right->isLeaf){
                for (int k=0;k<right->currentKeySize-1;k++){
                    right->keys[k]=right->keys[k+1];
                    
                }
                for (int k=0;k<right->currentPointerSize-1;k++){
                    
                    right->childrenNodes[k]=right->childrenNodes[k+1];
                }
                }
                else{
                    for (int k=0;k<right->currentKeySize;k++){
                        right->keys[k] = right->keys[k+1];
                        right->childrenNodes[k] = right->childrenNodes[k+1];
                    }
                }

            right->keys[right->currentKeySize-1]=0;
            right->childrenNodes[right->currentPointerSize-1]={nullptr, 0};
            right->currentKeySize--;
            right->currentPointerSize--;
            current->currentKeySize++;
            current->currentPointerSize++;


            }
            nodesUpdated++;
            //skip all the rest of the conditions 
           
        }

        //Requires Merging --------------------------------------------

        //if left node exists, merge with left
        if (left){

            //calculate the minimum number of keys for the left 
            if (left->isLeaf){
                minkeys = minkeyLeaf;
                minpt = minptLeaf;
            }else{
                minkeys = minkeyNonLeaf;
                minpt = minptNonLeaf;
            }

            //if left has less than the minkeys then merge
            if (left->currentKeySize <= minkeys && left->currentKeySize+current->currentKeySize <= left->maxKeySize){
            std::cout<<"merging with left"<<std::endl;
            int i=0;

            //transfer all to the left node
            for (int k=left->currentKeySize;k<left->maxKeySize;k++){
                left->keys[k]=current->keys[i];
                current->keys[i]=0;
                i++;
            }

            std::cout<<"after transferring to left"<<std::endl;


            //since we are merging with left, only removal of the pointer and key is necessary from the parent 
            parent = stack.top();
            stack.pop();

            for(int m=0;m<parent->currentPointerSize;m++){

                //find the current node in the parent's childrennode array to update it
                //and update the keys 
                if (parent->childrenNodes[m].getAddressNode() == current){

                    //once found update everything to the left of it 
                    for(int j=m;j<parent->currentPointerSize;j++){
                       parent->childrenNodes[j]=parent->childrenNodes[j+1];
                       parent->keys[j-1]=parent->keys[j];
                    }
                    break; //remove the pointers 
                }
            }
            std::cout<<"after updating parent"<<std::endl;
            //update the key and pointer sizes 
            parent->currentKeySize--;
            parent->currentPointerSize--;
            nodesDeleted++;
            nodesUpdated++;

            //must deallocate the block 
            disk.deallocate(current->addressInDisk, disk.getBlockSize());
            //stack.push(parent);
            //updateParent(stack, key, nodesUpdated);

           
            //delete current;

            current = parent;
            }
        }

        //if left node does not exist must check if right node exists 
        else if (right){

            //calculate the minimum number of keys for the nodes 
            if (right->isLeaf){
                minkeys = minkeyLeaf;
                minpt = minptLeaf;
            }else{
                minkeys = minkeyNonLeaf;
                minpt = minptNonLeaf;
            }

            //if the right has less than the minimum number of keys, merge with the right 
            if (right->currentKeySize<=minkeys && right->currentKeySize+current->currentKeySize <= current->maxKeySize){

            std::cout<<"merging with right"<<std::endl;

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

                //find the right node in the parent of childrenNode of the parent 
                if (parent->childrenNodes[m].getAddressNode() == right){

                    //once found, move everything to the right 
                    for(int j=m;j<parent->currentPointerSize;j++){
                       parent->childrenNodes[j]=parent->childrenNodes[j+1];
                       parent->keys[j-1]=parent->keys[j];
                    }

                    break; //remove the pointers 
                }
                
                //since we are merging with the right it is important to update the parent 
                if (parent->childrenNodes[m].getAddressNode() == current){
                    parent->keys[m-1]=findMinimumKeyInBPTree(current);
                }
            }

            //update the parents key and pointer size 
            parent->currentKeySize--;
            parent->currentPointerSize--;

            //must deallocate the block 
            current = parent;
            nodesDeleted++;
            nodesUpdated++;
            
            disk.deallocate(right->addressInDisk, disk.getBlockSize());
           // delete right; //delete in the parent 
            //find the pointer of the right node 
            }
        }


    }




    std::cout<<"DONE REMOVING~~~~~~~~"<<std::endl<<std::endl;
    height = findHeight(rootNode);
    std::cout<<"Content of Parent Node:"<<std::endl;
    this->rootNode->printNode();
    Node * child = this->rootNode->childrenNodes[0].getAddressNode();
    std::cout<<"Concent of Child Node:"<<std::endl;
    child->printNode();
    std::cout<<"here: " << AddressDeleted.blockAddress <<std::endl;
    disk.deallocate(AddressDeleted, disk.getBlockSize());
    return;
}


//helper function to update parent all the way to the top 
void BPTree::updateParent(std::stack <Node *> stack, int key, int &nodesUpdated){

    Node* parent = NULL;
    Node* current = NULL;
    Node* left = NULL;
    Node* right = NULL;
    int index = -1, minimum = 9999;
    current = stack.top();
    stack.pop();
    //current->printNode();
    while(!stack.empty()){
        stack.pop(); //right
        stack.pop(); //left
        parent = stack.top(); //parent 
        stack.pop();
        //within the parent must update the key only if it comes from the right subtree
        index = -1;

        //find the minimum key in the sub trees
        minimum = findMinimumKeyInBPTree(current);  
        
        for (int m = 0; m<parent->currentPointerSize;m++){

            //when the current equals the index of childrenNode array then update the key at the key array 
            if (parent->childrenNodes[m].getAddressNode() == current){ 
                nodesUpdated++;
                parent->keys[m-1]=minimum;
            }
        }

        //set current as parent since we are moving upwards 
        current = parent;

    }

   

}
