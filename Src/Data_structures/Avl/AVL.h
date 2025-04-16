
#pragma once
#include"../../Constants.h"
#include"deque"
#include <fstream>
#include <vector>
#include <iostream>
#include <stack>
#include <unordered_set>
const float nodeSpacing = 50.0f;

struct AVLNode {
    struct AnimationStep {
        Vector2 position;           // Thay thế queuexy
        std::pair<AVLNode*, AVLNode*> children; // Thay thế QueueChildren
        int value;                  // Thay thế Value
        int state;                  // Thay thế state
        int dif;
        //1 is normal 
        //2 is traversing 
        //3 is searching
    };
    int val;
    AVLNode* left;
    AVLNode* right;
    int currentstate;
    int height;
    int dif;
    bool firstAnimationFinished = false;
    float x, y, newx, newy, displayX, displayY;
    int currentAnimationNode;
    vector<AnimationStep> animationSteps;


    AVLNode(int key, float Ox, float Oy)
        : val(key), left(nullptr), right(nullptr), currentstate(1), height(1), dif(0),
        x(Ox), y(Oy + 200), newx(-1), newy(-1), displayX(Ox), displayY(Oy + 200), currentAnimationNode(-1) {
    }

    AVLNode(int key, float Ox, float Oy, int n)
        : val(key), left(nullptr), right(nullptr), currentstate(1), height(1), dif(0),
        x(Ox), y(Oy + 200), newx(-1), newy(-1), displayX(Ox), displayY(Oy + 200), currentAnimationNode(-1 - n) {
    }
    ~AVLNode() {
        if (left) delete left;
        if (right) delete right;
    }
};

class AVLtree {
public:
    int subcase;
    int currentanimation = 0;
    int previousanimation = 0;
    AVLNode* m_root = NULL;
    int height(AVLNode* node);
    AVLNode* Search(int key);
    unordered_set <AVLNode*> set;
    void Clear();
    void Insert(int key);
    void Delete(int key);
    void UpdatePos(AVLNode* root);
    vector< pair < AVLNode*, int > > mroot = { {nullptr, 0} };
    void CreateRandomAVL(int nodeCount, int minValue, int maxValue);
    void readNumbersFromFile(const std::string& filename);
    void InorderTraversal(std::vector<int>& result);
    void PreorderTraversal(std::vector<int>& result);
    void PostorderTraversal(std::vector<int>& result);
    void GetInOrder(std::vector<int>& result);
    void GetInOrderHelper(AVLNode* node, std::vector<int>& result);
    void UpdateOrderPos(int step);
    void UpdateDistance(AVLNode* root, float target_x, float target_y, int subcase);
	void UpdateValue( int value , int alterValue);
    int GetSize() const;
private:
    AVLNode* deletenode = NULL;
    void UpdateBalance(AVLNode* root);
    void Clear(AVLNode* root);
    AVLNode* DeleteNode(AVLNode*& root, int key);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insert(AVLNode*& root, int key, float Ox, float Oy);
    int difference(AVLNode* node);
    AVLNode* Search(AVLNode* root, int key);
    void PushAnimation(AVLNode* root, float target_x, float target_y, int subcase);
    void InorderHelper(AVLNode* node, std::vector<int>& result);
    void PreorderHelper(AVLNode* node, std::vector<int>& result);
    void PostorderHelper(AVLNode* node, std::vector<int>& result);
    AVLNode* insertNoAnimation(AVLNode*& root, int key, float Ox, float Oy);
    int countNodes(AVLNode* node) const;
};


