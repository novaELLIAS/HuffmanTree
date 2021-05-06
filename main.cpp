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
//#define PRINT_TREE
//#define LOGIC_PROCESS_DEBUG
//#define IO_PROCESS_DEBUG

class utility {
private:
    static inline char int2char (int c) {
        if (c < 10) return (char)(c + '0');
        if (c < 36) return (char)(c - 10 + 'A');
        if (c < 62) return (char)(c - 36 + 'a');
        return (char)(c - 62 + '<');
    }
    static inline int char2int (char c) {
        if (c <= '9') return c - '0';
        if (c <= '=') return 62 + c - '<';
        if (c <= 'Z') return 10 + c - 'A';
        return 36 + c - 'a';
    }
    static inline string dec8bit (unsigned char c);
    inline string strFill8bit (string);

public:
    static inline string bit2fla (const string&);
    static inline string fla2bit (const string&);
    static inline void printStrAs6(const string&);
    static inline void printStrAs8(const string &);
    inline int getRealLen (const string&);
    inline void bitWriter (const string&, const string&);
    inline string bitReader (const string&);
} util;

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

    ofstream treeDoc;
    gp_hash_table<int, string> eMap;
//    string ret;
//    int maxPos{};
    const node *root{};
    utility util;

    void ex_preorder (const node*);

public:
    inline void build (const string&);
    void generateMap (const node*, string);
    inline void saveTree (const node*);
    inline string encrypt (const string&);
    //inline string decrypt (const string&);
    //inline void decryptDFS (const node*, const string&, int);
    static void distructDFS(const node*);
    ~Huffman() {distructDFS (root);}
    static void printDFS (const node*);
};

class HuffmanDecryptor {
private:
    struct node {
        int name;
        node *lson, *rson;
    } *root;
    string ret = "";
    int maxPos = 0, pos = 0;

    void buildDFS (node*&, const string&);
    void decryptDFS (const node*, const string&, int);
    static void distructDFS (const node*);

public:
    inline void build (const string&);
    inline string decrypt (const string&);
    void printDFS(const node*);
    ~HuffmanDecryptor() {distructDFS (root);}
};

void localTest () {
    const string testString[5] = {
            "QwQniconiconi",
            "I love data Structure. I love Computer. I will try my best to study data Structure.",
            "This object has escaped into fantasy.\nNext Dream...",
            "nananananananananananananananananananananananananananananananananananana~",
            "nico nico nii! anata no haato ni nico nico nii! egao no todokeru yazawa nico nico! Nico Nii oboete Rabu nico!"
    }; for (int i=0; i^5; ++ i) {
        cout << "lev " << i << endl;
        Huffman tre;
        tre.build(testString[i]);
#ifdef LOGIC_PROCESS_DEBUG
        cout << "tre build fin." << endl;
#endif
        string enc = tre.encrypt(testString[i]);
#ifdef LOGIC_PROCESS_DEBUG
        cout << "tre enc fin." << endl;
#endif
        HuffmanDecryptor decryptor;
        decryptor.build("tree.bin");
#ifdef LOGIC_PROCESS_DEBUG
        cout << "decryptor build fin." << endl;
#endif
        string dec = decryptor.decrypt("enc.bin");
        cout << testString[i] << endl << dec << endl;
    }
}

signed main () {
    localTest ();

    ifstream inFile("../src/in.txt", ios::in);
    ostringstream temp;
    temp << inFile.rdbuf();
    string content = temp.str();

    Huffman tre; tre.build(content);
    string enc = tre.encrypt(content);

    ofstream outFile("../src/out_base6.bin", ios::out | ios::binary);
    outFile << util.bit2fla(enc); outFile.close();
    outFile.open("../src/out_base6.bit", ios::out);
    outFile << util.bit2fla(enc); outFile.close();

    HuffmanDecryptor decryptor;
    decryptor.build("tree.bin");
    decryptor.decrypt("enc.bin");
}


void HuffmanDecryptor::distructDFS(const node *rt) {
    if (rt->lson) distructDFS(rt->lson);
    if (rt->rson) distructDFS(rt->rson);
    delete rt;
}

inline string HuffmanDecryptor::decrypt(const string &fileDic) {
//    ifstream inFile("../src/" + fileDic, ios::in | ios::binary);
//    ostringstream temp; temp << inFile.rdbuf();
//    const string str = temp.str();
     const string str = util.bitReader(fileDic);
    ret = "", maxPos = 0; register int len = util.getRealLen(str);//str.length();
    while (maxPos < len) decryptDFS(root, str, maxPos);
    ofstream outFile("../src/out.txt", ios::out);
    outFile << ret; outFile.close();
    return ret;
}

void HuffmanDecryptor::decryptDFS(const node *rt, const string &s, int pos) {
    if (rt->name ^ -1) {
        ret += (char)rt->name, maxPos = pos; return;
    } else decryptDFS((s[pos]^'1')? rt->lson:rt->rson, s, pos + 1);
}

void HuffmanDecryptor::buildDFS(node*& rt, const string &s) {
    register char c = s[pos ++];
    if (c ^ (char)(-2)) {
        rt = new node{c, NULL, NULL};
        buildDFS(rt->lson, s); buildDFS(rt->rson, s);
    } else rt = NULL;
}

inline void HuffmanDecryptor::build(const string& fileDic) {
    ifstream inFile("../src/" + fileDic, ios::in | ios::binary);
    ostringstream temp;
    temp << inFile.rdbuf(); inFile.close();
    const string preorder = temp.str();
    buildDFS(root, preorder);
#ifdef PRINT_TREE
    printDFS(root);
#endif
}

