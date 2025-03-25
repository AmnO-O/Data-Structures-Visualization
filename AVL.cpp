#include "AVL.h"

int AVLtree::height(AVLNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

AVLNode* AVLtree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* z = y->left;


    // update children of x and y 
    x->right = z;
    y->left = x;


    // update height of x 
    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

AVLNode* AVLtree::rightRotate(AVLNode* x) {

    AVLNode* y = x->left;
    AVLNode* z = y->right;


    // update children of x and y 
    x->left = z;
    y->right = x;


    // update height of x 
    x->height = 1 + max(height(x->right), height(x->left));
    y->height = 1 + max(height(y->right), height(y->left));

    return y;
}

AVLNode* AVLtree::insert(AVLNode*& root, int key) {
    //if node is null insert new node 
    if (!root) {
        root = new AVLNode(key);
        return root;
    }
    // choose subtree to insert
    if (key < root->val) root->left = insert(root->left, key);
    else if (key > root->val) root->right = insert(root->right, key);
    else return root;


    // update height of current node 
    root->height = 1 + max(height(root->left), height(root->right));

    // get the difference of height between left and right subtree
    int dif = difference(root);
    int leftdif = difference(root->left);
    int rightdif = difference(root->right);
    // left left problem
    if (dif > 1 && leftdif > 0)  return rightRotate(root);
    // right right problem
    else if (dif < -1 && rightdif < 0) return leftRotate(root);
    // left right problem
    else if (dif > 1 && leftdif < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    //right left problem
    else if (dif < -1 && rightdif > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;

}

int AVLtree::difference(AVLNode* node) {
    if (!node) return 0;
    int n = height(node->left) - height(node->right);
    return n;
};

AVLNode* AVLtree::Search(AVLNode* root, int key) {
    if (!root) return NULL;
    if (root->val == key) return root;
    if (root->val > key) return Search(root->left, key);
    else return Search(root->right, key);
}

AVLNode* AVLtree::Search(int key) {
    return Search(root, key);
}

AVLNode* AVLtree::DeleteNode(AVLNode* root, int key) {
    if (!root) return root;
    if (key < root->val) root->left = DeleteNode(root->left, key);
    else if (key > root->val) root->right = DeleteNode(root->right, key);
    else {
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = NULL;
            }
            else *root = *temp;
            delete temp;
        }
        else {
            AVLNode* temp = root->right;
            while (temp->left) temp = temp->left;
            root->val = temp->val;
            root->right = DeleteNode(root->right, temp->val);
        }
    }
    if (!root) return root;
    root->height = 1 + max(height(root->left), height(root->right));
    int dif = difference(root);
    int leftdif = difference(root->left);
    int rightdif = difference(root->right);
    if (dif > 1 && leftdif > 0) return rightRotate(root);
    else if (dif < -1 && rightdif < 0) return leftRotate(root);
    else if (dif > 1 && leftdif < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    else if (dif < -1 && rightdif > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

void AVLtree::Clear(AVLNode* root) {
    AVLNode* temp = root;
    if (!temp) return;
    if (temp->left) Clear(temp->left);
    if (temp->right) Clear(temp->right);
    delete temp;
    temp = NULL;

}