#include <QApplication>
#include <QDoubleValidator>

#include "qtcalc.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  qtCalc w;
  w.show();
  return a.exec();
}
