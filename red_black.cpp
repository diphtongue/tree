#include <iostream>
#include <QVector>
#include <QMap>
#include <red_black.h>

using namespace std;

red_black_node * nil;

void get_nil() {
    nil = new red_black_node();
    nil->color = 0;
    nil->left = nil->right = nil;
    nil->parent = nullptr;
}

red_black_node * get_node(int x) {
    red_black_node * v = new red_black_node(x);
    v->left = v->right = nil;
    return v;
}

void red_black_print(red_black_node * v, int h);

red_black_node * grandpa(red_black_node * v) {
    if (v != nullptr && v->parent != nullptr) {
        return v->parent->parent;
    }
    return nullptr;
}

red_black_node * uncle(red_black_node * v) {
    red_black_node * g = grandpa(v);
    if (g == nullptr) {
        return nullptr;
    }
    if (g->left == v) {
        if (g->right != nil) {
            return g->right;
        }
        return nullptr;
    }
    else {
        if (g->left != nil) {
            return g->left;
        }
        return nullptr;
    }
}

red_black_node * bro(red_black_node * v) {
    if (v == nullptr || v->parent == nullptr) {
        return nullptr;
    }
    if (v == v->parent->left) {
        return v->parent->right;
    }
    else {
        return v->parent->left;
    }
}

int get_color(red_black_node * v) {
    if (v == nullptr) return 0;
    return v->color;
}

void red_black_turn_L(red_black_node * v) {
    if (v == nullptr) {
        return;
    }
    red_black_node * R = v->right;
    R->parent = v->parent;

    if (v->parent != nullptr) {
        if (v->parent->left == v) {
            v->parent->left = R;
        }
        else {
            v->parent->right = R;
        }
    }
    v->right = R->left;
    if (R->left != nullptr) {
        R->left->parent = v;
    }
    v->parent = R;
    R->left = v;
}

void red_black_turn_R(red_black_node * v) {
    if (v == nullptr) {
        return;
    }
    red_black_node * L = v->left;
    L->parent = v->parent;

    if (v->parent != nullptr) {
        if (v->parent->left == v) {
            v->parent->left = L;
        }
        else {
            v->parent->right = L;
        }
    }
    v->left = L->right;
    if (L->right != nullptr) {
        L->right->parent = v;
    }
    v->parent = L;
    L->right = v;
}

bool red_black_leaf(red_black_node * v) {
    if (v == nullptr || v == nil) return 0;
    return v->left == nil && v->right == nil;
}

void insert1(red_black_node * & v);
void insert2(red_black_node * & v);
void insert3(red_black_node * & v);
void insert4(red_black_node * & v);


void red_black_find_insert(red_black_node * &v, int x) {
    if (v == nullptr || v == nil || v->val == x) {
        return;
    }
    if (v->val < x) {
        if (v->right == nullptr || v->right == nil) {
            red_black_node * new_node = get_node(x);
            new_node->parent = v;
            v->right = new_node;

            insert1(new_node);
            return;
        }
        red_black_find_insert(v->right, x);
    }
    else {
        if (v->left == nullptr || v->left == nil) {
            red_black_node * new_node = get_node(x);
            new_node->parent = v;
            v->left = new_node;

            insert1(new_node);
            return;
        }
        red_black_find_insert(v->left, x);
    }
}

void insert1(red_black_node * &v) {
    if (v->parent == nullptr) {
        v->color = 0;
        return;
    }
    if (v->parent->color == 0) {
        return;
    }
    insert2(v);
}

void insert2(red_black_node * &v) {
    red_black_node * u = uncle(v);

    if (u != nullptr && u->color == 1) {
        red_black_node * g = grandpa(v);

        g->color = 1;
        v->parent->color = 0;
        u->color = 0;

        insert1(g);
        return;
    }
    insert3(v);
}

void insert3(red_black_node * &v) {
    red_black_node * g = grandpa(v);

    if (v->parent->left == v && g->right == v->parent) {
        red_black_turn_R(v->parent);
        v = v->right;
        return;
    }
    if (v->parent->right == v && g->left == v->parent) {
        red_black_turn_L(v->parent);
        v = v->left;
        return;
    }
    insert4(v);
}

void insert4(red_black_node * &v) {
    red_black_node * g = grandpa(v);
    g->color = 1;

    v->parent->color = 0;
    if (v->parent->left == v && v->parent == g->left) {
        red_black_turn_R(g);
    }
    else {
        red_black_turn_L(g);
    }
}

void red_black_up(red_black_node * &v) {
    while (v != nullptr && v->parent != nullptr) {
        v = v->parent;
    }
}

