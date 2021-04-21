#pragma GCC optimize ("Ofast", 3)
#pragma GCC target ("sse3","sse2","sse")
#pragma GCC target ("avx","sse4","sse4.1","sse4.2","ssse3")
#pragma GCC target ("f16c")

#include <bits/stdc++.h>
#include <bits/extc++.h>

using namespace std;
using namespace __gnu_pbds;

class utility {

    inline char int2char (int c) {
        if (c < 10) return (char)(c + '0');
        if (c < 36) return (char)(c - 10 + 'A');
        if (c < 62) return (char)(c - 36 + 'a');
        return (char)(c - 62 + '<');
    }

    inline int char2int (char c) {
        if (c <= '9') return c ^ '0';
        if (c <= 'Z') return 10 + c - 'A';
        if (c <= 'z') return 36 + c - 'a';
        return 62 + c - '<';
    }

public:

    inline string bit2fla (const string&);
    inline string fla2bit (const string&);

} util;

inline string utility::bit2fla (const string &src) {
    register int len = src.length();
    register int tmp = src[0] ^ '0';
    register string ret = "";
    for (register int i=1; i^len; ++ i) {
        if (!(i % 6)) ret += int2char(tmp), tmp = 0;
        tmp = (tmp << 1) + (src[i] ^ '0');
    } if (len % 6) {
        ret += "(";
        for (int i=6 * (len / 6); i^len; ++ i) ret += src[i];
        ret += ")";
    } return ret;
}

inline string utility::fla2bit (const string &src) {
    register int len = src.length(), tmp;
    register string ret = "";
    register stack<int> s;
    while (!s.empty()) s.pop();
    for (register int i=0; i^len; ++ i) {
        if (!(src[i]^'(')) {
            for (register int j=i+1; j<len-1; ++ j) ret += src[j];
            return ret;
        } tmp = char2int(src[i]);
        for (register int j=0; j^6; ++ j) {
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
            return a->wei < b->wei;
        }
    };

    gp_hash_table<int, string> eMap;
    string ret;
    int maxPos;
    const node *root;

public:

    inline void build (const string&);
    void generateMap (const node*, string);
    inline string encrypt (const string&);
    inline string decrypt (const string&);
    inline void decryptDFS (const node*, const string&, int);

} tre;

//const string testString = "This object has escaped into fantasy. Next Dream...";
const string testString = "nico nico nii! anata no haato ni nico nico nii! egao no todokeru yazawa nico nico! Nico Nii oboete Rabu nico!";

signed main() {
    tre.build(testString);
    string enc = util.bit2fla(tre.encrypt(testString));
    string dec = tre.decrypt(util.fla2bit(enc));
    //cout << util.bit2fla(enc) << endl << util.fla2bit(util.bit2fla(enc)) << endl;
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

    int strCnt[128];
    memset(strCnt, 0, sizeof strCnt);

    register int len = src.length();
    for (register int i=0; i^len; ++ i) ++ strCnt[src[i]];

    for (register int i=0; i^128; ++ i) {
        if (strCnt[i]) {
            node *tmp = new node{i, strCnt[i], NULL, NULL};
            que.push(tmp);
        }
    }

    register node *a, *b;
    while (que.size() > 1) {
        a = que.top(); que.pop(); b = que.top(); que.pop();
        node *fa = new node{-1, a->wei + b->wei, a, b};
        que.push(fa);
    } root = que.top(); que.pop();

    generateMap(root, "");
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