#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#if 0
class RBTree
{
    enum Color
    {
        R = 0,
        B
    };

    struct Node
    {
        Node(int data)
        {
            this->data = data;    
            color = R;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
        }

        int data;
        Color color;

        Node *left;
        Node *right;
        Node *parent;
    };

public:
    RBTree()
    {
        root = nullptr;
    }

    Node *InitRoot(int data)
    {
        root = new Node(data);
        return root;
    }

    void insert(Node *root, int data)
    {
        if (root->data < data)               //insert right
        {
            if (root->right == nullptr)
            {
                root->right = new Node(data);
            }
            else
            {
                insert(root->right, data);
            }
        }
        else
        {
            if (root->left == nullptr)
            {
                root->left = new Node(data);
            }
            else
            {
                insert(root->left, data);
            }
        }
    }

    void LeftRotate(Node *node)
    {
        //第一种是node没有父节点的时候那么只需要操作两个点
        //node是root节点
        if (node->parent == nullptr)
        {
            Node *E = root;              //先使用默认的拷贝构造函数
            Node *S = E->right;

            //移S的左子树
            E->right = S->left;
            S->left->parent = E;

            //修改E的指针
            E->parent = S;

            //修改S的指针
            S->parent = nullptr;
        }
        else
        {
            //如果有父节点，那么就需要操作三个点
            if (node == node->parent->left)
            {
                node->parent->left = node->right;
            }
            else
            {
                node->parent->right = node->right;
            }

            node->right = node->right->left;    //S点子树指针挂到了E点上面
            node->right->left->parent = node;   //S点原来的左子树会挂到E上面

            node->right->parent = node->parent;  //S点上浮变成E的父节点
            node->parent = node->right;          //E点的parent会变成原来的S点

            node->parent->left = node;           //修改S节点的左子树指针 
        }
    }

private:
    Node *root;
};
#endif

#define NULL    ((void *)0)

enum Color
{
    RED,
    BLACK
};

template<typename K, typename V>
struct Node
{
    K key;
    V val;
    int N;
    Color color;
    Node<K,V>* lNode;
    Node<K,V>* rNode;

    Node()
    {
        this->lNode = NULL;
        this->rNode = NULL;
    }

    Node(K key, V val)
    {
        this->key = key;
        this->val = val;
        this->N = 1;
        this->color = RED;
        this->lNode = NULL;
        this->rNode = NULL;
    }
};

template<typename K, typename V>
class RBT
{
public:
    RBT()
    {
        this->root = NULL;
    }

    ~RBT()
    {
        clean(this->root);
    }

    V get(const K& key) const
    {
        return get(this->root, key);
    }

    void put(const K& key, const V& val)
    {
        this->root = put(this->root, key, val);
    }

    void del(const K& key)
    {
        this->root = del(this->root, key);
    }

    bool contain(const K& key) const;

    int size() const
    {
        return size(this->root);
    }
    
    void clean()
    {
        clean(this->root);
        this->root = NULL;
    }

    void print() const
    {
        print(this->root);
    }

    K minKey() const
    {
        return minKey(this->root);
    }

    void delMin()
    {
        this->root = delMin(this->root);
    }

    void delMax()
    {
        this->root = delMax(this->root);
    }

    int countLevel() const
    {
        return countLevel(this->root);
    }
protected:
    Node<K,V>* root;

    Node<K,V>* rotateLeft(Node<K,V>* node);
    Node<K,V>* rotateRight(Node<K,V>* node);
    void filpNode(Node<K,V>* node);
    void reverseFlipNode(Node<K,V>* node);
    Node<K,V>* fixNode(Node<K,V>* node);

    Node<K,V>* put(Node<K,V>* node, const K& key, const V& val)
    {
        if (node == NULL)
            return new Node<K,V>(key, val);
        if (key > node->key)
            node->rNode = put(node->rNode, key, val);
        else if (key < node->key)
            node->lNode = put(node->lNode, key, val);
        else
            node->val = val;

        if (isRed(node->rNode) && !isRed(node->lNode))
            node = rotateLeft(node);
        if (node->lNode != NULL && isRed(node->lNode) && isRed(node->lNode->lNode))
            node = rotateRight(node);
        if (isRed(node->lNode) && isRed(node->rNode))
            filpNode(node);

        node->N = size(node->lNode) + size(node->rNode) + 1;
        return node;
    }

    V get(Node<K,V>* node, const K& key) const
    {
        if (node == NULL)
            return NULL;
        if (key > node->key)
            return get(node->rNode, key);
        else if (key < node->key)
            return get(node->lNode, key);
        else
            return node->val;
    }

    bool contain(Node<K,V>* node, const K& key) const;
    Node<K,V>* del(Node<K,V>* node, const K& key);

    void clean(Node<K,V>* node)
    {
        if (node == NULL)
            return ;
        clean(node->lNode);
        clean(node->rNode);
        delete node;
    }

    int size(Node<K,V>* node) const
    {
        if (node == NULL)
            return 0;
        return size(node->rNode) + size(node->lNode) + 1;
    }

    void print(Node<K,V>* node) const
    {
        if (node == NULL)
            return ;
        print(node->lNode);
        std::cout << node->key << " : " << node->val << "  :  " << node->N << "  :  " << node->color << std::endl;
        print(node->rNode);
    }

    K minKey(Node<K,V>* node) const
    {
        if (node->lNode == NULL)
            return node->key;
        else
            minKey(node->lNode);
    }

    Node<K,V>* delMin(Node<K,V>* node)
    {
        if (node == NULL)
            return NULL;
        if (node->lNode == NULL)
        {
            delete node;
            return NULL;
        }
        if (!isRed(this->root->lNode) && !isRed(this->root->rNode))
                this->root = RED;
        if (isRed(node) && !isRed(node->lNode) && !isRed(node->lNode->lNode))
            reverseFlipNode(node);
        if (node->rNode != NULL && isRed(node->rNode) && isRed(node->rNode->lNode))
            node = fixNode(node);

        node->lNode = delMin(node->lNode);

        //恢复红黑树的性质
        if (isRed(node->lNode) && isRed(node->rNode))
            filpNode(node);
        if (!isRed(node->lNode) && isRed(node->rNode))
            node = rotateLeft(node);

        node->N = size(node->lNode) + size(node->rNode) + 1;
        return node;
    }

    Node<K,V>* delMax(Node<K,V>* node)
    {
        if (node == NULL)
            return NULL;
        if (node->rNode == NULL)
        {
            if (isRed(node->lNode))
                node = rotateRight(node);
            else
            {
                    delete node;
                    return NULL;
            }
        }
        if (!isRed(this->root->lNode) && !isRed(this->root->rNode))
            this->root = RED;
        if (isRed(node->lNode) && !isRed(node->rNode->lNode))
            node = rotateRight(node);
        if (isRed(node) && !isRed(node->lNode) && !isRed(node->rNode->lNode))
            reverseFlipNode(node);

        node->rNode = delMax(node->rNode);

        if (isRed(node->lNode) && isRed(node->rNode))
            filpNode(node);
        if (isRed(node->rNode) && !isRed(node->lNode))
            node = rotateLeft(node);

        node->N = size(node->lNode) + size(node->rNode) + 1;
        return node;
    }

    bool isRed(Node<K,V>* node)
    {
        if (node == NULL || node->color == BLACK)
            return false;
        else
            return true;
    }

    int countLevel(Node<K,V>* node) const;
};

#endif