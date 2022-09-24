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

    std::cout<<std::endl;
    std::cout<<"REMOVING~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
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
                stack.push(NULL);
                stack.push(((Node **)current->childrenNodes)[1]);
            }else{
                parent = ((Node **)current->childrenNodes)[1];
                
                stack.push(((Node **)current->childrenNodes)[0]);
                stack.push(((Node **)current->childrenNodes)[2]);
            }
        }
        else{
            //run for loop until the correct index is found 
            for (int i =0; i<current->currentKeySize;i++){
                if(key>=current->keys[i] && key < current->keys[i+1]){    
                    parent = ((Node **)current->childrenNodes)[i+1];
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
        }else if (key<current->keys[0]){
            parent = ((Node **)current->childrenNodes)[0];
            stack.push(NULL);
            stack.push(((Node **)current->childrenNodes)[1]);
        }   
        stack.push(parent);
        current = parent;
        

        
    }
    //end while loop

    std::cout<<"current " << current << "Index: " << index <<std::endl;

    index = current->binarySearch(key);
    if (index == -1){
        std::cout<<"Key not found"<<std::endl;
        return;
    }

    current->remove(index);

    if (current == rootNode){
        for (int j=index;j<current->currentKeySize;j++){
            current->keys[j]=current->keys[j+1];
        }
        current->keys[current->currentKeySize]=0;
        if (current->currentKeySize==0){
            std::cout<<"Tree is now empty!"<<std::endl;
            //TODO: needs to be checked for bugs 
            //TODO: delete and deallocate

        }
    }


    if (current->currentKeySize<current->minkeys){
        std::cout<<"Borrowing/Merging needed"<<std::endl;
    } else{
        //TODO: Needs to be checked for bugs 
        std::cout<<"No merging required"<<std::endl;
        for (int j=index; j<current->currentKeySize;j++){
            current->keys[j]=current->keys[j+1];
        }
        current->keys[current->currentKeySize]=0;
        return;
    }

    //Checking if left and right neighbours can provide any keys (must check both neighbours)
    Node* LeafNode = stack.top();
    stack.pop();
    Node* right = stack.top();
    stack.pop();
    Node* left = stack.top();
    stack.pop();
    parent = stack.top();
    stack.pop();
    std::cout<<"left: "<< left << "right: "<< right << "current: " << current <<std::endl;
    
    if (left && left->currentKeySize>left->minkeys){
        //TODO: borrow from left shifting the pointers , the pointers are giving errors not so sure why
        
        // push to the left  
        for (int j= LeafNode->currentKeySize;j>0;j--){
            LeafNode->keys[j] = LeafNode->keys[j-1];
            
        }

        LeafNode->keys[0] = left->keys[left->currentKeySize-1];
        left->currentKeySize--;
        LeafNode->currentKeySize++;
        
        parent->keys[index]=LeafNode->keys[0];

        return;


    }else if (right && right->currentKeySize>right->minkeys){
        //TODO: borrow from right
        
        LeafNode->keys[LeafNode->currentKeySize] = right->keys[0];
        LeafNode->currentKeySize++;
        right->currentKeySize--;

        for (int j=0;j<right->currentKeySize;j++){
            right->keys[j]=right->keys[j+1];
        }
        parent->keys[index]=right->keys[0];
        return;
        
    }

    //call merge function 
    std::cout<<"calling merge function"<<std::endl;
    //merge function needs to keep track of left and right children cannot just merge
    




   


    std::cout<<"DONE REMOVING~~~~~~~~"<<std::endl<<std::endl;

    

}