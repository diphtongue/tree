#ifndef TREAP_H
#define TREAP_H

struct treap {
    treap * left;
    treap * right;
    int x, y;
    treap(int x0 = 0) {
        x = x0, y = rand();
        left = right = nullptr;
    }
};

treap * merge(treap * v1, treap * v2) ;
std::pair <treap *, treap *> split(treap * v, int x) ;

bool treap_find(treap * v, int x);
bool treap_insert(treap * &v, int x);
bool treap_del(treap * &v, int x);

void treap_print0(treap * v, int h) ;
void treap_print(treap * v);

void treap_build(treap * &v, int &sz, QVector <int> &a);
void treap_get_h(treap * p, int h, QMap <int, std::pair <int, int>> &coor);
void treap_get_edge(treap * &p, QVector <std::pair <int, std::pair <int, int>>> &edge);

#endif // TREAP_H
