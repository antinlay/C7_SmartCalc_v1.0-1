#include "debit.h"

#include "ui_debit.h"

debit::debit(QWidget *parent) : QWidget(parent), ui(new Ui::debit) {
  // for dot in validator
  QLocale lo(QLocale::C);
  lo.setNumberOptions(QLocale::RejectGroupSeparator);
  auto val = new QDoubleValidator();
  val->setLocale(lo);

  ui->setupUi(this);
  ui->lineEdit_sumDep->setValidator(val);
  ui->lineEdit_intRate->setValidator(val);
}

debit::~debit() { delete ui; }

void debit::on_pushButton_debit_clicked() {
  double sumDep = ui->lineEdit_sumDep->text().toDouble();
  double percent = ui->lineEdit_intRate->text().toDouble();
  int month = ui->spinBox_months->text().toInt();

  double resPercent = month / 12, resDep = 0, resProfit = 0, profit = 0;

  ui->lineEdit_resDep->clear();
  ui->lineEdit_resPercent->clear();
  ui->listWidget->clear();

  resProfit = (sumDep * percent * resPercent) / 100;
  resDep = sumDep + resProfit;
  for (int var = 1; var <= month; ++var) {
    QString depInfo, Qdep, Qpr;
    QString varMonth = QString::number(var);
    resPercent = varMonth.toDouble() / 12;

    if (ui->checkBox->checkState() == false) {
      // https://www.sravni.ru/vklady/info/kak-rasschitat-procenty-po-vklady/
      profit = (sumDep * percent * resPercent) / 100;

    } else {
      profit = (sumDep * percent * resPercent) / 100;
      sumDep = sumDep + profit;
      //            sumDep = dep;
    }
    double dep = sumDep + profit;
    Qdep = QString::number(dep, 'f', 2);
    Qpr = QString::number(profit, 'f', 2);
    depInfo = "Deposit for " + varMonth + " month = " + Qdep +
              " and profit = " + Qpr + "\n";
    ui->listWidget->addItem(depInfo);
  }
  ui->lineEdit_resPercent->setText(QString::number(resProfit, 'f', 2));
  ui->lineEdit_resDep->setText(QString::number(resDep, 'f', 2));
}
