#pragma GCC optimize ("Ofast", 3)
#pragma GCC target ("sse3","sse2","sse")
#pragma GCC target ("avx","sse4","sse4.1","sse4.2","ssse3")
#pragma GCC target ("f16c")

#include <bits/stdc++.h>
#include <bits/extc++.h>

using namespace std;
using namespace __gnu_pbds;

//#define COUNTER_DEBUG
//#define TREE_BUILDING_DEBUG

class utility {

    static inline char int2char (int c) {
        if (c < 10) return (char)(c + '0');
        if (c < 36) return (char)(c - 10 + 'A');
        if (c < 62) return (char)(c - 36 + 'a');
        return (char)(c - 62 + '<');
    }

    static inline int char2int (char c) {
        if (c <= '9') return c ^ '0';
        if (c <= 'Z') return 10 + c - 'A';
        if (c <= 'z') return 36 + c - 'a';
        return 62 + c - '<';
    }

public:

    static inline string bit2fla (const string&);
    static inline string fla2bit (const string&);

};

inline string utility::bit2fla (const string &src) {
    int len = src.length();
    int tmp = src[0] ^ '0';
    string ret = "";
    for (int i=1; i^len; ++ i) {
        if (!(i % 6)) ret += int2char(tmp), tmp = 0;
        tmp = (tmp << 1) + (src[i] ^ '0');
    } if (len % 6) {
        ret += "(";
        for (int i=6 * (len / 6); i^len; ++ i) ret += src[i];
        ret += ")";
    } return ret;
}

inline string utility::fla2bit (const string &src) {
    int len = src.length(), tmp;
    string ret = "";
    stack<int> s;
    while (!s.empty()) s.pop();
    for (int i=0; i^len; ++ i) {
        if (!(src[i]^'(')) {
            for (int j=i+1; j<len-1; ++ j) ret += src[j];
            return ret;
        } tmp = char2int(src[i]);
        for (int j=0; j^6; ++ j) {
            s.push(tmp % 2); tmp >>= 1;
        } while (!s.empty()) {ret += (char)(s.top()+'0'); s.pop();}
    } return ret;
}

class Huffman {
private:

    struct node {
        int name, wei;
        node *lson, *rson;
    };

    struct cmp {
        inline bool operator() (const node* a, const node* b) {
            return a->wei > b->wei;
        }
    };

    gp_hash_table<int, string> eMap;
    string ret;
    int maxPos{};
    const node *root{};
    utility util;

public:

    inline void build (const string&);
    void generateMap (const node*, string);
    inline string encrypt (const string&);
    inline string decrypt (const string&);
    inline void decryptDFS (const node*, const string&, int);

    static void printDFS (const node*);

} tre;

const string testString = "I love data Structure, I love Computer 。 I will try my best to study data Structure.";
//const string testString = "This object has escaped into fantasy. Next Dream...";
//const string testString = "nico nico nii! anata no haato ni nico nico nii! egao no todokeru yazawa nico nico! Nico Nii oboete Rabu nico!";

signed main() {
    tre.build(testString);
    string enc = tre.encrypt(testString);
    string dec = tre.decrypt(enc);
    cout << enc << endl << dec << endl;
    return 0;
}

inline string Huffman::decrypt(const string &s) {
    string src = util.fla2bit(s); ret = "", maxPos = 0;
    int len = src.length();
    while (maxPos < len) decryptDFS(root, src, maxPos);
    return ret;
}

inline void Huffman::decryptDFS (const node *rt, const string& s, int pos) {
    if (rt->name ^ -1) {
        ret += (char)rt->name, maxPos = pos; return;
    } else decryptDFS((s[pos]^'1')? rt->lson:rt->rson, s, pos + 1);
}

inline string Huffman::encrypt (const string &src) {
    string ret = "";
    int len = src.length();
    for (int i=0; i^len; ++ i) ret += eMap[src[i]];
    return util.bit2fla(ret);
}

inline void Huffman::build (const string& src) {
    __gnu_pbds::priority_queue<node*, cmp, pairing_heap_tag> que;
    while (!que.empty()) que.pop();

    int strCnt[128];
    memset(strCnt, 0, sizeof strCnt);

    int len = src.length();
    for (int i=0; i^len; ++ i) ++ strCnt[src[i]];

    for (int i=0; i^128; ++ i) {
        if (strCnt[i]) {
            node *tmp = new node{i, strCnt[i], NULL, NULL};

            #ifdef COUNTER_DEBUG
                printf("this: %c, %d\n", i, strCnt[i]);
            #endif

            que.push(tmp);
        }
    }

    node *a, *b;
    while (que.size() > 1) {
        a = que.top(); que.pop(); b = que.top(); que.pop();
        node *fa = new node{-1, a->wei + b->wei, a, b};

        #ifdef TREE_BUILDING_DEBUG
            printf("QuefuncA: %c, %d, wei: %d\n", a->name, a->name, a->wei);
            printf("QuefuncB: %c, %d, wei: %d\n", b->name, b->name, b->wei);
            printf("QuefuncF: %c, %d, wei: %d\n", fa->name, fa->name, fa->wei);
        #endif
        que.push(fa);
    } root = que.top(); que.pop();

    generateMap(root, "");

    printDFS (root);
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

void Huffman::printDFS (const node* rt) {
    printf("print: name: %d, %c; %d, %c, %c\n", rt->name, rt->name, rt->wei,\
        rt->lson? rt->lson->name:'-', rt->rson? rt->rson->name:'-');
    if (rt->lson) printDFS (rt->lson);
    if (rt->rson) printDFS (rt->rson);
}