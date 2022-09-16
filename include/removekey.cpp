#include "bptree.h"
#include <stdio.h>
#include <iostream>
#include "node.h"
#include <queue>
#include <vector>
#include "dummyRecord.h"
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
    std::cout<<"start"<<std::endl;
    do{
        index = -1, left = -1, right =-1;
        //check if the current node has only one key
        if (current->currentKeySize==1){
            if (key<current->keys[0]){
                parent = ((Node **)current->childrenNodes)[0];
            }else{
                parent = ((Node **)current->childrenNodes)[1];
            }
        }
        else{
            //run for loop until the correct index is found 
            for (int i =0; current->currentKeySize;i++){
                if(key>=current->keys[i] && key < current->keys[i+1]){    
                    parent = ((Node **)current->childrenNodes)[i+1];
                    break; //no point looking further 
                }
            }
        }
        
        std::cout<<"Found "<<index<< "address" << current<< std::endl;
        if (index == -1 && key>=current->keys[current->currentKeySize-1]){
            parent = ((Node **)current->childrenNodes)[current->currentPointerSize-1];
        }   
        stack.push(parent);
        current = parent;

        
    }while(!current->isLeaf);

    std::cout<<"current " << current <<std::endl;

    while (!stack.empty()){
        std::cout<<stack.top()<<" ";
        stack.pop();
    }




}