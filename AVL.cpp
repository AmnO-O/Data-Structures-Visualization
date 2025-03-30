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

AVLNode* AVLtree::insert(AVLNode*& root, int key, float Ox , float Oy) {
    //if node is null insert new node 
    if (!root) {
        root = new AVLNode(key , Ox , Oy);
        subcase = 7;
        return root;
    }

    // choose subtree to insert
    if (key < root->val) {
        AVLNode* cur = root->left;
        root->left = insert(root->left, key, root->x - nodeSpacing/2.0, root->y+ nodeSpacing);
        if (cur != root->left) {
            PushAnimation(m_root,900.0, 200.0 , subcase );
            waitinganimation++;
        }   
    }
    else if (key > root->val) {
        AVLNode* cur = root->right; 
        root->right = insert(root->right, key, root->x + nodeSpacing/2.0, root->y + nodeSpacing);
        if (cur != root->right) {
            PushAnimation(m_root, 900.0, 200.0 , subcase);
            waitinganimation++;
        }
    }
    else return root;


    // update height of current node 
    root->height = 1 + max(height(root->left), height(root->right));

    // get the difference of height between left and right subtree
    int dif = difference(root);
    int leftdif = difference(root->left);
    int rightdif = difference(root->right);
    // left left problem
    if (dif > 1 && leftdif > 0) {
        subcase = 2;
        return rightRotate(root);
    }
    // right right problem
    else if (dif < -1 && rightdif < 0) {
        subcase = 4;
        return leftRotate(root);
    }
    // left right problem
    else if (dif > 1 && leftdif < 0) {
        root->left = leftRotate(root->left);
        subcase = 3;
        PushAnimation(m_root,  900.0, 200.0 ,subcase );
        waitinganimation++;
        return rightRotate(root);
    }
    //right left problem
    else if (dif < -1 && rightdif > 0) {
        root->right = rightRotate(root->right);
        subcase = 5;
        PushAnimation(m_root, 900.0, 200.0, subcase);
        waitinganimation++;
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

AVLNode* AVLtree::DeleteNode(AVLNode*& root, int key) {
    if (!root)
        return root;

    // Search for the node to delete
    if (key < root->val) {
        AVLNode* cur = root->left;
        root->left = DeleteNode(root->left, key);
        if (cur != root->left || cur->val != root->left->val) {
            PushAnimation(m_root, 900.0, 200.0, subcase);
            waitinganimation++;
        }
    }

    else if (key > root->val) {
        AVLNode* cur = root->right;
        root->right = DeleteNode(root->right, key);
        if (cur != root->right || cur->val != root->right->val) {
            PushAnimation(m_root, 900.0, 200.0, subcase);
            waitinganimation++;
        }
    }
    else {
        // Node with only one child or no child
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;

            if (!temp) {
                // No child case
                temp = root;
                root = nullptr;
            }
            else {
                // One child case
                *root = *temp; // Copy contents
            }

            delete temp;
            subcase = 7;
        }
        // Node with two children
        else {
            AVLNode* temp = root->right;
            while (temp->left)
                temp = temp->left;
            root->val = temp->val;
            root->right = DeleteNode(root->right, temp->val);
            PushAnimation(m_root, 900.0, 200.0, subcase);
            waitinganimation++;
        }
    }

    if (!root) return root; // If tree had only one node

    // Update height
    root->height = 1 + max(height(root->left), height(root->right));

    // Get balance factor
    int balance = difference(root);

    // Left-Left Case
    if (balance > 1 && difference(root->left) >= 0) {
        return rightRotate(root);
        subcase = 2;
    }

    // Left-Right Case
    if (balance > 1 && difference(root->left) < 0) {
        root->left = leftRotate(root->left);
        subcase = 3;
        PushAnimation(m_root, 900.0, 200.0, subcase);
        waitinganimation++;
        return rightRotate(root);
    }

    // Right-Right Case
    if (balance < -1 && difference(root->right) <= 0) {
        subcase = 4;

        return leftRotate(root);
    }

    // Right-Left Case
    if (balance < -1 && difference(root->right) > 0) {
        root->right = rightRotate(root->right);
        subcase = 5;
        PushAnimation(m_root, 900.0, 200.0, subcase);
        waitinganimation++;
        return leftRotate(root);
    }

    return root;
}

void AVLtree::Clear(AVLNode* root) {
    AVLNode* temp = root;
    if (!temp) return;
    if (temp->left) {
        Clear(temp->left);
        temp->left = NULL;
    }
    if (temp->right) {
        Clear(temp->right);
        temp->right = NULL;
    }
    return;
}

void AVLtree::Clear() {
    Clear(m_root);
    m_root = NULL;
}

AVLNode* AVLtree::Search(int key) {
    return Search(m_root, key);
}

void AVLtree::Insert(int key) {
    AVLNode* cur = m_root;
    m_root = insert(m_root, key, 900 , 200);
    if (cur != m_root) {
        PushAnimation(m_root, 900.0, 200.0 , subcase);
        waitinganimation++;
    }
    waitinganimation++;


}

void AVLtree::Delete(int key) {
    AVLNode* cur = m_root;
    m_root = DeleteNode(m_root, key);
    if (cur != m_root) {
        PushAnimation(m_root, 900.0, 200.0 , subcase);
        waitinganimation++;
    }
    waitinganimation++;
}

void AVLtree::PushAnimation(AVLNode* root, float target_x, float target_y , int subcase) {
    if (root == nullptr) return;
    root->queuexy.push_back({ target_x, target_y });
    root->QueueChildren.push_back({ root->left, root->right });
    if (root == m_root) mroot.push_back({ root , subcase });

    int maxleaf = root->height > 1 ? root->height - 1 : 0;
    maxleaf = 1 << maxleaf;
    if (root->left) {
        float newox = target_x - maxleaf * nodeSpacing / 2.0;
        float newoy = target_y + nodeSpacing;
        PushAnimation(root->left, newox, newoy , subcase);
    }
    if (root->right) {
        float newox = target_x + maxleaf * nodeSpacing / 2.0;
        float newoy = target_y + nodeSpacing;
        PushAnimation(root->right, newox, newoy , subcase);
    }
}

void AVLtree::UpdatePos(AVLNode* root) {
    if (root == nullptr) return;
    if (root->firstAnimationFinished) {
        root->x = root->newx;
        root->y = root->newy;
    }
    else root->firstAnimationFinished = true;
    if (!root->queuexy.empty()) {
        Vector2 nextXY = root->queuexy.front();
        root->queuexy.pop_front();
        auto x = root->QueueChildren.front();
        root->QueueChildren.pop_front();
        root->left = x.first;
        root->right = x.second;
        root->newx = nextXY.x;
        root->newy = nextXY.y;
    }
    
    if (root->left)UpdatePos(root->left);
    if (root->right)UpdatePos(root->right);
}
