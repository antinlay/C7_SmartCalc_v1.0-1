#include "credit.h"

#include "math.h"
#include "ui_credit.h"

Credit::Credit(QWidget *parent) : QWidget(parent), ui(new Ui::Credit) {
  QLocale lo(QLocale::C);
  lo.setNumberOptions(QLocale::RejectGroupSeparator);
  auto val = new QDoubleValidator();
  val->setLocale(lo);

  ui->setupUi(this);
  ui->lineEdit_sumCredit->setValidator(val);
  ui->lineEdit_stavkaProc->setValidator(val);
  ui->comboBox->addItem("annuity");
  ui->comboBox->addItem("differentiated");
}

Credit::~Credit() { delete ui; }

void Credit::on_pushButtonCredit_clicked() {
  int month = ui->spinBox->text().toInt();
  double payment = 0, allSum = 0, pp = 0;
  double summa = ui->lineEdit_sumCredit->text().toDouble();

  ui->lineEdit_sumVyplat->clear();
  ui->lineEdit_sumPereplata->clear();
  ui->listWidget->clear();

  if (ui->lineEdit_stavkaProc->text() == "" ||
      ui->lineEdit_sumCredit->text() == "" || ui->spinBox->text() == "") {
    ui->listWidget->addItem("ERROR");
  } else {
    double summaProc = summa;

    for (int var = 1; var <= month; var++) {
      double stavka = ui->lineEdit_stavkaProc->text().toDouble();
      QString payInfo, monthPay;
      if (ui->comboBox->currentIndex() == 1) {
        // https://www.banki.ru/wikibank/raschet_differentsirovannogo_plateja/
        // in 31 days on month and 365 days on year
        QString varMonth = QString::number(var);
        payment = summa / month + summaProc * stavka * 31 / 365 / 100;
        allSum += payment;
        summaProc = summa - var * summa / month;
        monthPay = QString::number(payment, 'f', 2);
        payInfo = "Pay for " + varMonth + " month = " + monthPay + "\n";
      } else if (ui->comboBox->currentIndex() == 0) {
        // https://www.banki.ru/wikibank/raschet_annuitetnogo_plateja/
        var = month;
        double prStavka = stavka / 12 / 100;
        payment = summa * (prStavka * pow(1 + prStavka, month) /
                           (pow(1 + prStavka, month) - 1));
        allSum = payment * month;
        monthPay = QString::number(payment, 'f', 2);
        payInfo = "Pay for every month = " + monthPay + "\n";
        //                ui->listWidget->setMaximumSize(6, 6);
      }

      ui->listWidget->addItem(payInfo);
    }
    pp = allSum - summa;
    QString Qpp = QString::number(pp, 'f', 2);
    QString Qall = QString::number(allSum, 'f', 2);

    ui->lineEdit_sumVyplat->setText(Qall);
    ui->lineEdit_sumPereplata->setText(Qpp);
  }
}