void red_black_print(red_black_node * v, int h) {
    if (v == nullptr) return;
    if (v == nil) {
        for (int i = 0; i < 2 * h; ++i) cerr << ' ';
        cerr << "nil\n";
        return;
    }
    red_black_print(v->left, h + 1);
    for (int i = 0; i < 2 * h; ++i) cerr << ' ';
    cerr << v->val << "(" << v->color << ")\n";
    red_black_print(v->right, h + 1);
}

void fix_del(red_black_node * &v) {
    red_black_node * p = v->parent;
    if (p == nullptr) {
        return;
    }
    red_black_node * s = bro(v);
    if (s->color == 1) {
        p->color = 1;
        s->color = 0;

        if (v == p->left) {
            red_black_turn_L(p);
        }
        else {
            red_black_turn_R(p);
        }
    }
    s = bro(v), p = v->parent;

    if (s->color == 0 &&
        s->left->color == 0 &&
        s->right->color == 0) {

        if (p->color == 0) {
            s->color = 1;
            fix_del(p);
        }
        else  {
            s->color = 1;
            p->color = 0;
        }
    }
    else {
        s = bro(v), p = v->parent;

        if (s->right->color == 0 && v == p->left) {
            s->color = 1;
            s->left->color = 0;
            red_black_turn_R(s);
        }
        else if (s->left->color == 0 && v == p->right) {
            s->color = 1;
            s->right->color = 0;
            red_black_turn_L(s);
        }

        s = bro(v), p = v->parent;
        s->color = p->color;
        p->color = 0;

        if (v == p->left) {
            s->right->color = 0;
            red_black_turn_L(p);
        }
        else {
            s->left->color = 0;
            red_black_turn_R(p);
        }
    }
}

void swap_child(red_black_node * &v, red_black_node * &u) {
    u->parent = v->parent;
    if (v->parent != nullptr) {
        if (v->parent->left == v) {
            v->parent->left = u;
        }
        else {
            v->parent->right = u;
        }
    }
}

void del_child(red_black_node * &v) {
    red_black_node * child;
    if (v->left == nil) {
        child = v->right;
    }
    else {
        child = v->left;
    }

    swap_child(v, child);

    if (v->color == 0) {
        if (child->color == 1) {
            child->color = 0;
            v = child;
        }
        else {
            fix_del(child);
        }
    }
}

void del(red_black_node * &v, int x) {
    while (v->val != x) {
        if (v->val < x) {
            v = v->right;
        }
        else {
            v = v->left;
        }
    }

    if (v->left == nil || v->right == nil) {
        del_child(v);
    }
    else {
        red_black_node * q = v->right;
        while (q->left != nil)
            q = q->left;

        v->val = q->val;
        del_child(q);
    }

    red_black_up(v);
    v->color = 0;
}

bool red_black_find(red_black_node * &p, int x) {
    if (p == nullptr || p == nil) return 0;
    if (p->val == x) return 1;
    if (p->val < x) {
        return red_black_find(p->right, x);
    }
    else {
        return red_black_find(p->left, x);
    }
}

bool red_black_insert0(red_black_node * &root, int &red_black_sz, int VAL) {
    if (red_black_sz == 0) {
        root = get_node(VAL);
        root->color = 0;
        ++red_black_sz;
        return 1;
    }
    else {
        if (!red_black_find(root, VAL)) {
            red_black_find_insert(root, VAL);
            red_black_up(root);
            ++red_black_sz;
            return 1;
        }
    }
    return 0;
}

bool red_black_del0(red_black_node * &root, int &red_black_sz, int VAL) {
    if (red_black_sz == 0) {
        return 0;
    }
    else if (red_black_sz == 1) {
        if (root->val == VAL) {
            delete root;
            root = nullptr;
            --red_black_sz;
            return 1;
        }
    }
    else {
        if (red_black_find(root, VAL)) {
            del(root, VAL);
            red_black_up(root);
            --red_black_sz;
            return 1;
         }
    }
    return 0;
}

void red_black_build(red_black_node * &p, int &red_black_sz, QVector <int> &a) {
    p = nullptr;
    red_black_sz = 0;
    for (auto i : a) {
        red_black_insert0(p, red_black_sz, i);
    }
}

void red_black_get_h(red_black_node * p, int h, QMap <int, pair <int, pair <int, int> > > &coor) {
    if (p == nullptr || p == nil) return;
    red_black_get_h(p->left, h + 1, coor);
    coor[p->val] = {p->color, {h, -1}};
    red_black_get_h(p->right, h + 1, coor);
}

void red_black_get_edge(red_black_node * &p, QVector <pair <int, pair <int, int>>> &edge) {
    if (p == nullptr || p == nil) return;
    if (p->left != nullptr && p->left != nil) {
        edge.push_back({0, {p->val, p->left->val}});
        red_black_get_edge(p->left, edge);
    }
    if (p->right != nullptr && p->right != nil) {
        edge.push_back({1, {p->val, p->right->val}});
        red_black_get_edge(p->right, edge);
    }
}
