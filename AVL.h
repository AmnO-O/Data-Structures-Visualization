#pragma once
#include"Constants.h"


struct AVLNode {
    int val;
    AVLNode* left;
    AVLNode* right;
    int height;
    float x, y, newx, newy , displayX , displayY;
    AVLNode(int key) {
        val = key;
        left = NULL;
        right = NULL;
        height = 1;
        x = y = newx = newy = displayX = displayY = -1;
    }
    AVLNode(int key , float Ox , float Oy) {
        val = key;
        left = NULL;
        right = NULL;
        height = 1;
        x = newx = displayX = Ox;
        newy = Oy;
        displayY = y = Oy + 200.0;
        
    }

};

class AVLtree {
public:
    AVLNode* m_root = NULL;
    int height(AVLNode* node);
    AVLNode* Search( int key);
    void Clear();
    void Insert(int key);
    void Delete(int key);

private:
    void Clear(AVLNode* root);
    AVLNode* Delete(AVLNode* root, int key);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insert(AVLNode*& root, int key , float Ox , float Oy);
    int difference(AVLNode* node);
    AVLNode* Search(AVLNode* root, int key);
};



