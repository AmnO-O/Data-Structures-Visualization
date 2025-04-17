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

AVLNode* AVLtree::insert(AVLNode*& root, int key, float Ox, float Oy) {

    //if node is null insert new node 
    if (!root) {
        root = new AVLNode(key, Ox, Oy, previousanimation);
        set.insert(root);
        subcase = 7;
        return root;
    }
    root->currentstate = 2;
    PushAnimation(m_root, 900.0, 200.0, subcase);
    previousanimation++;
    // choose subtree to insert
    AVLNode* cur;
    if (key < root->val) {
        cur = root->left;
        root->left = insert(root->left, key, root->x - nodeSpacing / 2.0, root->y + nodeSpacing);
        if (cur) {
            cur->currentstate = 1;
        }
    }
    else if (key > root->val) {
        cur = root->right;
        root->right = insert(root->right, key, root->x + nodeSpacing / 2.0, root->y + nodeSpacing);
        if (cur) {
            cur->currentstate = 1;
        }
    }
    else {
        root->currentstate = 1;
        return root;
    }
    if (!root) return root;
    // update height of current node 
    root->height = 1 + max(height(root->left), height(root->right));
    // get the difference of height between left and right subtree
    root->dif = difference(root);
    int leftdif = difference(root->left);
    int rightdif = difference(root->right);
    if (cur != root->left && cur != root->right) {
        UpdateDistance(m_root, 900.0, 200.0, subcase);
    }
    else PushAnimation(m_root, 900.0, 200.0, subcase);
    previousanimation++;
    // left left problem
    if (root->dif > 1 && leftdif > 0) {
        subcase = 2;
        return rightRotate(root);
    }
    // right right problem
    else if (root->dif < -1 && rightdif < 0) {
        subcase = 4;
        return leftRotate(root);
    }
    // left right problem
    else if (root->dif > 1 && leftdif < 0) {
        root->left = leftRotate(root->left);
        subcase = 3;
        UpdateDistance(m_root, 900.0, 200.0, subcase);
        previousanimation++;
        return rightRotate(root);
    }
    //right left problem
    else if (root->dif < -1 && rightdif > 0) {
        root->right = rightRotate(root->right);
        subcase = 5;
        UpdateDistance(m_root, 900.0, 200.0, subcase);
        previousanimation++;
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
    if (!root) {
        subcase = 2;
        return NULL;
    }
    root->currentstate = root->val == key ? 3 : 2;
    PushAnimation(m_root, 900.0, 200.0, subcase);
    if (root->val == key) {
        return root;
    }
    else if (root->val > key) {
        subcase = 5;
        AVLNode* cur = Search(root->left, key);
        root->currentstate = 1;
        return cur;
    }
    else {
        subcase = 4;
        AVLNode* cur = Search(root->right, key);
        root->currentstate = 1;
        return cur;
    }
}

AVLNode* AVLtree::DeleteNode(AVLNode*& root, int key) {
    if (!root)
        return root;
    // Search for the node to delete
    AVLNode* cur;
    if (key < root->val) {
        cur = root->left;
        root->currentstate = 2;
        PushAnimation(m_root, 900.0, 200.0, subcase);
        root->left = DeleteNode(root->left, key);

    }

    else if (key > root->val) {
        cur = root->right;
        root->currentstate = 2;
        PushAnimation(m_root, 900.0, 200.0, subcase);
        root->right = DeleteNode(root->right, key);
    }
    else {
        root->currentstate = 3;
        PushAnimation(m_root, 900.0, 200.0, subcase);
        cur = root->right;
        // Node with only one child or no child
        if (!root->left || !root->right) {
            if (deletenode) {
                deletenode->val = root->val;
            }
            AVLNode* temp = root->left ? root->left : root->right;

            if (!temp) {
                // No child case
                root = nullptr;
            }
            else {
                // One child case
                root = temp; // Copy contents
            }
            subcase = 7;
        }
        // Node with two children
        else {
            deletenode = root;
            AVLNode* temp = root->right;
            while (temp->left)
                temp = temp->left;
            root->right = DeleteNode(root->right, temp->val);
        }
    }

    if (!root) return root; // If tree had only one node
    if (cur) {
        cur->currentstate = 1;
    }

    // Update height
    root->height = 1 + max(height(root->left), height(root->right));

    // Get balance factor
    int balance = difference(root);
    root->dif = balance;
    if (cur != root->left && cur != root->right) {
        UpdateDistance(m_root, 900.0, 200.0, subcase);
    }
    else PushAnimation(m_root, 900.0, 200.0, subcase);

    // Left-Left Case
    if (balance > 1 && difference(root->left) >= 0) {
        subcase = 2;
        return rightRotate(root);

    }

    // Left-Right Case
    if (balance > 1 && difference(root->left) < 0) {
        root->left = leftRotate(root->left);
        subcase = 3;
        UpdateDistance(m_root, 900.0, 200.0, subcase);

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
        UpdateDistance(m_root, 900.0, 200.0, subcase);

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
    mroot = { {nullptr, 0} };
    currentanimation = 0;
    set.clear();
}

AVLNode* AVLtree::Search(int key) {
    AVLNode* cur = Search(m_root, key);
    PushAnimation(m_root, 900.0, 200.0, subcase);
    return cur;
}

void AVLtree::UpdateBalance(AVLNode* root) {
    if (root == nullptr) return;
    if (root->left) UpdateBalance(root->left);
    if (root->right) UpdateBalance(root->right);
    root->dif = difference(root);
}

void AVLtree::Insert(int key) {
    AVLNode* cur = m_root;
    previousanimation = 0;
    m_root = insert(m_root, key, 900, 200);
    if (cur)cur->currentstate = 1;
    previousanimation = 0;
    UpdateBalance(m_root);
    UpdateDistance(m_root, 900.0, 200.0, subcase);
}

void AVLtree::Delete(int key) {
    AVLNode* cur = m_root;
    deletenode = NULL;
    m_root = DeleteNode(m_root, key);
    if (cur)cur->currentstate = 1;
    UpdateBalance(m_root);
    UpdateDistance(m_root, 900.0, 200.0, subcase);
    deletenode = NULL;

}

void AVLtree::UpdateDistance(AVLNode* root, float target_x, float target_y, int subcase) {
    if (root == nullptr) return;
    root->animationSteps.push_back({ { target_x, target_y } , { root->left, root->right } , root->val , root->currentstate , root->dif });
    if (root == m_root) mroot.push_back({ m_root , subcase });
    int maxleaf = root->height > 1 ? root->height - 2 : 0;
    maxleaf = 1 << maxleaf;
    if (root->left) {
        float newox = target_x - maxleaf * nodeSpacing / 2.0;
        float newoy = target_y + nodeSpacing;
        UpdateDistance(root->left, newox, newoy, subcase);
    }
    if (root->right) {
        float newox = target_x + maxleaf * nodeSpacing / 2.0;
        float newoy = target_y + nodeSpacing;
        UpdateDistance(root->right, newox, newoy, subcase);
    }
}
void AVLtree::PushAnimation(AVLNode* root, float target_x, float target_y, int subcase) {
    if (root == nullptr) return;
    root->animationSteps.push_back({ { target_x, target_y } , { root->left, root->right } , root->val , root->currentstate , root->dif });
    if (root == m_root) mroot.push_back({ m_root , subcase });
    if (root->left) {
        float newox, newoy;
        if (root->left->animationSteps.size() >= 1) {
            newox = root->left->animationSteps.back().position.x;
            newoy = root->left->animationSteps.back().position.y;
        }
        else
        {
            newox = target_x - nodeSpacing / 2.0;
            newoy = target_y + nodeSpacing;
        }

        PushAnimation(root->left, newox, newoy, subcase);
    }
    if (root->right) {
        float newox, newoy;
        if (root->right->animationSteps.size() >= 1) {
            newox = root->right->animationSteps.back().position.x;
            newoy = root->right->animationSteps.back().position.y;
        }
        else
        {
            newox = target_x + nodeSpacing / 2.0;
            newoy = target_y + nodeSpacing;
        }
        PushAnimation(root->right, newox, newoy, subcase);
    }
}

void AVLtree::UpdateOrderPos(int step) {
    for (auto& x : set) x->currentAnimationNode += step;
}

void AVLtree::UpdatePos(AVLNode* root) {
    if (root == nullptr) return;
    if (root->currentAnimationNode >= 0 && root->currentAnimationNode < root->animationSteps.size()) {
        auto x = root->animationSteps[root->currentAnimationNode];
        root->val = x.value;
        root->left = x.children.first;
        root->right = x.children.second;
        root->currentstate = x.state;
        root->dif = x.dif;
        if (root->newx > 0)root->x = root->newx;
        if (root->newy > 0)root->y = root->newy;
        root->newx = x.position.x;
        root->newy = x.position.y;
    }


    if (root->left) UpdatePos(root->left);
    if (root->right) UpdatePos(root->right);
}

void AVLtree::CreateRandomAVL(int nodeCount, int minValue, int maxValue) {
    Clear(); // Xóa cây hiện tại trước khi tạo mới
    std::unordered_set<int> usedValues;

    while (usedValues.size() < nodeCount) {
        int value = GetRandomValue(minValue, maxValue);
        if (usedValues.insert(value).second) { // Chỉ insert nếu chưa tồn tại
            m_root = insertNoAnimation(m_root, value, 900, 200);
        }
    }
    UpdateDistance(m_root, 900.0, 200.0, subcase);
}

void AVLtree::readNumbersFromFile(const std::string& filename) {
    Clear(); // Xóa cây hiện tại trước khi tạo mới

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file!" << std::endl;
        return;
    }

    std::unordered_set<int> usedValues;  // Dùng set để kiểm tra trùng lặp
    int value;

    while (file >> value) {
        // Kiểm tra xem giá trị đã được thêm vào chưa
        if (usedValues.insert(value).second) { // Chỉ thêm vào cây nếu giá trị chưa có trong set
            m_root = insertNoAnimation(m_root, value, 900, 200);;  // Thực hiện chèn giá trị vào cây
        }
    }
    UpdateDistance(m_root, 900.0, 200.0, subcase);

    file.close();
}

void AVLtree::InorderTraversal(std::vector<int>& result) {
    InorderHelper(m_root, result);
}

void AVLtree::PreorderTraversal(std::vector<int>& result) {
    PreorderHelper(m_root, result);
}

void AVLtree::PostorderTraversal(std::vector<int>& result) {
    PostorderHelper(m_root, result);
}

void AVLtree::InorderHelper(AVLNode* node, std::vector<int>& result) {
    if (!node) return;
    InorderHelper(node->left, result);
    result.push_back(node->val);
    InorderHelper(node->right, result);
}

void AVLtree::PreorderHelper(AVLNode* node, std::vector<int>& result) {
    if (!node) return;
    result.push_back(node->val);
    PreorderHelper(node->left, result);
    PreorderHelper(node->right, result);
}

void AVLtree::PostorderHelper(AVLNode* node, std::vector<int>& result) {
    if (!node) return;
    PostorderHelper(node->left, result);
    PostorderHelper(node->right, result);
    result.push_back(node->val);
}

void AVLtree::GetInOrder(std::vector<int>& result) {
    GetInOrderHelper(m_root, result);
}
void AVLtree::GetInOrderHelper(AVLNode* node, std::vector<int>& result) {
    if (!node) return;
    GetInOrderHelper(node->left, result);
    result.push_back(node->val);
    GetInOrderHelper(node->right, result);
}

AVLNode* AVLtree::insertNoAnimation(AVLNode*& root, int key, float Ox, float Oy) {
    //if node is null insert new node 
    if (!root) {
        root = new AVLNode(key, Ox, Oy);
        set.insert(root);
        return root;
    }
    // choose subtree to insert
    if (key < root->val)         root->left = insertNoAnimation(root->left, key, root->x - nodeSpacing / 1.0, root->y + nodeSpacing);
    else if (key > root->val)        root->right = insertNoAnimation(root->right, key, root->x + nodeSpacing / 1.0, root->y + nodeSpacing);
    else      return root;
    if (!root) return root;
    // update height of current node 
    root->height = 1 + max(height(root->left), height(root->right));
    // get the difference of height between left and right subtree
    root->dif = difference(root);
    int leftdif = difference(root->left);
    int rightdif = difference(root->right);
    // left left problem
    if (root->dif > 1 && leftdif > 0) {
        return rightRotate(root);
    }
    // right right problem
    else if (root->dif < -1 && rightdif < 0) {
        return leftRotate(root);
    }
    // left right problem
    else if (root->dif > 1 && leftdif < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    //right left problem
    else if (root->dif < -1 && rightdif > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

void AVLtree::UpdateValue(int value, int alterValue) {
    AVLNode* cur = m_root;
    AVLNode* parent = NULL;
    subcase = 1;

    // Search for the node with 'value'
    while (cur && cur->val != value) {
        if (cur->val < value) {
            subcase = 3;
            cur->currentstate = 2;
            PushAnimation(m_root, 900.0, 200.0, subcase);
            if (cur->val >= alterValue) {
                for (auto x : set) {
                    x->currentstate = 1;
                }
                subcase = 1;
                PushAnimation(m_root, 900.0, 200.0, subcase);
                return;
            }
            cur = cur->right;
        }
        else {
            subcase = 2;
            cur->currentstate = 2;
            PushAnimation(m_root, 900.0, 200.0, subcase);
            if (cur->val <= alterValue) {
                for (auto x : set) {
                    x->currentstate = 1;
                }
                subcase = 1;
                PushAnimation(m_root, 900.0, 200.0, subcase);
                return;
            }

            cur = cur->left;
        }
    }
    if (!cur) {
        subcase = 1;
        for (auto x : set) {
            x->currentstate = 1;
        }
        PushAnimation(m_root, 900.0, 200.0, subcase);
        return;
    }
    else {
        cur->currentstate = 3;
        PushAnimation(m_root, 900.0, 200.0, subcase);
    }

    AVLNode* largest = cur->left;
    subcase = 5;
    while (largest && largest->right) {
        largest->currentstate = 2;
        PushAnimation(m_root, 900.0, 200.0, subcase);
        largest = largest->right;
    }
    if (largest) {
        largest->currentstate = 2;
        PushAnimation(m_root, 900.0, 200.0, subcase);
        if (largest->val >= alterValue) {
            subcase = 1;
            for (auto x : set) {
                x->currentstate = 1;
            }
            PushAnimation(m_root, 900.0, 200.0, subcase);
            return;
        }

    }

    subcase = 4;
    AVLNode* smallest = cur->right;
    while (smallest && smallest->left) {
        smallest->currentstate = 2;
        PushAnimation(m_root, 900.0, 200.0, subcase);
        smallest = smallest->left;
    }
    if (smallest) {
        smallest->currentstate = 2;
        PushAnimation(m_root, 900.0, 200.0, subcase);
        if (smallest->val <= alterValue) {
            subcase = 1;
            for (auto x : set) {
                if (x->currentstate == 2) {
                    x->currentstate = 1;
                }
            }
            PushAnimation(m_root, 900.0, 200.0, subcase);
            return;
        }

    }

    cur->val = alterValue;
    subcase = 7;
    for (auto x : set) {
        if (x->currentstate == 2) {
            x->currentstate = 1;
        }
    }
    PushAnimation(m_root, 900.0, 200.0, subcase);
}


int AVLtree::countNodes(AVLNode* node) const {
    if (!node) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

int AVLtree::GetSize() const {
    return countNodes(m_root);
}