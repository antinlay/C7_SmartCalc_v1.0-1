#ifndef QTCALC_H
#define QTCALC_H

#include <QDoubleValidator>
#include <QMainWindow>

#include "credit.h"
#include "debit.h"
#include "graph.h"

extern "C" {
#include "../s21_smartcalc.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class qtCalc;
}
QT_END_NAMESPACE

class qtCalc : public QMainWindow {
  Q_OBJECT

 public:
  qtCalc(QWidget *parent = nullptr);
  ~qtCalc();
  void initCalc();

 signals:
  void sendData(QString res);

 private:
  Ui::qtCalc *ui;
  Credit *credit;
  debit *debitWindow;
  graph *graphWindow;

 private slots:
  void mathFuncs();
  void numFuncs();
  void symbs();
  void dotClick();
  void equalClick();
  void ceClick();
  void on_credit_clicked();
  void on_debit_clicked();
  void on_open_clicked();
  void on_graph_clicked();
  void on_xoy_clicked();
  void on_close_clicked();
};
#endif  // QTCALC_H
