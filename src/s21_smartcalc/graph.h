#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
extern "C" {
#include "../s21_smartcalc.h"
}

namespace Ui {
class graph;
}

class graph : public QWidget {
  Q_OBJECT

 public:
  explicit graph(QWidget *parent = nullptr);
  ~graph();
  void getData(QString res);

 public slots:
  void on_pushButton_clicked();

 private:
  Ui::graph *ui;
};

#endif  // GRAPH_H
