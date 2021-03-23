#ifndef AVL_H
#define AVL_H

struct avl_node {
    int val, sz;
    avl_node * L;
    avl_node * R;
    avl_node(int x = 0) {
        val = x;
        sz = 1;
        L = R = nullptr;
    }
};

int avl_get_sz(avl_node * p);

void avl_update(avl_node * &p);

void avl_small_R(avl_node * &p);

void avl_small_L(avl_node * &p);

void avl_big_R(avl_node * &p);
void avl_big_L(avl_node * &p);

void avl_rotate(avl_node * &p);

void avl_insert(avl_node * &p, int x);

bool avl_leaf(avl_node * &p);

int avl_del_L(avl_node * &p);
int avl_del_R(avl_node * &p);

void avl_del(avl_node * &p, int x);
void avl_get_h(avl_node * p, int h, QMap <int, std::pair <int, int>> &coor) ;

void avl_print(avl_node * &p, int h) ;
bool avl_find(avl_node * &p, int x) ;

bool avl_insert0(avl_node * &avl_root, int &avl_sz, int VAL) ;
bool avl_del0(avl_node * &avl_root, int &avl_sz, int VAL) ;

void avl_build(avl_node * &p, int &avl_sz, QVector <int> &a) ;
void avl_get_edge(avl_node * &p, QVector <std::pair <int, std::pair <int, int>>> &edge) ;

#endif // AVL_H
