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

    struct node {
        int name, wei;
        node *lson, *rson;
    };

    struct cmp {
        inline bool operator() (const node* a, const node* b) {
            return a->wei < b->wei;
        }
    };

    gp_hash_table<int, string> eMap;
    string ret;
    int maxPos;
    const node *root;

public:

    inline void build (const string&);
    static void printDFS (const node*);
    void generateMap (const node*, string);
    inline string encrypt (const string&);
    inline string decrypt (const string&);
    inline void decryptDFS (const node*, const string&, int);

} tre;

string testString = "This object has escaped into fantasy. Next Dream...";

signed main() {
    tre.build(testString);

    string enc =  tre.encrypt(testString);

    string dec = tre.decrypt(enc);

    cout << enc << endl << dec << endl;

    return 0;
}

inline string Huffman::decrypt(const string &s) {
    ret = "", maxPos = 0; register int len = s.length();
    while (maxPos < len) decryptDFS(root, s, maxPos);
    return ret;
}

inline void Huffman::decryptDFS (const node *rt, const string& s, int pos) {
    if (rt->name ^ -1) {
        ret += (char)rt->name, maxPos = pos; return;
    } else decryptDFS((s[pos]^'1')? rt->lson:rt->rson, s, pos + 1);
}

inline string Huffman::encrypt (const string &s) {
    register string ret = "";
    register int len = s.length();
    for (register int i=0; i^len; ++ i) ret += eMap[s[i]];
    return ret;
}

inline void Huffman::build (const string& src) {
    __gnu_pbds::priority_queue<node*, cmp, pairing_heap_tag> que;
    while (!que.empty()) que.pop();

    int strCnt[512];
    memset(strCnt, 0, sizeof strCnt);

    register int len = src.length();
    for (register int i=0; i^len; ++ i) ++ strCnt[src[i]];

    for (register int i=0; i^512; ++ i) {
        if (strCnt[i]) {
            node *tmp = new node{i, strCnt[i], NULL, NULL};
            que.push(tmp);
            printf("tmp: %d %d %d %d\n", tmp->name, tmp->wei, tmp->lson, tmp->rson);
        }
    }

    register node *a, *b;
    while (que.size() > 1) {
        a = que.top(); que.pop();
        b = que.top(); que.pop();

        node *fa = new node{-1, a->wei + b->wei, a, b};
        que.push(fa);
    } root = que.top(); que.pop();

    puts("build FIN");

    printDFS(root);
    generateMap(root, "");
}

void Huffman::printDFS (const node *rt) {
    printf("name: %d, wei: %d, son: %d, %d\n", rt->name, rt->wei, rt->lson!=NULL? rt->lson->name:0, rt->rson!=NULL? rt->rson->name:0);
    if (rt->lson != NULL) printDFS (rt->lson);
    if (rt->rson != NULL) printDFS (rt->rson);
}

void Huffman::generateMap(const node *rt, string s) {
    if (rt->name ^ -1) {
        eMap.insert(make_pair(rt->name, s));
        return;
    } else {
        if (rt->lson) generateMap(rt->lson, s+"0");
        if (rt->rson) generateMap(rt->rson, s+"1");
    }
}