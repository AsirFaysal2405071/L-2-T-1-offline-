#include <bits/stdc++.h>
using namespace std;
class Node
{
public:
    int key;
    int order;
    Node *par;
    vector<Node *> child;
    Node(int v) : key(v), order(0), par(nullptr) {}
};
void merge(Node *a, Node *b, vector<Node *> &x)
{
    if (!a || !b)
        return;

    if (a->order != b->order)
        return;
    if (max(a->order, b->order) + 1 >= x.size())
        x.resize(max(a->order, b->order) + 2);

    if (a->key >= b->key)
    {
        b->child.insert(b->child.begin() + b->order, a);
        a->par = b;
        int t = b->order;
        x[t] = nullptr;
        b->order++;
        if (!x[b->order])
        {
            x[b->order] = b;
        }
        else
            merge(x[b->order], b, x);
    }
    else
    {
        a->child.insert(a->child.begin() + b->order, b);
        b->par = a;
        int t = a->order;
        x[t] = nullptr;
        a->order++;
        if (!x[a->order])
        {
            x[a->order] = a;
        }
        else
            merge(x[a->order], a, x);
    }
}

void insert(int x, vector<Node *> &v)
{
    if (!v.size())
        v.resize(1, nullptr);
    Node *new_node = new Node(x);
    if (!v[0])
        v[0] = new_node;
    else
        merge(v[0], new_node, v);
}

Node *find_min(vector<Node *> &v)
{
    Node *x = nullptr;
    for (auto &y : v)
    {
        if (y && (!x || y->key < x->key))
            x = y;
    }
    return x;
}

void consolidation(vector<Node *> &v1, vector<Node *> &v2)
{
    if (!v2.size())
        return;
    for (auto &x : v2)
    {
        if (!x)
            continue;
        if (x->order + 1 >= v1.size())
            v1.resize(x->order + 2);
        if (!v1[x->order])
            v1[x->order] = x;
        else
            merge(v1[x->order], v2[x->order], v1);
    }
    v2.clear();
}
int extract_min(vector<Node *> &v)
{
    if (!v.size())
        return INT_MIN;
    Node *x = find_min(v);
    if (!x)
        return INT_MIN;
    int val = x->key;
    vector<Node *> dh(x->order);
    for (auto &y : x->child)
    {
        dh[y->order] = y;
        y->par = nullptr;
    }
    delete v[x->order];
    v[x->order] = nullptr;
   consolidation(v, dh);
    return val;
}
Node *find_in_tree(Node *root, int key)
{
    if (!root)
        return nullptr;
    if (root->key == key)
        return root;

    for (Node *c : root->child)
    {
        if (Node *result = find_in_tree(c, key))
            return result;
    }
    return nullptr;
}

Node *find(int key, vector<Node *> &heap)
{
    for (Node *root : heap)
    {
        if (Node *result = find_in_tree(root, key))
            return result;
    }
    return nullptr;
}
void bubble_up(Node *node)
{
    while (node && node->par && node->key < node->par->key)
    {
        swap(node->key, node->par->key);
        node = node->par;
    }
}
void bubble_down(Node *node)
{
    while (node && !node->child.empty())
    {
        Node *smallest = node->child[0];

        for (Node *c : node->child)
        {
            if (c->key < smallest->key)
                smallest = c;
        }

        if (node->key <= smallest->key)
            break;

        swap(node->key, smallest->key);
        node = smallest;
    }
}

void decrease_key(int val, int new_val, vector<Node *> &v)
{
    Node *x = find(val, v);

    if (!x)
        return;

    if (new_val > x->key)
        return;

    x->key = new_val;
    bubble_up(x);
}
void remove_key(int val, vector<Node *> &v)
{
    Node *x = find(val, v);

    if (!x)
        return;

    x->key = INT_MIN;
    bubble_up(x);
    extract_min(v);
}
int count_nodes(Node *root)
{
    if (!root)
        return 0;

    int total = 1;

    for (Node *child : root->child)
        total += count_nodes(child);

    return total;
}

void print(Node *root)
{
    queue<Node *> q;
    q.push(root);

    int level = 0;

    while (!q.empty())
    {
        int nodes_at_level = q.size();

        cout << "Level " << level << ": ";

        for (int i = 0; i < nodes_at_level; i++)
        {
            Node *current = q.front();
            q.pop();

            cout << current->key;

            if (i + 1 < nodes_at_level)
                cout << " ";

            for (Node *child : current->child)
                q.push(child);
        }

        cout << endl;
        level++;
    }
}

