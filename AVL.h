#pragma once
#include"Constants.h"


struct AVLNode {
    int val;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(int key) {
        val = key;
        left = NULL;
        right = NULL;
        height = 1;
    }
};

class AVLtree {
public:
    AVLNode* root = NULL;
    int height(AVLNode* node);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insert(AVLNode*& root, int key);
    int difference(AVLNode* node);
    AVLNode* Search( int key);
    AVLNode* Search(AVLNode* root, int key);
    AVLNode* DeleteNode(AVLNode* root, int key);
    void Clear(AVLNode* root);
private:

};



