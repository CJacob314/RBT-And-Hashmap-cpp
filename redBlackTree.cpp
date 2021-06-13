/* Copyright (C) Jacob Cohen - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Jacob Cohen <jcohen30@uic.edu> or <jacob@jacobcohen.info>
 */

#include <iostream>

using namespace std;

enum RBTColor
{
    RED = true,
    BLACK = false
};

template <typename kT, typename vT> struct RBTNode
{
    kT key;
    vT value;

    RBTColor color;

    RBTNode<kT, vT>* ptrs[2]; // 0 is left, 1 is right

    RBTNode<kT, vT>(kT key, vT value, RBTColor color)
    {
        this->key = key;
        this->value = value;
        this->ptrs[0] = this->ptrs[1] = nullptr;
        this->color = color;
    }

    /*
    RBTNode<kT, vT>* getLeft() { return ptrs[0]; }
    RBTNode<kT, vT>* getRight() { return ptrs[1]; }

    void setLeft(RBTNode<kT, vT>* left) { this->ptrs[0] = left; }
    void setRight(RBTNode<kT, vT>* right) { this->ptrs[1] = right; }
    */

    void setColor(RBTColor color) { this->color = color; }
};

template <typename kT, typename vT> class RBT
{
    private:
    RBTNode<kT, vT>* root;

    bool isRed(RBTNode<kT, vT>* node) { return node != nullptr && node->color == RBTColor::RED; }

    vT get(RBTNode<kT, vT>* node, kT key)
    {
        while (node != nullptr)
        {
            if (node->key == key) return node->value;
            int lr = node->key < key;

            node = node->ptrs[lr];
        }
    }

    RBTNode<kT, vT>* singleRotation(RBTNode<kT, vT>* node, int lr)
    {
        RBTNode<kT, vT>* t = node->ptrs[!lr];

        node->ptrs[!lr] = t->ptrs[lr];
        t->ptrs[lr] = node;

        node->setColor(RBTColor::RED);
        t->setColor(RBTColor::BLACK);

        return t;
    }

    RBTNode<kT, vT>* doubleRotation(RBTNode<kT, vT>* node, int lr)
    {
        node->ptrs[!lr] = singleRotation(node->ptrs[!lr], !lr);

        return singleRotation(node, lr);
    }

    RBTNode<kT, vT>* put(RBTNode<kT, vT>* node, kT key, vT value)
    {
        if (node == nullptr) return new RBTNode<kT, vT>(key, value, RBTColor::RED);

        if (key != node->key)
        {
            int lr = node->key < key; // 0 or 1

            node->ptrs[lr] = put(node->ptrs[lr], key, value);

            if (isRed(node->ptrs[lr]))
            {
                if (isRed(node->ptrs[!lr]))
                {
                    // case 1
                    node->setColor(RBTColor::RED);
                    node->ptrs[0]->setColor(RBTColor::BLACK);
                    node->ptrs[1]->setColor(RBTColor::BLACK);
                }
                else
                {
                    // cases 2 and 3
                    if (isRed(node->ptrs[lr]->ptrs[lr]))
                    {
                        node = singleRotation(node, !lr);
                    }
                    else if (isRed(node->ptrs[lr]->ptrs[!lr]))
                    {
                        node = doubleRotation(node, !lr);
                    }
                }
            }
        }
        else
            node->value = value;

        return node;
    }

    RBTNode<kT, vT>* remove(RBTNode<kT, vT>* node, kT key, bool& done)
    {
        if (node == nullptr)
            done = true;
        else
        {
            int lr;

            if (node->key == key)
            {
                if (node->ptrs[0] == nullptr || node->ptrs[1] == nullptr)
                {
                    RBTNode<kT, vT>* t = node->ptrs[node->ptrs[0] == nullptr];

                    // case 0
                    if (isRed(node))
                        done = true;
                    else if (isRed(t))
                    {
                        t->setColor(RBTColor::BLACK);
                        done = true;
                    }

                    delete node;

                    return t;
                }
                else
                {
                    RBTNode<kT, vT>* succ = node->ptrs[0];

                    while (succ->ptrs[1] != nullptr) succ = succ->ptrs[1];

                    node->key = succ->key;
                    key = succ->key;
                }
            }

            lr = node->key < key;
            node->ptrs[lr] = remove(node->ptrs[lr], key, done);

            if (!done) node = afterRemBal(node, lr, done);
        }

        return node;
    }

    RBTNode<kT, vT>* afterRemBal(RBTNode<kT, vT>* node, int lr, bool& done)
    {
        RBTNode<kT, vT>* p = node;
        RBTNode<kT, vT>* s = node->ptrs[!lr];

        if (isRed(s))
        {
            node = singleRotation(node, lr);
            s = p->ptrs[!lr];
        }

        if (s != nullptr)
        {
            if (!isRed(s->ptrs[0]) && !isRed(s->ptrs[1]))
            {
                if (isRed(p)) done = true;

                p->setColor(RBTColor::BLACK);
                s->setColor(RBTColor::RED);
            }
            else
            {
                bool t = p->color == RBTColor::RED;
                bool newNode = (node == p);

                if (isRed(s->ptrs[!lr]))
                    p = singleRotation(p, lr);
                else
                    p = doubleRotation(p, lr);

                p->setColor(t ? RBTColor::RED : RBTColor::BLACK);
                p->ptrs[0]->setColor(RBTColor::BLACK);
                p->ptrs[1]->setColor(RBTColor::BLACK);

                if (newNode)
                    node = p;
                else
                    node->ptrs[lr] = p;

                done = true;
            }
        }

        return node;
    }

    void __dump(RBTNode<kT, vT>* node, string indent, bool last)
    {
        if (node != nullptr)
        {
            cout << indent;
            if (last)
            {
                cout << "R====";
                indent += "   ";
            }
            else
            {
                cout << "L====";
                indent += "|  ";
            }

            string sColor = node->color == RBTColor::RED ? "RED" : "BLACK";
            cout << node->key << "->" << node->value << "(" << sColor << ")" << endl;
            __dump(node->ptrs[0], indent, false);
            __dump(node->ptrs[1], indent, true);
        }
    }

    void destroyTree(RBTNode<kT, vT>*& node)
    {
        if (node == nullptr) return;

        destroyTree(node->ptrs[0]);
        destroyTree(node->ptrs[1]);

        delete node;

        node = nullptr;
    }

    void preOrderInserts(RBTNode<kT, vT>* node)
    {
        this->put(node->key, node->value);
        if (node->ptrs[0] != nullptr) preOrderInserts(node->ptrs[0]);
        if (node->ptrs[1] != nullptr) preOrderInserts(node->ptrs[1]);
    }

    public:
    RBT<kT, vT>() { this->root = nullptr; }
    bool isEmpty() { return root == nullptr; }
    vT get(kT key) { return get(root, key); }
    void put(kT key, vT value)
    {
        root = put(root, key, value);
        root->setColor(RBTColor::BLACK);
    }
    void remove(kT key)
    {
        bool done = false;
        root = remove(root, key, done);

        if (root != nullptr) root->setColor(RBTColor::BLACK);
    }

    void dump()
    {
        if (root != nullptr) __dump(root, "", true);
    }

    ~RBT<kT, vT>() { destroyTree(root); }

    RBT<kT, vT>(const RBT<kT, vT>& other)
    {
        if (other.root == nullptr)
            root = nullptr;
        else
            preOrderInserts(other.root);
    }

    RBT<kT, vT>& operator=(const RBT<kT, vT>& other)
    {
        if (this != &other)
        {
            destroyTree(root);

            RBTNode<kT, vT>* otherRoot = other.root;
            preOrderInserts(otherRoot);
        }
        return *this;
    }
};