void print_heap(vector<Node *> &heap, int x)
{
    cout << "Printing Binomial Heap H" << x << '\n';
    if (heap.empty())
    {
        cout << "Heap size is zero" << endl
             << "Heap H" << x << " is empty" << endl;
        return;
    }

    int size = 0;
    for (Node *root : heap)
        size += count_nodes(root);

    cout << "Heap size: " << size << '\n';

    for (int order = 0; order < (int)heap.size(); order++)
    {
        if (heap[order])
        {
            cout << "Binomial Tree, B" << order << '\n';
            print(heap[order]);
        }
    }
}

void dn(Node *a, int x)
{
    if (!a)
        return;

    for (int i = 0; i < x; i++)
        cout << "    ";

    if (x > 0)
        cout << "|-- ";

    cout << a->key <<endl;

    for (Node *child : a->child)
        dn(child, x + 1);
}

void visu_heap(vector<Node *> &v, int x)
{
    cout << "Visualizing Binomial Heap H" << x << endl;
    if (v.empty())
    {
        cout << "Heap H" << x << " is empty" << '\n';
        return;
    }
    bool any = false;
    for (int i = 0; i < v.size();i++)
    {
        if (!v[i])
            continue;
        any = true;
        cout << "Binomial Tree, B" <<i << '\n';
        dn(v[i], 0);
        cout << endl;
    }
    if (!any)
        cout << "Heap H" << x << " is empty" << endl;
}

void visu_consolidation(vector<Node *> &v1, vector<Node *> &v2, int x, int y)
{
    cout << endl
         << "Before Union " << endl;
    visu_heap(v1, x);
    visu_heap(v2, y);

    consolidation(v1, v2);

    cout << endl
         << " After Union (result stored in H" << x << ")" << endl;
   visu_heap(v1, x);
}
int main(int argc, char *argv[])
{
    vector<Node *> h1;
    vector<Node *> h2;
    if (argc < 2)
        return 0;
    string file = argv[1];
    ifstream fin(file);
    char a;
    while (fin >> a)
    {
        if (a == 'P')
        {
            int x;
            fin >> x;
            if (x == 2)
                print_heap(h2, x);
            else
                print_heap(h1, x);
        }
        if (a == 'I')
        {
            int x, y;
            fin >> x >> y;
            if (x == 1)
            {
                insert(y, h1);
            }
            else
                insert(y, h2);
            ;
        }
        if (a == 'F')
        {
            int x;
            fin >> x;
            Node *y = nullptr;
            if (x == 1)
                y = find_min(h1);
            else
                y = find_min(h2);
            if (y)
                cout << "Find min returned: " << y->key << endl;
            else
                cout << "Heap " << x << " is empty" << endl;
        }
        if (a == 'E')
        {
            int x;
            fin >> x;
            int y;
            if (x == 1)
                y = extract_min(h1);
            else
                y = extract_min(h2);
            if (y != INT_MIN)
                cout << "Extracted min returned: " << y << endl;
            else
                cout << "Heap " << x << " is empty" << endl;
        }
        if (a == 'U')
        {
            int x, y;
            fin >> x >> y;
            if (x == 1)
                consolidation(h1, h2);
            else
                consolidation(h2, h1);
        }
        if (a == 'D')
        {
            int x, y, z;
            fin >> x >> y >> z;
            if (x == 1)
            {
                decrease_key(y, z, h1);
            }
            else
                decrease_key(y, z, h2);
        }
        if (a == 'R')
        {
            int x, y;
            cin >> x >> y;
            if (x == 1)
                remove_key(y, h1);
            else
                remove_key(y, h2);
        }
        if (a == 'V')
        {
            int x;
            fin >> x;
            if (x == 2)
                visu_heap(h2, x);
            else
                visu_heap(h1, x);
        }
        if (a == 'W')
        {
            int x, y;
            fin >> x >> y;
            if (x == 1)
                visu_consolidation(h1, h2, x, y);
            else
                 visu_consolidation(h2, h1, x, y);
        }
    }
    return 0;
}