#include <bits/stdc++.h>
using namespace std;

bool is_prime(int n)
{
  if (n < 2)
    return false;

  for (int i = 2; i * i <= n; i++)
  {
    if (n % i == 0)
      return false;
  }

  return true;
}

void find_prime(int &n)
{
  n = 2 * n + 1;

  while (!is_prime(n))
  {
    n++;
  }
}

int prev_prime(int n)
{
  n = n / 2 - 1;

  while (n >= 2)
  {
    if (is_prime(n))
      return n;

    n--;
  }

  return 2;
}
template <typename k>
unsigned long long hash1(const k &word)
{
  if constexpr (is_same_v<k, string>)
  {
    unsigned long long x = 6007;

    for (auto &y : word)
    {
      x = x * 31 + y;
    }

    return x;
  }
  else if constexpr (is_same_v<k, char>)
  {
    unsigned long long x = 6007;

    x = x * 31 + word;

    return x;
  }
  else
    return word;
}
template <typename k>
unsigned long long hash2(const k &word)
{
  if constexpr (is_same_v<k, string>)
  {
    unsigned long long x = 14695981039346656037ULL;

    for (auto &y : word)
    {
      x ^= y;
      x *= 1099511628211ULL;
    }

    return x;
  }
  else if constexpr (is_same_v<k, char>)
  {
    unsigned long long x = 14695981039346656037ULL;

    x ^= word;
    x *= 1099511628211ULL;

    return x;
  }
  else
    return word * 1099511628211ULL;
}
template <typename k>
unsigned long long aux_hash(const k &word)
{
  if constexpr (is_same_v<k, string>)
  {
    unsigned long long x = 0;

    for (auto &y : word)
    {
      x = x * 211 + y + 1;
    }

    return x;
  }
  else if constexpr (is_same_v<k, char>)
  {
    unsigned long long x = 0;

    x = x * 211 + word + 1;

    return x;
  }
  else
  {
    return word * 211 + 1;
  }
}

string ranword(int length, mt19937 &rng)
{
  string word;

  for (int i = 0; i < length; i++)
  {
    word += char('a' + rng() % 26);
  }

  return word;
}

template <typename K, typename V>
class chain_tb
{
public:
  using hf_type = unsigned long long (*)(const K &);

  vector<vector<pair<K, V>>> tb;
  hf_type hf;

  int ts;
  int cnt;
  int col;

  chain_tb(hf_type hf)
  {
    this->hf = hf;
    ts = 13;
    cnt = 0;
    col = 0;

    tb.resize(ts);
  }

  void rehash(int ns)
  {
    vector<vector<pair<K, V>>> prev_tb = tb;

    ts = ns;
    tb.clear();
    tb.resize(ts);

    for (auto &b : prev_tb)
    {
      for (auto &p : b)
      {
        int i = hf(p.first) % ts;
        tb[i].push_back(p);
      }
    }
  }

  void compress()
  {
    if ((double)cnt / ts < 0.25 && ts > 13)
    {
      int ns = prev_prime(ts);

      if (ns >= 13)
        rehash(ns);
    }
  }

  void insert(const K &key, const V &val)
  {
    int i = hf(key) % ts;

    for (auto &p : tb[i])
    {
      if (p.first == key)
        return;
    }

    if (!tb[i].empty())
      col++;

    tb[i].push_back({key, val});
    cnt++;

    if ((double)cnt / ts > 0.5)
    {
      find_prime(ts);
      rehash(ts);
    }
  }

  bool del(const K &key)
  {
    int i = hf(key) % ts;

    for (int j = 0; j < (int)tb[i].size(); j++)
    {
      if (tb[i][j].first == key)
      {
        tb[i].erase(tb[i].begin() + j);
        cnt--;

        compress();
        return true;
      }
    }

    return false;
  }

  int search(const K &key)
  {
    int i = hf(key) % ts;
    int cmp = 0;

    for (auto &p : tb[i])
    {
      cmp++;

      if (p.first == key)
        return cmp;
    }

    return -1;
  }
};

template <typename K, typename V>
class open_addresstb
{
public:
  using hf_type = unsigned long long (*)(const K &);

  vector<pair<K, V>> tb;

  vector<int> con;

  hf_type hf;
  hf_type aux_hf;

  int ts;
  int ic;
  int col;
  string way;

  open_addresstb(string type, hf_type h_f, hf_type auxhf)
  {
    way = type;
    hf = h_f;
    aux_hf = auxhf;

    ts = 13;
    ic = 0;
    col = 0;

    tb.resize(ts);
    con.resize(ts, 0);
  }