void HuffmanDecryptor::printDFS (const node* rt) {
    printf("print: name: %d, %c; %c, %c\n", rt->name, rt->name,\
        rt->lson? rt->lson->name:'-', rt->rson? rt->rson->name:'-');
    if (rt->lson) printDFS (rt->lson);
    if (rt->rson) printDFS (rt->rson);
}

inline void Huffman::saveTree(const node *rt) {
    treeDoc.open("../src/tree.bin", ios::out | ios::binary);
    ex_preorder(rt);
    treeDoc.close();
}

void Huffman::ex_preorder(const node *rt) {
    if(rt == NULL) {treeDoc << (char)(-2); return;}
    treeDoc << (char)rt->name;
    ex_preorder(rt->lson);
    ex_preorder(rt->rson);
}

inline string Huffman::encrypt (const string &src) {
    string ret = "";
    int len = src.length();
    for (int i=0; i^len; ++ i) ret += eMap[src[i]];
//    ofstream outFile("../src/enc.bin", ios::out | ios::binary);
//    outFile << ret; outFile.close();
    util.bitWriter(ret, "enc.bin");
    return ret;
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

#ifdef PRINT_TREE
    printDFS(root);
#endif
    saveTree(root);
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

void Huffman::distructDFS(const node *rt) {
    if (rt->lson) distructDFS(rt->lson);
    if (rt->rson) distructDFS(rt->rson);
    delete rt;
}

inline void utility::printStrAs6(const string &s) {
    int len = s.length(); cout << s[0];
    for (int i=1; i^len; ++ i) {
        if (!(i % 6)) printf(" %d:", i/6);
        cout << s[i];
    } putchar('\n');
}

inline void utility::printStrAs8(const string &s) {
    int len = s.length(); cout << s[0];
    for (int i=1; i^len; ++ i) {
        if (!(i % 8)) {
            if (!(i%64)) putchar('\n');
            printf(" %d:", i/8);
        }
        cout << s[i];
    } putchar('\n');
}

inline string utility::bit2fla (const string &src) {
    int len = src.length(), tmp = src[0] ^ '0';
    string ret = "";
    for (int i=1; i^len; ++ i) {
        if (!(i % 6)) ret += int2char(tmp), tmp = 0;
        tmp = (tmp << 1) + (src[i] ^ '0');
    } if (len % 6) {
        ret += "(";
        for (int i = 6*(len/6); i^len; ++ i) ret += src[i];
        ret += ")";
    } return ret;
}

inline string utility::fla2bit (const string &src) {
    int len = src.length(), tmp;
    string ret = ""; stack<int> s;
    while (!s.empty()) s.pop();
    for (int i=0; i^len; ++ i) {
        if (!(src[i]^'(')) {
            for (int j=i+1; j<len-1; ++ j) ret += src[j];
            return ret;
        } tmp = char2int(src[i]);
        for (int j=0; j^6; ++ j) s.push(tmp % 2), tmp >>= 1;
        while (!s.empty()) {ret += (char)(s.top()+'0'); s.pop();}
    } return ret;
}

inline string utility::dec8bit(unsigned char c) {
    register string ret = ""; register int cnt = 8;
    register stack<int> s; while (!s.empty()) s.pop();
    while (cnt --) {s.push(c&1); c >>= 1;}
    while (!s.empty()) {ret += char(s.top() + '0'); s.pop();}
    return ret;
}

inline string utility::bitReader(const string &FileName) {
    ifstream inFile("../src/" + FileName, ios::in | ios::binary);
    ostringstream temp;
    temp << inFile.rdbuf();
    string s = temp.str();
//    ifstream inFile("../src/" + FileName, ios::in | ios::binary);
//    register string file = "../src/" + FileName;
//    freopen (file.c_str(), "rb", stdin);
    register string ret = "";
    register int len = s.length();
    for (int i=0; i^len; ++ i) ret += dec8bit(s[i]);
    inFile.close(); return ret;
}

inline void utility::bitWriter(const string &src, const string &FileName) {
    ofstream ouFile("../src/" + FileName, ios::out | ios::binary);
    register string s = strFill8bit(src);
#ifdef IO_PROSESS_DEBUG
    //cout << "src: " << src << " " << src.length() << endl;
    //cout << "s:   " << s << " " << s.length() << endl;
    cout << "src: "; printStrAs8(src); cout << " " << src.length() << endl;
    cout << "s  : "; printStrAs8(  s); cout << " " <<   s.length() << endl;
#endif
    register int len = s.length();
    register unsigned char tmp = s[0] ^ '0';
    for (int i=1; i^len; ++ i) {
        //if (!(i % 8)) ouFile << tmp, tmp = 0;
        if (!(i % 8)) ouFile.write(reinterpret_cast<const char *>(&tmp), sizeof tmp), tmp = 0;
        tmp = (tmp << 1) | (s[i] ^ '0');
    } if (tmp) ouFile.write(reinterpret_cast<const char *>(&tmp), sizeof tmp); ouFile.close(); return;
}

inline int utility::getRealLen(const string &s) {
#ifdef IO_PROCESS_DEBUG
    cout << "ret: "; util.printStrAs8(s); cout << " " << s.length() << endl;
#endif
    register int len = s.length() - 1;
    while (s[len --] ^ '1');
#ifdef IO_PROCESS_DEBUG
    cout << "len: " << len + 1 << endl;
#endif
    return len + 1;
}

inline string utility::strFill8bit(string s) {
    register string ret = s;
    register int len = ret.length() % 8;
    if (len) {
        ret += "1"; ++ len;
        while (len % 8) ret += "0", ++ len;
    } else ret += "10000000";
    return ret;
}