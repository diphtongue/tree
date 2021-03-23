#ifndef RED_BLACK_H
#define RED_BLACK_H

struct red_black_node {
    red_black_node * left;
    red_black_node * right;
    red_black_node * parent;
    int val, color;
    red_black_node(int x = 0) {
        val = x, color = 1;
        left = right = parent = nullptr;
    }
};

void get_nil();

red_black_node * get_node(int x = 0);

void red_black_print(red_black_node * v, int h);

red_black_node * grandpa(red_black_node * v);
red_black_node * uncle(red_black_node * v) ;
red_black_node * bro(red_black_node * v) ;

int get_color(red_black_node * v) ;

void red_black_turn_L(red_black_node * v) ;
void red_black_turn_R(red_black_node * v) ;
bool red_black_leaf(red_black_node * v) ;

void insert1(red_black_node * & v);
void insert2(red_black_node * & v);
void insert3(red_black_node * & v);
void insert4(red_black_node * & v);

void red_black_find_insert(red_black_node * &v, int x) ;

void red_black_up(red_black_node * &v) ;

void fix_del(red_black_node * &v) ;
void swap_child(red_black_node * &v, red_black_node * &u) ;
void del_child(red_black_node * &v) ;

void del(red_black_node * &v, int x) ;
bool red_black_find(red_black_node * &p, int x) ;

bool red_black_insert0(red_black_node * &root, int &red_black_sz, int VAL) ;
bool red_black_del0(red_black_node * &root, int &red_black_sz, int VAL) ;

void red_black_build(red_black_node * &p, int &red_black_sz, QVector <int> &a);
void red_black_get_h(red_black_node * p, int h, QMap <int, std::pair <int, std::pair <int, int>>> &coor);
void red_black_get_edge(red_black_node * &p, QVector <std::pair <int, std::pair <int, int>>> &edge);


#endif // RED_BLACK_H
