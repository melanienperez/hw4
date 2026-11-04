#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int pathHeight(Node* node);

bool equalPaths(Node * root)
{
    // Add your code below
    return pathHeight(root) != -1;


}
int pathHeight(Node* node){
    if(node == NULL){
        return 0;
    }
    int left = pathHeight(node->left); 
    int right = pathHeight(node->right);
    if( left == -1 || right == -1){
        return -1;
    }
    if (left == 0){
        return right + 1;
    }
    if(right == 0){
        return left + 1;
    }
    if( left != right){
        return -1;
    }
    return left + 1;

}

