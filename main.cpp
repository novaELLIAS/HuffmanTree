#pragma GCC optimize ("Ofast", 3)
#pragma GCC target ("sse3","sse2","sse")
#pragma GCC target ("avx","sse4","sse4.1","sse4.2","ssse3")
#pragma GCC target ("f16c")

#include <bits/stdc++.h>
#include <bits/extc++.h>

using namespace std;
using namespace __gnu_pbds;

class Huffman {
private:
    int totalLeaf = 0;

    struct node {
        int name, wei;
        node *lson, *rson;
    } *root;

    struct cmp {
        inline bool operator() (const node& a, const node& b) {
            return a.wei < b.wei;
        }
    };

public:
    inline void build (const string&);
} tre;

string testString = "This object has escaped into fantasy. Next Dream...";

signed main() {
    tre.build(testString);
    return 0;
}

inline void Huffman::build (const string& src) {
    __gnu_pbds::priority_queue<node, cmp, pairing_heap_tag> que;
    while (!que.empty()) que.pop();

    int strCnt[512];
    memset(strCnt, 0, sizeof strCnt);

    register int len = src.length();
    for (register int i=0; i^len; ++ i) {
        if (!strCnt[src[i]]) ++ totalLeaf; ++ strCnt[src[i]];
    }

    register int cnt = 0;
    for (register int i=0; i^512; ++ i) {
        if (!strCnt[i]) {
            node *tmp = new node{i, strCnt[i], NULL, NULL};
            que.push(*tmp);
        }
    } cnt = 0;

    const node *a, *b;
    while (que.empty() > 1) {
        a = &que.top(); que.pop();
        b = &que.top(); que.pop();

        node *fa = new node{0, a->wei+b->wei, const_cast<node *>(a), const_cast<node *>(b)};
        que.push(*fa);
    } *root = que.top(); que.pop();

}