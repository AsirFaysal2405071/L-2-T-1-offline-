#include <bits/stdc++.h>
using namespace std;

class Node
{
public:
    int key;
    Node *left;
    Node *right;
    int height;
    Node(int val) : key(val), left(nullptr), right(nullptr), height(0) {}
};
void ll(Node *root);
void rr(Node *root);
int height(Node *root);
int getBalance(Node *root);
bool find(int key, Node *root);
// insert
bool insert(int x, Node *&root)
{
    if (root == nullptr)
    {
        root = new Node(x);
        return true;
    }

    if (find(x, root))
        return false;

    if (x < root->key)
        insert(x, root->left);
    else
        insert(x, root->right);

    root->height =
        1 + max(height(root->left),
                height(root->right));

    int b = getBalance(root);

    if (b > 1 && x < root->left->key)
        rr(root);

    else if (b > 1 && x > root->left->key)
    {
        ll(root->left);
        rr(root);
    }

    else if (b < -1 && x < root->right->key)
    {
        rr(root->right);
        ll(root);
    }

    else if (b < -1 && x > root->right->key)
        ll(root);

    return true;
}
// find
bool find(int key, Node *root)
{
    if (root == nullptr)
        return false;
    else if (root->key == key)
        return true;
    else if (root->key > key)
        return find(key, root->left);
    else
        return find(key, root->right);
}

// rotations
void rr(Node *r)
{
    Node *x = r->left;
    int key = r->key;
    Node *y = x->right;
    Node *z = r->right;
    r->key = x->key;
    Node *n = new Node(key);
    n->right = z;
    n->left = y;

    r->left = x->left;
    r->right = n;
    x->left = nullptr;
    delete x;

    n->height = 1 + max(height(n->left), height(n->right));

    r->height = 1 + max(height(r->left), height(r->right));
}

void ll(Node *r)
{
    Node *x = r->right;
    int key = r->key;
    Node *n = new Node(key);
    n->left = r->left;
    n->right = x->left;
    r->key = x->key;
    r->right = x->right;
    x->right = nullptr;
    r->left = n;
    delete x;

    n->height = 1 + max(height(n->left), height(n->right));

    r->height = 1 + max(height(r->left), height(r->right));
}
int height(Node *n)
{
    if (!n)
        return -1;
    return n->height;
}
int getBalance(Node *N)
{
    if (N == nullptr)
        return 0;
    return height(N->left) -
           height(N->right);
}

// deletion
bool erase(int key, Node *&root)
{
    if (!root)
        return false;

    if (key < root->key)
    {
        if (!erase(key, root->left))
            return false;
    }
    else if (key > root->key)
    {
        if (!erase(key, root->right))
            return false;
    }
    else
    {
      
        if (!root->left && !root->right)
        {
            delete root;
            root = nullptr;
            return true;
        }

       
        else if (!root->left)
        {
            Node *temp = root;
            root = root->right;
            delete temp;
        }

        
        else if (!root->right)
        {
            Node *temp = root;
            root = root->left;
            delete temp;
        }

        
        else
        {
            Node *x = root->right;

            // Find inorder successor
            while (x->left)
                x = x->left;

            root->key = x->key;

            erase(x->key, root->right);
        }
    }

    if (!root)
        return true;

    
    root->height =
        1 + max(height(root->left),
                height(root->right));

    int b = getBalance(root);

    
    if (b > 1 && getBalance(root->left) >= 0)
    {
        rr(root);
    }

    
    else if (b > 1 && getBalance(root->left) < 0)
    {
        ll(root->left);
        rr(root);
    }

    
    else if (b < -1 && getBalance(root->right) <= 0)
    {
        ll(root);
    }

    
    else if (b < -1 && getBalance(root->right) > 0)
    {
        rr(root->right);
        ll(root);
    }

    return true;
}
// traverse
void p(Node* root)
{
    if (!root)
        return;

    cout << root->key;

    if (root->left || root->right)
    {
        cout << "(";

        if (root->left)
            p(root->left);

        cout << ",";

        if (root->right)
            p(root->right);

        cout << ")";
    }
}
void t(Node *&root, vector<int> &c)
{
    if (!root)
        return;
    t(root->left, c);
    c.push_back(root->key);
    t(root->right, c);
}

vector<int> traverse(Node *root)
{
    vector<int> c;
    t(root, c);
    return c;
}

int main()
{

    Node *root = nullptr;
    ifstream fin("input.txt");

    char command;
    int x;
    while (fin >> command)
    {
        
        if (command == 'I')
        {
            fin >> x;
            bool ans = insert(x, root);
            if (!ans)
                cout << "duplicate";
            else
                p(root);
            cout << endl;
        }
        else if (command == 'D')
        {
            fin >> x;
            bool ans = erase(x, root);
            if (!ans)
                cout << "not found";
            else
                p(root);
            cout << endl;
        }
        else if (command == 'F')
        {
            fin >> x;

            if (find(x, root))
                cout <<  " Found";
            else
                cout << " Not Found";
            cout << endl;
        }
        else if (command == 'T')
        {
            vector<int> c = traverse(root);

            for (int v : c)
                cout << v << " ";

            cout << endl;
        }
    }
  
    return 0;
}