  int getIndex(const K &key, int i)
  {
    int x = hf(key) % ts;
    int step = aux_hf(key) % ts;

    if (step == 0)
      step = 1;

    if (way == "DOUBLE_HASHING")
      return (x + i * step) % ts;

    return (x + i * step + 31 * i * i) % ts;
  }

  void rehash(int ns)
  {
    vector<pair<K, V>> prev_tb = tb;
    vector<int> prev_con = con;

    ts = ns;
    tb.clear();
    con.clear();

    tb.resize(ts);
    con.resize(ts, 0);

    for (int i = 0; i < (int)prev_tb.size(); i++)
    {
      if (prev_con[i] == 1)
      {
        for (int j = 0; j < ts; j++)
        {
          int index = getIndex(prev_tb[i].first, j);

          if (con[index] == 0)
          {
            tb[index] = prev_tb[i];
            con[index] = 1;
            break;
          }
        }
      }
    }
  }

  void compress()
  {
    if ((double)ic / ts < 0.25 && ts > 13)
    {
      int ns = prev_prime(ts);

      if (ns >= 13)
        rehash(ns);
    }
  }

  void insert(const K &key, const V &value)
  {
    int deleted_index = -1;

    for (int i = 0; i < ts; i++)
    {
      int index = getIndex(key, i);

      if (con[index] == 1)
      {
        if (tb[index].first == key)
          return;

        col++;
      }
      else if (con[index] == 2)
      {
        if (deleted_index == -1)
          deleted_index = index;
      }
      else
      {
        if (deleted_index != -1)
          index = deleted_index;

        tb[index] = {key, value};
        con[index] = 1;
        ic++;

        if ((double)ic / ts > 0.5)
        {
          find_prime(ts);
          rehash(ts);
        }

        return;
      }
    }

    if (deleted_index != -1)
    {
      tb[deleted_index] = {key, value};
      con[deleted_index] = 1;
      ic++;
    }
  }

  bool del(const K &key)
  {
    for (int i = 0; i < ts; i++)
    {
      int index = getIndex(key, i);

      if (con[index] == 0)
        return false;

      if (con[index] == 1 && tb[index].first == key)
      {
        con[index] = 2;
        ic--;

        compress();
        return true;
      }
    }

    return false;
  }

  int search(const K &key)
  {
    int comp = 0;

    for (int i = 0; i < ts; i++)
    {
      int index = getIndex(key, i);
      comp++;

      if (con[index] == 0)
        return -1;

      if (con[index] == 1 && tb[index].first == key)
        return comp;
    }

    return -1;
  }
};

template <typename T, typename K>
double avg_hit(T &table, vector<K> &w)
{
  long long compare = 0;

  for (const K &x : w)
  {
    compare += table.search(x);
  }

  return (double)compare / w.size();
}

int main()
{
  mt19937 rng(50);
  vector<string> w;
  unordered_set<string> uw;

  while ((int)w.size() < 10000)
  {
    string x = ranword(10, rng);

    if (uw.insert(x).second)
      w.push_back(x);
  }

  chain_tb<string, int> c1(hash1);
  chain_tb<string, int> c2(hash2);

  open_addresstb<string, int> dh1(
      "DOUBLE_HASHING", hash1, aux_hash);

  open_addresstb<string, int> dh2(
      "DOUBLE_HASHING", hash2, aux_hash);

  open_addresstb<string, int> cus1(
      "CUSTOM_PROBING", hash1, aux_hash);

  open_addresstb<string, int> cus2(
      "CUSTOM_PROBING", hash2, aux_hash);

  for (int i = 0; i < 10000; i++)
  {
    c1.insert(w[i], i + 1);
    c2.insert(w[i], i + 1);

    dh1.insert(w[i], i + 1);
    dh2.insert(w[i], i + 1);

    cus1.insert(w[i], i + 1);
    cus2.insert(w[i], i + 1);
  }

  shuffle(w.begin(), w.end(), rng);
  w.resize(1000);

  cout << fixed << setprecision(2);

  cout << "Technique "
       << "H1 Collisions "
       << "H1 AvgHits "
       << "H2 Collisions "
       << "H2 AvgHits" << endl;

  cout << "Chaining "
       << c1.col << " "
       << avg_hit(c1, w) << " "
       << c2.col << " "
       << avg_hit(c2, w) << endl;

  cout << "Double Hashing "
       << dh1.col << " "
       << avg_hit(dh1, w) << " "
       << dh2.col << " "
       << avg_hit(dh2, w) << endl;

  cout << "Custom Probing "
       << cus1.col << " "
       << avg_hit(cus1, w) << " "
       << cus2.col << " "
       << avg_hit(cus2, w) << endl;
  return 0;
}
