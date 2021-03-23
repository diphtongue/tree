#include <iostream>
#include <QString>

using namespace std;

int avl_get_sz(avl_node * p) {
    if (p == nullptr) return 0;
    return p->sz;
}

void avl_update(avl_node * &p) {
    if (p == nullptr) return;
    p->sz = max(avl_get_sz(p->L), avl_get_sz(p->R)) + 1;
}

void avl_small_R(avl_node * &p) {
    avl_node * q = p->L;
    p->L = q->R;
    avl_update(p);
    q->R = p;
    avl_update(q);
    p = q;
}

void avl_small_L(avl_node * &p) {
    avl_node * q = p->R;
    p->R = q->L;
    avl_update(p);
    q->L = p;
    avl_update(q);
    p = q;
}

void avl_big_R(avl_node * &p) {
    avl_node * q = p->L;
    avl_node * t = q->R;

    p->L = t->R;
    q->R = t->L;

    avl_update(p);
    avl_update(q);

    t->R = p;
    t->L = q;

    avl_update(t);

    p = t;
}

void avl_big_L(avl_node * &p) {
    avl_node * q = p->R;
    avl_node * t = q->L;

    p->R = t->L;
    q->L = t->R;

    avl_update(p);
    avl_update(q);

    t->R = q;
    t->L = p;

    avl_update(t);

    p = t;
}

void avl_rotate(avl_node * &p) {
    int hL = avl_get_sz(p->L);
    int hR = avl_get_sz(p->R);

    if (hL - hR == 2) {
        int h = hR;
        if (avl_get_sz(p->L->L) == h && avl_get_sz(p->L->R) == h + 1) {
            avl_big_R(p);
        }
        else {
            avl_small_R(p);
        }
    }
    else if (hR - hL == 2) {
        int h = hL;
        if (avl_get_sz(p->R->R) == h && avl_get_sz(p->R->L) == h + 1) {
            avl_big_L(p);
        }
        else {
            avl_small_L(p);
        }
    }
}

void avl_insert(avl_node * &p, int x) {
    if (p->val == x) return;
    if (p->val < x) {
        if (p->R == nullptr) {
            avl_node * new_node = new avl_node(x);
            p->R = new_node;

            avl_update(p);
            avl_rotate(p);
            return;
        }
        avl_insert(p->R, x);
        avl_update(p);
        avl_rotate(p);
    }
    else {
        if (p->L == nullptr) {
            avl_node * new_node = new avl_node(x);
            p->L = new_node;

            avl_update(p);
            avl_rotate(p);
            return;
        }
        avl_insert(p->L, x);
        avl_update(p);
        avl_rotate(p);
    }
}

bool avl_leaf(avl_node * &p) {
    return p->L == nullptr && p->R == nullptr;
}

int avl_del_L(avl_node * &p) {
    if (avl_leaf(p->L)) {
        int ans = p->L->val;
        delete p->L;
        p->L = nullptr;

        avl_update(p);
        avl_rotate(p);

        return ans;
    }
    int ans = avl_del_L(p->L);
    avl_update(p);
    avl_rotate(p);

    return ans;
}

int avl_del_R(avl_node * &p) {
    if (avl_leaf(p->R)) {
        int ans = p->R->val;
        delete p->R;
        p->R = nullptr;

        avl_update(p);
        avl_rotate(p);

        return ans;
    }
    int ans = avl_del_R(p->R);
    avl_update(p);
    avl_rotate(p);

    return ans;
}

void avl_del(avl_node * &p, int x) {
    if (p == nullptr) {
        return;
    }
    if (p->val == x) {
        if (avl_leaf(p)) {
            delete p;
            return;
        }
        if (p->L != nullptr) {
            if (avl_leaf(p->L)) {
                p->val = p->L->val;
                delete p->L;
                p->L = nullptr;

                avl_update(p);
                avl_rotate(p);
                return;
            }
            avl_node * q = p->L;
            p->val = avl_del_R(q);

            avl_update(p);
            avl_rotate(p);
            return;
        }
        else {
            if (avl_leaf(p->R)) {
                p->val = p->R->val;
                delete p->R;
                p->R = nullptr;

                avl_update(p);
                avl_rotate(p);
                return;
            }
            avl_node * q = p->R;
            p->val = avl_del_L(q);

            avl_update(p);
            avl_rotate(p);
            return;
        }
        return;
    }
    if (p->val < x) {
        if (avl_leaf(p->R)) {
            avl_del(p->R, x);
            p->R = nullptr;
        }
        else {
            avl_del(p->R, x);
        }
    }
    else {
        if (avl_leaf(p->L)) {
            avl_del(p->L, x);
            p->L = nullptr;
        }
        else {
            avl_del(p->L, x);
        }
    }

    avl_update(p);
    avl_rotate(p);
}

void avl_get_h(avl_node * p, int h, QMap <int, pair <int, int>> &coor) {
    if (p == nullptr) return;
    avl_get_h(p->L, h + 1, coor);
    coor[p->val] = {h, -1};
    avl_get_h(p->R, h + 1, coor);
}

void avl_print(avl_node * &p, int h) {
    if (p == nullptr) return;
    avl_print(p->L, h + 1);
    for (int i = 0; i < h; ++i) cerr << " ";
    cerr << p->val << "(" << p->sz << ")\n";
    avl_print(p->R, h + 1);
}

bool avl_find(avl_node * &p, int x) {
    if (p == nullptr) return 0;
    if (p->val == x) return 1;
    if (p->val < x) {
        return avl_find(p->R, x);
    }
    else {
        return avl_find(p->L, x);
    }
}

bool avl_insert0(avl_node * &avl_root, int &avl_sz, int VAL) {
    if (avl_sz == 0) {
        avl_root = new avl_node(VAL);
        ++avl_sz;

        return 1;
    }
    else {
        if (!avl_find(avl_root, VAL)) {
            avl_insert(avl_root, VAL);
            ++avl_sz;

            return 1;
        }
    }
    return 0;
}

bool avl_del0(avl_node * &avl_root, int &avl_sz, int VAL) {
    if (avl_sz == 0) {
        return 0;
    }
    else if (avl_sz == 1) {
        if (avl_root->val == VAL) {
            --avl_sz;
            delete avl_root;
            avl_root = nullptr;

            return 1;
        }
    }
    else {
        if (avl_find(avl_root, VAL)) {
            avl_del(avl_root, VAL);
            --avl_sz;

            return 1;
        }
    }
    return 0;
}

void avl_build(avl_node * &p, int &avl_sz, QVector <int> &a) {
    p = nullptr;
    avl_sz = 0;
    for (auto i : a) {
        avl_insert0(p, avl_sz, i);
    }
}

void avl_get_edge(avl_node * &p, QVector <pair <int, pair <int, int>>> &edge) {
    if (p == nullptr) return;
    if (p->L != nullptr) {
        edge.push_back({0, {p->val, p->L->val}});
        avl_get_edge(p->L, edge);
    }
    if (p->R != nullptr) {
        edge.push_back({1, {p->val, p->R->val}});
        avl_get_edge(p->R, edge);
    }
}
