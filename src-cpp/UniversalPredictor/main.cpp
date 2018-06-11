//
//  main.cpp
//  UniversalPredictor
//
//  Created by Aloÿs Augustin on 30/10/15.
//  Copyright © 2015 A. Augustin. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <random>
#include <vector>
#include <string>
#include <limits>

#define min(a, b) (((a) > (b)) ? (b) : (a))

template <typename T>
class SuffixTree;

template<typename T>
struct STNode {
    /*
     There is no need to create an "Edge" struct.
     Information about the edge is stored right in the node.
     [start; end) interval specifies the edge,
     by which the node is connected to its parent node.
     */
    
    int start, end, slink;
    int next[std::numeric_limits<T>::max()];
    
    int edge_length(SuffixTree<T> *owner) {
        return min(end, owner->pos + 1) - start;
    }
    
    std::string toString(SuffixTree<T> *owner, int num) {
        std::ostringstream o;
        for (int i = start; i < start + edge_length(owner); i ++) {
            o << owner->text[i];
        }
        o << num;
        return o.str();
    }
};

template <typename T>
class SuffixTree {
public:
    
    std::vector<STNode<T>> tree;
    std::vector<T> text;
    int root, last_added, pos, needSL, remainder, active_node, active_e, active_len;
    
    static const int oo = 0xFFFFFFF;

    
    SuffixTree() {
        needSL = 0;
        last_added = 0;
        pos = -1;
        remainder = 0;
        active_node = 0;
        active_e = 0;
        active_len = 0;
        root = active_node = new_node(-1, -1);
        STNode<T> n = {0};
        tree.push_back(n); //dummy node 0, root will be at 1
    }
    
    int new_node(int start, int end = oo) {
        STNode<T> nd;
        nd.start = start;
        nd.end = end;
        nd.slink = 0;
        for (int i = 0; i < std::numeric_limits<T>::max(); i++)
            nd.next[i] = 0;
        tree.push_back(nd);
        last_added ++;
        return last_added;
    }
    
    T active_edge() {
        return text[active_e];
    }
    
    void add_SL(int node) {
        if (needSL > 0) tree[needSL].slink = node;
        needSL = node;
    }
    
    bool walk_down(int node) {
        if (active_len >= tree[node].edge_length(this)) {
            active_e += tree[node].edge_length(this);
            active_len -= tree[node].edge_length(this);
            active_node = node;
            return true;
        }
        return false;
    }
    
    void extend(T c) {
        text.push_back(c);
        pos ++;
        needSL = 0;
        remainder++;
        while(remainder > 0) {
            if (active_len == 0) active_e = pos;
            if (tree[active_node].next[active_edge()] == 0) {
                int leaf = new_node(pos);
                tree[active_node].next[active_edge()] = leaf;
                add_SL(active_node); //rule 2
            } else {
                int nxt = tree[active_node].next[active_edge()];
                if (walk_down(nxt)) continue; //observation 2
                if (text[tree[nxt].start + active_len] == c) { //observation 1
                    active_len++;
                    add_SL(active_node); //observation 3
                    break;
                }
                int split = new_node(tree[nxt].start, tree[nxt].start + active_len);
                tree[active_node].next[active_edge()] = split;
                int leaf = new_node(pos);
                tree[split].next[c] = leaf;
                tree[nxt].start += active_len;
                tree[split].next[text[tree[nxt].start]] = nxt;
                add_SL(split); //rule 2
            }
            remainder--;
            if (active_node == root && active_len > 0) { //rule 1
                active_len--;
                active_e = pos - remainder + 1;
            } else
                active_node = tree[active_node].slink > 0 ? tree[active_node].slink : root; //rule 3
        }
    }

    std::string toDot(int cur = 1) {
        std::string r;
        if (cur == root) {
            r += "graph g {\n";
        }
        for (int i = 0; i < std::numeric_limits<T>::max(); i ++) {
            if (tree[cur].next[i] > 0) {
                int son = tree[cur].next[i];
                r += tree[cur].toString(this, cur) + " -- " + tree[son].toString(this, son) + "\n";
                r += toDot(son);
            }
        }
        if (cur == root) {
            r += "}\n";
        }
        return r;
    }

};

size_t longestSuffix(std::vector<bool> &vec) {
    size_t maxlen = 0;
    size_t maxend = 0;
    for (size_t i = vec.size() - 2; i > 0; i --) {
        size_t curlen = 0;
        while ((i-curlen) > 0 && vec[i-curlen] == vec[vec.size() - 1 - curlen]) {
            curlen ++;
        }
        if (curlen > maxlen) {
            maxlen = curlen;
            maxend = i;
        }
    }
    return maxend;
}

bool predictBit(std::vector<bool> &previous) {
    if (previous.size() > 1) {
        size_t n = longestSuffix(previous);
        return previous[n+1];
    } else {
        return true;
    }
}

void st_test() {
    char in = 's';
    SuffixTree<unsigned char> st = SuffixTree<unsigned char>();
    
    while (in != 'q') {
        std::cin >> in;
        st.extend(in);
    }
    std::cout << "Remainder " << st.remainder << std::endl;
    std::cout << st.toDot() << std::endl;
}

void bool_predictor_run() {
    char in = 's';
    int successes = 0;
    SuffixTree<bool> st = SuffixTree<bool>();
    
    while (in != 'q') {
        std::cin >> in;
        bool nextbit = true;// predictBit(st.text);
        st.extend(in);
        if (in == '0') {
            //st.extend(false);
            if (nextbit == false) {
                successes ++;
            }
            std::cout << "Got " << in << ", predicted " << (int) nextbit;
            std::cout << " success rate " << (float) successes * 100.0 / (float) st.text.size() << "%" << std::endl;
            
        } else if (in == '1') {
            //st.extend(true);
            if (nextbit == true) {
                successes ++;
            }
            std::cout << "Got " << in << ", predicted " << (int) nextbit;
            std::cout << " success rate " << (float) successes * 100.0 / (float) st.text.size() << "%" << std::endl;
        } else if (in == 'q') {
            std::cout << "Quitting." << std::endl;
            std::cout << st.toDot() << std::endl;
        } else {
            std::cout << "Unrecognized input." << std::endl;
        }
        //std::cout << st.remainder << std::endl;
    }
}

int main(int argc, const char * argv[]) {
    
    st_test();

    return 0;
}
