#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <avl.cpp>
//#include <red_black.cpp>
#include <QMap>

using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //define tools

    l1.setText("AVL");
    l2.setText("Red-Black");
    l3.setText("Treap");

    font = l1.font();
    font.setFamily("Courier");
    font.setPixelSize(25);

    l1.setFont(font);
    l2.setFont(font);
    l3.setFont(font);
    l1.setMaximumWidth(200);
    l2.setMaximumWidth(200);
    l3.setMaximumWidth(200);
    l1.setParent(this);
    l2.setParent(this);
    l3.setParent(this);

    b1.setParent(this);
    b2.setParent(this);
    b3.setParent(this);

    draw.setFont(font);
    draw.setText("Draw");
    draw.setMaximumWidth(200);
    draw.setParent(this);

    input.setFont(font);
    input.setMaximumWidth(1000);
    input.setParent(this);

    //define show
    this->setMinimumHeight(100);
    this->setMinimumWidth(1200);
    this->setWindowTitle("Show tree");
    this->setWindowState(Qt::WindowFullScreen);
    this->setStyleSheet("Widget {background-color: #FDFCFD}");

    W = this->width();
    H = this->height();
    SHIFT = std::min(W / 15, H / 15);

    insert.setText("Insert");
    del.setText("Delete");

    insert.setFont(font);
    del.setFont(font);
    input_num.setFont(font);

    insert.setParent(this);
    del.setParent(this);
    input_num.setParent(this);

    input_num.setGeometry(SHIFT, SHIFT, W / 4, H / 20);
    insert.setGeometry(SHIFT, 2 * SHIFT + H / 10, W / 8 - SHIFT, H / 20);
    del.setGeometry(W / 8 + SHIFT, 2 * SHIFT + H / 10, W / 8 - SHIFT, H / 20);

    b1.setGeometry(insert.x(), insert.y() + SHIFT, SHIFT, SHIFT);
    b2.setGeometry(b1.x(), b1.y() + SHIFT, b1.width(), b1.height());
    b3.setGeometry(b2.x(), b2.y() + SHIFT, b2.width(), b2.height());

    l1.setGeometry(b1.x() + SHIFT, b1.y(), input_num.width() - SHIFT - b1.width(), b1.height());
    l2.setGeometry(l1.x(), l1.y() + SHIFT, l1.width(), l1.height());
    l3.setGeometry(l2.x(), l2.y() + SHIFT, l2.width(), l2.height());

    draw.setGeometry(b3.x(), H - 2 * SHIFT, input_num.width(), SHIFT);
    input.setGeometry(b3.x(), b3.y() + b3.height() + SHIFT, input_num.width(), draw.y() - b3.y() - b3.height() - 2 * SHIFT);


    W0 = W - 3 * SHIFT - W / 4;
    H0 = H - 2 * SHIFT;
    X0 = input_num.x() + input_num.width() + SHIFT;
    Y0 = input_num.y();

    PM = QPixmap(W0, H0);

    need_redraw = 1;
    need_tree = 0;
    cur_tree = 2;

    QObject::connect(&input_num, SIGNAL(textChanged(QString)), this, SLOT(get_num(QString)));
    QObject::connect(&insert, SIGNAL(released()), this, SLOT(do_insert()));
    QObject::connect(&del, SIGNAL(released()), this, SLOT(do_del()));
    QObject::connect(&draw, SIGNAL(released()), this, SLOT(do_tree()));

    QObject::connect(&b1, SIGNAL(released()), this, SLOT(set_avl()));
    QObject::connect(&b2, SIGNAL(released()), this, SLOT(set_red_black()));
    QObject::connect(&b3, SIGNAL(released()), this, SLOT(set_treap()));

    //define trees
    avl_root = nullptr;
    avl_sz = 0;

    get_nil();
    red_black_root = nullptr;
    red_black_sz = 0;

    treap_root = nullptr;
    treap_sz = 0;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::resizeEvent(QResizeEvent *event) {
    W = this->width();
    H = this->height();
    SHIFT = std::min(W / 15, H / 15);

    input_num.setGeometry(SHIFT, SHIFT, W / 4, H / 20);
    insert.setGeometry(SHIFT, 2 * SHIFT + H / 20, W / 8 - SHIFT, H / 20);
    del.setGeometry(W / 8 + 2 * SHIFT, 2 * SHIFT + H / 20, W / 8 - SHIFT, H / 20);

    W0 = W - 3 * SHIFT - W / 4;
    H0 = H - 2 * SHIFT;
    X0 = input_num.x() + input_num.width() + SHIFT;
    Y0 = input_num.y();

    PM = QPixmap(W0, H0);
    need_redraw = 1;
    repaint();
}

