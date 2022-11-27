#ifndef DEBIT_H
#define DEBIT_H

#include <QWidget>

namespace Ui {
class debit;
}

class debit : public QWidget {
  Q_OBJECT

 public:
  explicit debit(QWidget *parent = nullptr);
  ~debit();

 private slots:
  void on_pushButton_debit_clicked();

 private:
  Ui::debit *ui;
};

#endif  // DEBIT_H
