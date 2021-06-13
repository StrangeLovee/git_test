#include <iostream>
#include "rbtree.h"

//构造 析构函数
template<typename K, typename V>
RBT<K,V>::RBT()
{
    this->root = NULL;
}

template<typename K, typename V>
RBT<K,V>::~RBT()
{
    clean(this->root);
}


/******************************************************************** 
函数名称:  isRed
函数参数： @node 节点指针
函数说明:  判断节点的颜色
返回值:    成功返回true 失败返回false
*********************************************************************/
template<typename K, typename V>
bool RBT<K,V>::isRed(Node<K,V>* node)
{
    if (node == NULL || node->color == BLACK)
        return false;
    else
        return true;
}

/******************************************************************** 
函数名称:   put
函数说明:   放入键值对<key,value> 若存在则将原有的value替换成新的value
返回值:     无
*********************************************************************/
template<typename K, typename V>
void RBT<K,V>::put(const K& key, const V& val)
{
    this->root = put(this->root, key, val);
}

template<typename K, typename V>
Node<K,V>* RBT<K,V>::put(Node<K,V>* node, const K& key, const V& val)
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

/******************************************************************** 
函数名称:   get
函数说明:   获取指定key的value
返回值:     value
*********************************************************************/
template<typename K, typename V>
V RBT<K,V>::get(const K& key) const
{
    return get(this->root, key);
}

template<typename K, typename V>
V RBT<K,V>::get(Node<K,V>* node, const K& key) const
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

/******************************************************************** 
函数名称:   clean
函数说明:   清空表
返回值:    无 
*********************************************************************/
template<typename K, typename V>
void RBT<K,V>::clean()
{
    clean(this->root);
    this->root = NULL;
}

template<typename K, typename V>
void RBT<K,V>::clean(Node<K,V>* node)
{
    if (node == NULL)
        return ;
    clean(node->lNode);
    clean(node->rNode);
    delete node;
}
/******************************************************************** 
函数名称:   size
函数说明:   无
返回值:     表的大小
*********************************************************************/
template<typename K, typename V>
int RBT<K,V>::size() const
{
    return size(this->root);
}

template<typename K, typename V>
int RBT<K,V>::size(Node<K,V>* node) const
{
    if (node == NULL)
        return 0;
    return size(node->rNode) + size(node->lNode) + 1;
}
/******************************************************************** 
函数名称:   del
函数说明:   删除某个key
返回值:     无
*********************************************************************/
template<typename K ,typename V>
void RBT<K,V>::del(const K& key)
{
    this->root = del(this->root, key);
}

template<typename K, typename V>
Node<K,V>* RBT<K,V>::del(Node<K,V>* node, const K& key)
{
    if (node == NULL)
        return NULL;
    if (!isRed(this->root->lNode) && !isRed(this->root->rNode))
            this->root = RED;
    if (key > node->key)
    {
        if (isRed(node->lNode))
            node = rotateRight(node);
        if (node->rNode != NULL && isRed(node) && !isRed(node->rNode) && !isRed(node->rNode->lNode))
            reverseFlipNode(node);
        node->rNode = del(node->rNode, key);
    }
    else if (key < node->key)
    {
        if (node->lNode != NULL && isRed(node) && !isRed(node->lNode) && !isRed(node->lNode->lNode))
            reverseFlipNode(node);
        if (node->rNode != NULL && isRed(node->rNode) && isRed(node->rNode->lNode))
            node = fixNode(node);
        node->lNode = del(node->lNode, key);
    }
    else
    {
        if (isRed(node))
        {
            if (node->lNode == NULL)
            {
                delete node;
                return NULL;
            }
            else
            {
                node->key = minKey(node->rNode);
                node->val = get(node->key);
                node->rNode = delMin(node->rNode);
            }
        }
        else
        {
            node = rotateRight(node);
            node->rNode = del(node->rNode, key);
        }

    }
    if (isRed(node->rNode) && !isRed(node->lNode))
        node = rotateLeft(node);
    if (isRed(node->lNode) && isRed(node->rNode))
        filpNode(node);

    node->N = size(node->rNode) + size(node->lNode) + 1;
    return node;
}

/******************************************************************** 
函数名称:   print
函数说明:   打印表中内容
返回值:     无
*********************************************************************/
template<typename K, typename V>
void RBT<K,V>::print() const
{
    print(this->root);
}

