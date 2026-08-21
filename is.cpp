#include <bits/stdc++.h>
using namespace std;
class Node
{
public:
    int key;
    Node *left;
    Node *right;
    int height;
    int start, end, maxend;
    Node(int val, int s, int e) : key(val), left(nullptr), right(nullptr), height(0), start(s), end(e), maxend(e) {}
};

void ll(Node *root);
void rr(Node *root);
int height(Node *root);
int getBalance(Node *root);
int getme(Node *root);
// insert
bool insert(int x, int s, int e, Node *&root)
{
    if (root == nullptr)
    {
        root = new Node(x, s, e);
        return true;
    }

    if (s < root->start || (s == root->start && x < root->key))
        insert(x, s, e, root->left);
    else
        insert(x, s, e, root->right);

    root->height =
        1 + max(height(root->left),
                height(root->right));
    root->maxend = max({root->end,
                        max(getme(root->left),
                            getme(root->right))});

    int b = getBalance(root);

    if (b > 1 && (s < root->left->start || (s == root->left->start && x < root->left->key)))
        rr(root);

    else if (b > 1 && (s > root->left->start || (s == root->left->start && x > root->left->key)))
    {
        ll(root->left);
        rr(root);
    }

    else if (b < -1 && (s < root->right->start || (s == root->right->start && x < root->right->key)))
    {
        rr(root->right);
        ll(root);
    }

    else if (b < -1 && (s > root->right->start || (s == root->right->start && x > root->right->key)))
        ll(root);

    return true;
}

// rotations
void rr(Node *r)
{
    Node *x = r->left;
    int key = r->key, s = r->start, e = r->end;
    Node *y = x->right;
    Node *z = r->right;
    r->key = x->key;
    r->start = x->start;
    r->end = x->end;
    Node *n = new Node(key, s, e);
    n->right = z;
    n->left = y;

    r->left = x->left;
    r->right = n;
    x->left = nullptr;
    delete x;

    n->height = 1 + max(height(n->left), height(n->right));

    r->height = 1 + max(height(r->left), height(r->right));
    n->maxend = max({n->end,
                     max(getme(n->left),
                         getme(n->right))});

    r->maxend = max({r->end,
                     max(getme(r->left),
                         getme(r->right))});
}