void Widget::set_avl() {
    cur_tree = 0;
}

void Widget::set_red_black() {
    cur_tree = 1;
}

void Widget::set_treap() {
    cur_tree = 2;
}

void Widget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void Widget::paintEvent(QPaintEvent *event) {
    if (!need_redraw) {
        QPainter p(this);
        p.drawPixmap(X0, Y0, W0, H0, PM);
        return;
    }

    QPainter p(&PM);

    p.setPen(QColor(240, 240, 255));
    for (int i = 0; i < W0; ++i) {
        for (int j = 0; j < H0; ++j) {
            p.drawPoint(i, j);
        }
    }

    if (cur_tree == 0) {
        avl_print(avl_root, 0);
        std::cerr << "\n";

        //AVL draw
        QMap <int, pair <int, int>> coor;
        avl_get_h(avl_root, 0, coor);

        QMapIterator<int, pair <int, int>> i(coor);
        int cnt = 0, mx_h = 0;
        while (i.hasNext()) {
            i.next();
            mx_h = std::max(mx_h, i.value().first);
            coor[i.key()] = {i.value().first, cnt};
            ++cnt;
        }

        p.setPen(Qt::black);

        if (cnt) {
            int block_w = W0 / cnt, block_h = H0 / (mx_h + 1);
            int sh = std::max(block_w, block_h) / 10;
            int xc = block_w / 2 - min(block_w, block_h) / 2 + sh,
                    yc = block_h / 2 - min(block_w, block_h) / 2 + sh;

            QMapIterator<int, pair <int, int>> j(coor);
            while (j.hasNext()) {
                j.next();
                int x = j.value().second * block_w + xc;
                int y = j.value().first * block_h + yc;

                QRect tmp(x, y, block_w - 2 * xc, block_h - 2 * yc);
                p.drawText(tmp,
                           Qt::AlignCenter, QString::number(j.key()));
                p.drawRect(tmp);
            }

            QVector <pair <int, pair <int, int>>> edge;
            avl_get_edge(avl_root, edge);
            for (auto elem : edge) {
                int x = coor[elem.second.first].second * block_w + xc;
                int y = coor[elem.second.first].first * block_h + yc;
                int x0 = coor[elem.second.second].second * block_w + xc;
                int y0 = coor[elem.second.second].first * block_h + yc;
                int w = block_w - 2 * xc;
                int h = block_h - 2 * yc;
                if (elem.first == 0) {
                    //left edge
                    p.drawLine(x, y + h / 2, x0 + w / 2, y0);
                }
                else {
                    //right edge
                    p.drawLine(x + w, y + h / 2, x0 + w / 2, y0);
                }
            }
        }
    }
    else if (cur_tree == 1) {
        //Red-black
        red_black_print(red_black_root, 0);
        cerr << "\n";

        QMap <int, pair <int, pair <int, int>>> coor;
        red_black_get_h(red_black_root, 0, coor);

        QMapIterator<int, pair <int, pair <int, int>>> i(coor);
        int cnt = 0, mx_h = 0;
        while (i.hasNext()) {
            i.next();
            mx_h = std::max(mx_h, i.value().second.first);
            coor[i.key()] = {i.value().first, {i.value().second.first, cnt}};
            ++cnt;
        }


        if (cnt) {
            int block_w = W0 / cnt, block_h = H0 / (mx_h + 1);
            int sh = std::max(block_w, block_h) / 10;
            int xc = block_w / 2 - min(block_w, block_h) / 2 + sh,
                    yc = block_h / 2 - min(block_w, block_h) / 2 + sh;

            QMapIterator<int, pair <int, pair <int, int>>> j(coor);
            while (j.hasNext()) {
                j.next();
                int c = j.value().first;
                int x = j.value().second.second * block_w + xc;
                int y = j.value().second.first * block_h + yc;

                if (c == 0) {
                    //black
                    p.setPen(Qt::black);
                }
                else {
                    //red
                    p.setPen(Qt::red);
                }

                QRect tmp(x, y, block_w - 2 * xc, block_h - 2 * yc);
                p.drawText(tmp,
                           Qt::AlignCenter, QString::number(j.key()));
                p.drawRect(tmp);
            }

            p.setPen(Qt::black);

            QVector <pair <int, pair <int, int>>> edge;
            red_black_get_edge(red_black_root, edge);
            for (auto elem : edge) {
                int x = coor[elem.second.first].second.second * block_w + xc;
                int y = coor[elem.second.first].second.first * block_h + yc;
                int x0 = coor[elem.second.second].second.second * block_w + xc;
                int y0 = coor[elem.second.second].second.first * block_h + yc;
                int w = block_w - 2 * xc;
                int h = block_h - 2 * yc;
                if (elem.first == 0) {
                    //left edge
                    p.drawLine(x, y + h / 2, x0 + w / 2, y0);
                }
                else {
                    //right edge
                    p.drawLine(x + w, y + h / 2, x0 + w / 2, y0);
                }
            }
        }
    }
    else {
        treap_print(treap_root);
        //Treap draw

        QMap <int, pair <int, int>> coor;
        treap_get_h(treap_root, 0, coor);

        QMapIterator<int, pair <int, int>> i(coor);
        int cnt = 0, mx_h = 0;
        while (i.hasNext()) {
            i.next();
            mx_h = std::max(mx_h, i.value().first);
            coor[i.key()] = {i.value().first, cnt};
            ++cnt;
        }

        p.setPen(Qt::black);

        if (cnt) {
            int block_w = W0 / cnt, block_h = H0 / (mx_h + 1);
            int sh = std::max(block_w, block_h) / 10;
            int xc = block_w / 2 - min(block_w, block_h) / 2 + sh,
                    yc = block_h / 2 - min(block_w, block_h) / 2 + sh;

            QMapIterator<int, pair <int, int>> j(coor);
            while (j.hasNext()) {
                j.next();
                int x = j.value().second * block_w + xc;
                int y = j.value().first * block_h + yc;

                QRect tmp(x, y, block_w - 2 * xc, block_h - 2 * yc);
                p.drawText(tmp,
                           Qt::AlignCenter, QString::number(j.key()));
                p.drawRect(tmp);
            }

            QVector <pair <int, pair <int, int>>> edge;
            treap_get_edge(treap_root, edge);

            for (auto elem : edge) {
                int x = coor[elem.second.first].second * block_w + xc;
                int y = coor[elem.second.first].first * block_h + yc;
                int x0 = coor[elem.second.second].second * block_w + xc;
                int y0 = coor[elem.second.second].first * block_h + yc;
                int w = block_w - 2 * xc;
                int h = block_h - 2 * yc;
                if (elem.first == 0) {
                    //left edge
                    p.drawLine(x, y + h / 2, x0 + w / 2, y0);
                }
                else {
                    //right edge
                    p.drawLine(x + w, y + h / 2, x0 + w / 2, y0);
                }
            }
        }
    }

    QPainter p0(this);
    p0.drawPixmap(X0, Y0, W0, H0, PM);

    need_redraw = 0;
}

