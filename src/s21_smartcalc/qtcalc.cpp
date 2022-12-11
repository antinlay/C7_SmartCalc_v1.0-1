#include "qtcalc.h"

#include "./ui_qtcalc.h"
#include "credit.h"
#include "debit.h"
#include "graph.h"
#include "qobjectdefs.h"

int ox = 0, dot = 0, symb = 0, open = 0;

qtCalc::qtCalc(QWidget *parent) : QMainWindow(parent), ui(new Ui::qtCalc) {
  QLocale lo(QLocale::C);
  lo.setNumberOptions(QLocale::RejectGroupSeparator);
  auto val = new QDoubleValidator();
  val->setLocale(lo);

  ui->setupUi(this);

  ui->resultShow->setValidator(val);
  ui->xRes->setValidator(val);

  graphWindow = new graph;

  connect(ui->sin, SIGNAL(clicked()), this, SLOT(mathFuncs()));
  connect(ui->cos, SIGNAL(clicked()), this, SLOT(mathFuncs()));
  connect(ui->tan, SIGNAL(clicked()), this, SLOT(mathFuncs()));
  connect(ui->asin, SIGNAL(clicked()), this, SLOT(mathFuncs()));
  connect(ui->acos, SIGNAL(clicked()), this, SLOT(mathFuncs()));
  connect(ui->atan, SIGNAL(clicked()), this, SLOT(mathFuncs()));
  connect(ui->sqrt, SIGNAL(clicked()), this, SLOT(mathFuncs()));
  connect(ui->ln, SIGNAL(clicked()), this, SLOT(mathFuncs()));
  connect(ui->log, SIGNAL(clicked()), this, SLOT(mathFuncs()));

  connect(ui->num0, SIGNAL(clicked()), this, SLOT(numFuncs()));
  connect(ui->num1, SIGNAL(clicked()), this, SLOT(numFuncs()));
  connect(ui->num2, SIGNAL(clicked()), this, SLOT(numFuncs()));
  connect(ui->num3, SIGNAL(clicked()), this, SLOT(numFuncs()));
  connect(ui->num4, SIGNAL(clicked()), this, SLOT(numFuncs()));
  connect(ui->num5, SIGNAL(clicked()), this, SLOT(numFuncs()));
  connect(ui->num6, SIGNAL(clicked()), this, SLOT(numFuncs()));
  connect(ui->num7, SIGNAL(clicked()), this, SLOT(numFuncs()));
  connect(ui->num8, SIGNAL(clicked()), this, SLOT(numFuncs()));
  connect(ui->num9, SIGNAL(clicked()), this, SLOT(numFuncs()));

  connect(ui->div, SIGNAL(clicked()), this, SLOT(symbs()));
  connect(ui->mult, SIGNAL(clicked()), this, SLOT(symbs()));
  connect(ui->add, SIGNAL(clicked()), this, SLOT(symbs()));
  connect(ui->sub, SIGNAL(clicked()), this, SLOT(symbs()));
  connect(ui->pow, SIGNAL(clicked()), this, SLOT(symbs()));
  connect(ui->mod, SIGNAL(clicked()), this, SLOT(symbs()));
  //  connect(ui->open, SIGNAL(clicked()), this, SLOT(symbs()));
  //  connect(ui->close, SIGNAL(clicked()), this, SLOT(symbs()));
  //  connect(ui->xoy, SIGNAL(clicked()), this, SLOT(symbs()));
  connect(ui->equal, SIGNAL(clicked()), this, SLOT(equalClick()));
  connect(ui->dot, SIGNAL(clicked()), this, SLOT(dotClick()));

  connect(ui->CE, SIGNAL(clicked()), this, SLOT(ceClick()));

  //  connect(ui->graph, SIGNAL(clicked()), this, SLOT(on_graph_clicked()));
  connect(this, &qtCalc::sendData, graphWindow, &graph::getData);
}

qtCalc::~qtCalc() { delete ui; }

