#include "mainWindow.h"
#include "ui_kalkulators.h"
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Nerādam lietotājam "Lauks" tekstu, tas maisīsies ar tālāk ievadītajiem cipariem.
    ui->field->setText("");

    this->skaitluPogas[0] = ui->seven;
    this->skaitluPogas[1] = ui->eight;
    this->skaitluPogas[2] = ui->nine;
    this->skaitluPogas[3] = ui->four;
    this->skaitluPogas[4] = ui->five;
    this->skaitluPogas[5] = ui->six;
    this->skaitluPogas[6] = ui->one;
    this->skaitluPogas[7] = ui->two;
    this->skaitluPogas[8] = ui->three;
    this->skaitluPogas[9] = ui->zero;

    for (QPushButton* poga : this->skaitluPogas) {
        if (!poga) continue;

        connect(poga, &QPushButton::clicked, this, &MainWindow::ciparuNospiesana);
    }

    this->simboluPogas[0] = ui->division;
    this->simboluPogas[1] = ui->multiply;
    this->simboluPogas[2] = ui->plus;
    this->simboluPogas[3] = ui->minus;
    this->simboluPogas[4] = ui->comma;

    for (QPushButton* poga : this->simboluPogas) {
        if (!poga) continue;

        connect(poga, &QPushButton::clicked, this, &MainWindow::simboluNospiesana);
    }
}

void MainWindow::ciparuNospiesana() {
    QPushButton* poga = qobject_cast<QPushButton*>(sender());

    ui->field->setText(ui->field->text() + poga->text());
}

void MainWindow::simboluNospiesana() {
    QPushButton* poga = qobject_cast<QPushButton*>(sender());
    QString pogasTeksts = poga->text();
    QString teksts = ui->field->text();
    QString papildinajums;  // Šī vērtība būs pievienota teksta laukam.

    if (pogasTeksts == "-") {
        if (
            teksts.isEmpty() ||
            teksts.back() == "÷" ||
            teksts.back() == "x" ||
            teksts.back() == "+"
        ) {
            papildinajums = pogasTeksts;
        }
    }
    else if (pogasTeksts == ",") {
        // Operatori atdala numurus kuriem var būt komats.
        int pedejaisOp = teksts.lastIndexOf(QRegularExpression("[÷x+-]"));
        QString pedejaisSkaitlis = teksts.mid(pedejaisOp + 1);

        if (!pedejaisSkaitlis.contains(",")) {
            papildinajums = ",";
        }
    }
    else {
        // Neatļaujam lietotājam ievadīt simbolu ja lauka beigās nav ievadīts kaut viens cipars!
        if (!teksts.isEmpty() && teksts.back().isDigit()) {
            papildinajums = pogasTeksts;
        }
    }

    ui->field->setText(teksts + papildinajums);
}

MainWindow::~MainWindow() {
    delete ui;
}
