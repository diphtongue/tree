#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QPainter>
#include <avl.h>
#include <red_black.h>
#include <treap.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    //tools

    QPlainTextEdit input;
    QPushButton draw;
    QLabel l1, l2, l3;
    QRadioButton b1, b2, b3;
    QFont font;

    //show
    int W, H, SHIFT, W0, H0, X0, Y0;
    int VAL;
    QPixmap PM;
    QPushButton insert, del;
    QLineEdit input_num;
    bool need_redraw, need_tree;
    int cur_tree;
    QVector <int> TREE;
    QString input_tree;

    //trees

    avl_node * avl_root;
    int avl_sz;

    red_black_node * red_black_root;
    int red_black_sz;

    treap * treap_root;
    int treap_sz;

    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

public slots:
    void get_num(QString);
    void do_insert();
    void do_del();
    void do_tree();
    void get_tree();

    void set_avl();
    void set_red_black();
    void set_treap();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
