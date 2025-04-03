#pragma once
#include"Constants.h"
#include"deque"
#include <fstream>
#include <vector>
#include <iostream>

const float nodeSpacing = 45.0f;

struct AVLNode {
    int val;
    AVLNode* left;
    AVLNode* right;
    int height;
    bool firstAnimationFinished = false;
    float x, y, newx, newy, displayX, displayY;
    deque<Vector2> queuexy;
    deque<  pair<AVLNode*, AVLNode*  > > QueueChildren;
    AVLNode(int key) {
        val = key;
        left = NULL;
        right = NULL;
        height = 1;
        x = y = newx = newy = displayX = displayY = -1;
        firstAnimationFinished = false;
    }
    AVLNode(int key, float Ox, float Oy) {
        val = key;
        left = NULL;
        right = NULL;
        height = 1;
        newx = Ox; newy = Oy;
        x = displayX = Ox;
        y = displayY = Oy + 200;
        firstAnimationFinished = false;
    }
    ~AVLNode() {
        if (left) delete left;
        if (right) delete right;
    }
};

class AVLtree {
public:
    int subcase;
    int waitinganimation;
    AVLNode* m_root = NULL;
    int height(AVLNode* node);
    AVLNode* Search(int key);
    void Clear();
    void Insert(int key);
    void Delete(int key);
    void UpdatePos(AVLNode* root);
    deque< pair < AVLNode*, int > > mroot;
    void CreateRandomAVL(int nodeCount, int minValue, int maxValue);
    void readNumbersFromFile(const std::string& filename);
private:
    void Clear(AVLNode* root);
    AVLNode* DeleteNode(AVLNode*& root, int key);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insert(AVLNode*& root, int key, float Ox, float Oy);
    int difference(AVLNode* node);
    AVLNode* Search(AVLNode* root, int key);
    void PushAnimation(AVLNode* root, float target_x, float target_y, int subcase);

};