void Widget::get_num(QString s) {
    VAL = s.toInt();
}

void Widget::do_insert() {
    if (cur_tree == 0) {
        //AVL tree
        if (avl_insert0(avl_root, avl_sz, VAL)) {
            need_redraw = 1;
            repaint();
        }
    }
    else if (cur_tree == 1) {
        if (red_black_insert0(red_black_root, red_black_sz, VAL)) {
            need_redraw = 1;
            repaint();
        }
    }
    else {
        if (treap_insert(treap_root, VAL)) {
            ++treap_sz;
            need_redraw = 1;
            repaint();
        }
    }
}

void Widget::do_del() {
    if (cur_tree == 0) {
        //AVL tree
        if (avl_del0(avl_root, avl_sz, VAL)) {
            need_redraw = 1;
            repaint();
        }
    }
    else if (cur_tree == 1) {
        if (red_black_del0(red_black_root, red_black_sz, VAL)) {
            need_redraw = 1;
            repaint();
        }
    }
    else {
        if (treap_del(treap_root, VAL)) {
            --treap_sz;
            need_redraw = 1;
            repaint();
        }
    }
}

void Widget::get_tree() {
    input_tree = input.toPlainText();
    TREE.clear();
    int i = 0;

    while (i < input_tree.size()) {
        while (!input_tree[i].isNumber() && input_tree[i] != '-') {
            ++i;
        }
        int sign = 1, num = 0;
        if (input_tree[i] == '-') {
            sign = -1;
            ++i;
        }

        while (input_tree[i].isNumber()) {
            num *= 10;
            QString tmp = "";
            tmp += input_tree[i];
            num += tmp.toInt();
            ++i;
        }
        TREE.push_back(num * sign);
    }
    sort(TREE.begin(), TREE.end());
}

void Widget::do_tree() {
    get_tree();
    if (cur_tree == 0) {
        avl_build(avl_root, avl_sz, TREE);
        need_redraw = 1;
        repaint();
    }
    else if (cur_tree == 1) {
        red_black_build(red_black_root, red_black_sz, TREE);
        need_redraw = 1;
        repaint();
    }
    else {
        treap_build(treap_root, treap_sz, TREE);
        need_redraw = 1;
        repaint();
    }
}
