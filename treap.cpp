#include <iostream>
#include <ctime>
#include <QVector>
#include <QMap>
#include <treap.h>

using namespace std;

treap * merge(treap * v1, treap * v2) {
    if (v1 == nullptr) return v2;
    if (v2 == nullptr) return v1;

    if (v1->y < v2->y) {
        v2->left = merge(v1, v2->left);
        return v2;
    }
    else {
        v1->right = merge(v1->right, v2);
        return v1;
    }
}

pair <treap *, treap *> split(treap * v, int x) {
    if (v == nullptr) {
        return {nullptr, nullptr};
    }
    treap * left;
    treap * right;
    if (v->x <= x) {
        auto p = split(v->right, x);
        v->right = p.first;
        left = v;
        right = p.second;
    }
    else {
        auto p = split(v->left, x);
        left = p.first;
        v->left = p.second;
        right = v;
    }
    return {left, right};
}

bool treap_find(treap * v, int x) {
    if (v == nullptr) return 0;
    if (v->x == x) return 1;
    if (v->x < x) {
        return treap_find(v->right, x);
    }
    else {
        return treap_find(v->left, x);
    }
}

bool treap_insert(treap * &v, int x) {
    if (treap_find(v, x)) {
        return 0;
    }
    auto p = split(v, x);
    treap * new_node = new treap(x);
    v = merge(merge(p.first, new_node), p.second);
    return 1;
}

bool treap_del(treap * &v, int x) {
    if (!treap_find(v, x)) {
        return 0;
    }
    auto p1 = split(v, x);
    auto p2 = split(p1.first, x - 1);
    delete p2.second;

    v = merge(p2.first, p1.second);
    return 1;
}

void treap_print0(treap * v, int h) {
    if (v == nullptr) return;
    treap_print0(v->left, h + 1);
    for (int i = 0; i < h; ++i) cerr << " ";
    cerr << v->x << "(" << h << ")\n";
    treap_print0(v->right, h + 1);
}

void treap_build(treap * &v, int &sz, QVector <int> &a) {
    v = nullptr;
    sz = 0;
    for (auto i : a) {
        if (treap_insert(v, i)) {
            ++sz;
        }
    }
}

void treap_print(treap * v) {
    treap_print0(v, 0);
    cerr << "\n";
}

void treap_get_h(treap * p, int h, QMap <int, pair <int, int>> &coor) {
    if (p == nullptr) return;
    treap_get_h(p->left, h + 1, coor);
    coor[p->x] = {h, -1};
    treap_get_h(p->right, h + 1, coor);
}

void treap_get_edge(treap * &p, QVector <pair <int, pair <int, int>>> &edge) {
    if (p == nullptr) return;
    if (p->left != nullptr) {
        edge.push_back({0, {p->x, p->left->x}});
        treap_get_edge(p->left, edge);
    }
    if (p->right != nullptr) {
        edge.push_back({1, {p->x, p->right->x}});
        treap_get_edge(p->right, edge);
    }
}