void qtCalc::initCalc() {
  if (ui->resultShow->text() == "0 ") {
    ui->resultShow->clear();
    dot = 0;
    ox = 0;
    symb = 0;
    open = 0;
  }
}

void qtCalc::mathFuncs() {
  initCalc();
  QPushButton *button = (QPushButton *)sender();
  QString allStrings;
  allStrings = (ui->resultShow->text() + button->text());
  ui->resultShow->setText(allStrings + '(');
  open++;
  symb++;
  if (dot) dot--;
  if (ox) ox--;
}

void qtCalc::numFuncs() {
  initCalc();
  QPushButton *button = (QPushButton *)sender();
  QString newString;
  newString = (ui->resultShow->text() + button->text());
  ui->resultShow->setText(newString);
  if (symb) symb--;
  if (ox) ox--;
}

void qtCalc::symbs() {
  initCalc();
  QPushButton *button = (QPushButton *)sender();
  QString allStrings;
  if (!symb) {
    allStrings = (ui->resultShow->text()) + button->text();
    ui->resultShow->setText(allStrings);
    symb++;
    if (dot) dot--;
    if (ox) ox--;
  }
}

void qtCalc::dotClick() {
  initCalc();
  if (!dot) {
    char point = '.';
#ifdef linux
    point = ',';
#endif
    ui->resultShow->setText(ui->resultShow->text() + point);
    dot++;
    symb++;
  }
}

void qtCalc::equalClick() {
  initCalc();
  while (open) {
    on_close_clicked();
  }
  QString calc = ui->resultShow->text(), num;
  QByteArray byte = calc.toLocal8Bit();
  double result = 0;
  char *str = byte.data();

  if (calc.left(1) == "+") calc.replace("+", "");

  if (calc.contains("X", Qt::CaseInsensitive)) {
    num = ui->xRes->text();
    calc = calc.replace("X", num);
    str = calc.toLocal8Bit().data();
    if (validAriphSymb(str)) {
      result = calculate(str);
      QString resCalc = QString::number(result, 'g', 14);
#ifdef linux
      resCalc.replace('.', ',');
#endif
      ui->resultShow->setText(resCalc);
    } else {
      ui->resultShow->setText("0 ");
    }
  } else if (validAriphSymb(str)) {
    result = calculate(str);
    QString resCalc = QString::number(result, 'g', 14);
#ifdef linux
    resCalc.replace('.', ',');
#endif
    ui->resultShow->setText(resCalc);
  } else {
    ui->resultShow->setText("0 ");
  }
  dot = 0;
  symb = 0;
  ox = 0;
}

void qtCalc::ceClick() {
  if (ui->resultShow->text() != "") {
    ui->resultShow->clear();
  }
  dot = 0;
  symb = 0;
  ox = 0;
}

void qtCalc::on_credit_clicked() {
  credit = new Credit();
  credit->show();
}

void qtCalc::on_debit_clicked() {
  debitWindow = new debit();
  debitWindow->show();
}

void qtCalc::on_open_clicked() {
  initCalc();
  if (!dot) {
    ui->resultShow->setText(ui->resultShow->text() + "(");
    open++;
    symb++;
    if (ox) ox--;
  }
}

void qtCalc::on_graph_clicked() {
  initCalc();
  if (ui->resultShow->text().contains("X", Qt::CaseInsensitive)) {
    emit sendData(ui->resultShow->text());
    graphWindow->show();
    graphWindow->on_pushButton_clicked();
  } else {
    ui->resultShow->setText("0 ");
  }
}

void qtCalc::on_xoy_clicked() {
  initCalc();
  if (!ox) {
    ui->resultShow->setText(ui->resultShow->text() + "X");
    ox++;
    if (symb) symb--;
  }
  emit sendData(ui->resultShow->text());
}

void qtCalc::on_close_clicked() {
  initCalc();
  if (!dot && open) {
    ui->resultShow->setText(ui->resultShow->text() + ")");
    open--;
    if (symb) symb--;
  }
}