template<typename K, typename V>
void RBT<K,V>::print(Node<K,V>* node) const
{
    if (node == NULL)
        return ;
    print(node->lNode);
    std::cout << node->key << " : " << node->val << "  :  " << node->N << "  :  " << node->color << std::endl;
    print(node->rNode);
}
/******************************************************************** 
函数名称:   contain
函数说明:   是否包含key
返回值:     包含返回true 不包含返回false
*********************************************************************/
template<typename K, typename V>
bool RBT<K,V>::contain(const K& key) const
{
    return contain(this->root, key);
}

template<typename K, typename V>
bool RBT<K,V>::contain(Node<K,V>* node, const K& key) const
{
    if (node == NULL)
        return false;
    if (key > node->key)
        get(node->rNode, key);
    else if (key < node->key)
        get(node->lNode, key);
    else
        return true;
}
/******************************************************************** 
函数名称:  rotateLeft
函数参数： 需要旋转的节点指针
函数说明:  将右节点旋转到左边并设置为红色
返回值:    旋转后的树根指针
*********************************************************************/
template<typename K, typename V>
Node<K,V>* RBT<K,V>::rotateLeft(Node<K,V>* node)
{
    Node<K,V>* rNode = node->rNode;
    node->rNode = rNode->lNode;
    rNode->lNode = node;
    rNode->color = node->color;
    node->color = RED;
    node->N = size(node->lNode) + size(node->rNode) + 1;
    rNode->N = size(rNode->lNode) + size(rNode->rNode) + 1;
    return rNode;
}

/******************************************************************** 
函数名称:  rotateRight
函数参数： 需要旋转的节点指针
函数说明:  将左节点旋转到右边并设置为红色
返回值:    旋转后的树根指针
*********************************************************************/
template<typename K, typename V>
Node<K,V>* RBT<K,V>::rotateRight(Node<K,V>* node)
{
    Node<K,V>* lNode = node->lNode;
    node->lNode = lNode->rNode;
    lNode->rNode = node;
    lNode->color = node->color;
    node->color = RED;
    node->N = size(node->lNode) + size(node->rNode) + 1;
    lNode->N = size(lNode->lNode) + size(lNode->rNode) + 1;
    return lNode;
}

/******************************************************************** 
函数名称:  flipNode
函数参数： 节点指针
函数说明:  将节点的左右子树的颜色变成黑色 同时节点颜色变成红色
返回值:    无
*********************************************************************/
template<typename K, typename V>
void RBT<K,V>::filpNode(Node<K,V>* node)
{
    node->color = RED;
    node->lNode->color = BLACK;
    node->rNode->color = BLACK;
}

/******************************************************************** 
函数名称:  reverseFlipNode
函数参数： @node 节点指针
函数说明:  反向翻转节点和子节点的颜色 将节点颜色变成黑色 子节点红色
返回值:    无
*********************************************************************/
template<typename K, typename V>
void RBT<K,V>::reverseFlipNode(Node<K,V>* node)
{
    node->color = BLACK;
    node->lNode->color = RED;
    node->rNode->color = RED;
}

/******************************************************************** 
函数名称:  fixRedNode
函数参数： @node 节点指针
函数说明:  修复反向反转后节点 node->rNode->lNode 为红色的情况
返回值:    
*********************************************************************/
template<typename K, typename V>
Node<K,V>* RBT<K,V>::fixNode(Node<K,V>* node)
{
    node->rNode = rotateRight(node->rNode);
    node = rotateLeft(node);
    return node;
}

/******************************************************************** 
函数名称:  minKey
函数参数： 无
函数说明:  返回表中最小的key
返回值:    key
*********************************************************************/
template<typename K, typename V>
K RBT<K,V>::minKey() const
{
    return minKey(this->root);
}

template<typename K, typename V>
K RBT<K,V>::minKey(Node<K,V>* node) const
{
    if (node->lNode == NULL)
        return node->key;
    else
        minKey(node->lNode);
}

/******************************************************************** 
函数名称:  delMin
函数参数： 无
函数说明:  删除表中最小key 需要保证节点的颜色是红色
返回值:    无
*********************************************************************/
template<typename K, typename V>
void RBT<K,V>::delMin()
{
    this->root = delMin(this->root);
}

template<typename K, typename V>
Node<K,V>* RBT<K,V>::delMin(Node<K,V>* node)
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

/******************************************************************** 
函数名称:  delMax
函数参数： 无
函数说明:  删除树中最大key 需要保证节点的颜色是红色
返回值:    无
*********************************************************************/
template<typename K, typename V>
void RBT<K,V>::delMax()
{
    this->root = delMax(this->root);
}

template<typename K, typename V>
Node<K,V>* RBT<K,V>::delMax(Node<K,V>* node)
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

template<typename K, typename V>
int RBT<K,V>::countLevel() const
{
    return countLevel(this->root);
}