void ll(Node *r)
{
    Node *x = r->right;
    int key = r->key, s = r->start, e = r->end;
    Node *n = new Node(key, s, e);
    n->left = r->left;
    n->right = x->left;
    r->key = x->key;
    r->start = x->start;
    r->end = x->end;
    r->right = x->right;
    x->right = nullptr;
    r->left = n;
    delete x;

    n->height = 1 + max(height(n->left), height(n->right));

    r->height = 1 + max(height(r->left), height(r->right));
    n->maxend = max({n->end,
                     max(getme(n->left),
                         getme(n->right))});

    r->maxend = max({r->end,
                     max(getme(r->left),
                         getme(r->right))});
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

int getme(Node *r)
{
    if (r == nullptr)
        return 0;
    return r->maxend;
}

// deletion
bool erase(int key, int s, Node *&root)
{
    if (!root)
        return false;

    if (s < root->start || (s == root->start && key < root->key))
    {
        if (!erase(key, s, root->left))
            return false;
    }
    else if (s > root->start || (s == root->start && key > root->key))
    {
        if (!erase(key, s, root->right))
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
            Node *temp = root->right;
            while (temp->left)
                temp = temp->left;

            root->key = temp->key;
            root->start = temp->start;
            root->end = temp->end;

            erase(temp->key, temp->start, root->right);
        }

        else if (!root->right)
        {
            Node *temp = root->left;

            while (temp->right)
            {
                temp = temp->right;
            }
            root->key = temp->key;
            root->start = temp->start;
            root->end = temp->end;
            erase(temp->key, temp->start, root->left);
        }

        else
        {
            Node *x = root->right;
            while (x->left)
                x = x->left;

            int succKey = x->key;
            int succStart = x->start;
            int succEnd = x->end;

            root->key = succKey;
            root->start = succStart;
            root->end = succEnd;

            erase(succKey, succStart, root->right);
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

    root->maxend = max({root->end,
                        max(getme(root->left),
                            getme(root->right))});
    return true;
}
// traverse
void p(Node *root)
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

void at(int t, Node *root, bool &x)
{
    if (root == nullptr)
        return;
    if (root->left != nullptr && root->left->maxend > t)
        at(t, root->left, x);
    if (t >= root->start && t < root->end)
    {
        cout << root->key << " ";
        x = true;
    }
    if (root->start <= t)
        at(t, root->right, x);
}

Node *next(Node *root, int t)
{
    if (root == nullptr)
        return nullptr;

    if (root->start < t)
        return next(root->right, t);

    Node *leftAns = next(root->left, t);

    if (leftAns != nullptr)
        return leftAns;

    return root;
}

// conflict
void overlap(int s1, int e1, Node *root, vector<pair<int, int>> &v)
{
    if (root == nullptr)
        return;

    if (root->left && root->left->maxend > s1)
        overlap(s1, e1, root->left, v);

    if (s1 < root->end && root->start < e1)
        v.push_back({root->start, root->key});

    if (root->start < e1)
        overlap(s1, e1, root->right, v);
}

bool conflict(int s, int e, Node *root)
{
    if (root == nullptr)
        return false;

    if (root->left && root->left->maxend > s)
    {
        if (conflict(s, e, root->left))
            return true;
    }

    if (root->start < e && root->end > s)
        return true;

    if (root->start < e)
        return conflict(s, e, root->right);

    return false;
}

int main()
{
    ifstream fin("isinput.txt");
    long long addCount = 0, addTotal = 0;
    long long removeCount = 0, removeTotal = 0;
    long long updateCount = 0, updateTotal = 0;
    long long conflictCount = 0, conflictTotal = 0;
    long long overlapsCount = 0, overlapsTotal = 0;
    long long atCount = 0, atTotal = 0;
    long long nextCount = 0, nextTotal = 0;
    Node *root = nullptr;
    unordered_map<int, pair<int, int>> m;
    string com;
    int id = 1;
    while (fin >> com)
    {
        if (com == "ADD")
        {
            int s, e;
            fin >> s >> e;
            auto st = chrono::steady_clock::now();
            bool x = insert(id, s, e, root);
            auto en = chrono::steady_clock::now();

            addTotal += chrono::duration_cast<chrono::nanoseconds>(en - st).count();
            addCount++;
            if (x)
            {
                m[id] = {s, e};
                id++;
                p(root);
                cout << endl;
            }
        }
        else if (com == "CONFLICT")
        {
            int s, e;
            fin >> s >> e;
            vector<pair<int, int>> v;
            auto st = chrono::steady_clock::now();
            bool ans = conflict(s, e, root);
            auto en = chrono::steady_clock::now();

            conflictTotal += chrono::duration_cast<chrono::nanoseconds>(en - st).count();
            conflictCount++;
            if (ans)
                cout << "yes";
            else
                cout << "no";
            cout << endl;
        }
        else if (com == "OVERLAPS")
        {
            int s, e;
            fin >> s >> e;
            vector<pair<int, int>> v;
            auto st = chrono::steady_clock::now();
            overlap(s, e, root, v);
            auto en = chrono::steady_clock::now();
            overlapsTotal = chrono::duration_cast<chrono::nanoseconds>(en - st).count();
            overlapsCount++;
            sort(v.begin(), v.end());

            for (auto &x : v)
            {
                cout << x.second << " ";
            }
            if (v.empty())
                cout << "none";
            cout << endl;
        }
        else if (com == "UPDATE")
        {
            int i, ns, e;

            fin >> i >> ns >> e;
            if (m.find(i) != m.end())
            {
                int s = m[i].first;
                auto st = chrono::steady_clock::now();

                erase(i, s, root);
                insert(i, ns, e, root);

                auto en = chrono::steady_clock::now();

                updateTotal += chrono::duration_cast<chrono::nanoseconds>(en - st).count();
                updateCount++;
                m[i].first = ns;
                m[i].second = e;
                p(root);
            }
            else
                cout << "not found";
            cout << endl;
        }
        else if (com == "REMOVE")
        {
            int i;
            fin >> i;
            if (m.find(i) != m.end())
            {
                int s = m[i].first;
                auto st = chrono::steady_clock::now();

                erase(i, s, root);

                auto en = chrono::steady_clock::now();

                removeTotal += chrono::duration_cast<chrono::nanoseconds>(en - st).count();
                removeCount++;
                p(root);
                m.erase(i);
            }
            else
                cout << "not found";
            cout << endl;
        }
        else if (com == "AT")
        {
            int k;
            fin >> k;
            bool x = false;
            auto st = chrono::steady_clock::now();

            at(k, root, x);

            auto en = chrono::steady_clock::now();

            atTotal += chrono::duration_cast<chrono::nanoseconds>(en - st).count();
            atCount++;
            if (!x)
                cout << "none";
            cout << endl;
        }
        else if (com == "NEXT")
        {
            int k;

            fin >> k;
            auto st = chrono::steady_clock::now();

            Node *ans = next(root, k);

            auto en = chrono::steady_clock::now();

            nextTotal += chrono::duration_cast<chrono::nanoseconds>(en - st).count();
            nextCount++;
            if (!ans)
                cout << "none";
            else
                cout << ans->key << " " << ans->start << " " << ans->end;
            cout << endl;
        }
    }
    cout << "operation_count operation_total operation_ns" << endl;
    auto avg = [](long long total, long long count)
    {
        if (count == 0)
            return string("N/A");

        return to_string(total / count);
    };

    cout << "add: " << addCount << " " << addTotal << " "
         << avg(addTotal, addCount) << "\n";

    cout << "remove: " << removeCount << " " << removeTotal << " "
         << avg(removeTotal, removeCount) << "\n";

    cout << "update: " << updateCount << " " << updateTotal << " "
         << avg(updateTotal, updateCount) << "\n";

    cout << "conflict: " << conflictCount << " " << conflictTotal << " "
         << avg(conflictTotal, conflictCount) << "\n";

    cout << "overlaps: " << overlapsCount << " " << overlapsTotal << " "
         << avg(overlapsTotal, overlapsCount) << "\n";

    cout << "at: " << atCount << " " << atTotal << " "
         << avg(atTotal, atCount) << "\n";

    cout << "next: " << nextCount << " " << nextTotal << " "
         << avg(nextTotal, nextCount) << "\n";

    return 0;
}
