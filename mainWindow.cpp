#include "mainWindow.h"
#include "ui_kalkulators.h"
#include <iomanip>
#include <queue>
#include <stack>
#include <sstream>
#include <vector>
#include <QRegularExpression>

class Tokens {
    public:
        enum class Tips {
            KreisaisP,
            LabaisP,
            Numurs,
            Operators
        };

        Tips tips;
        int prioritate;
        std::string vertiba;
};

std::deque<Tokens> dabutTokenus(std::string izteiksme) {
    std::deque<Tokens> kaudze;
    QString teksts = QString::fromStdString(izteiksme).replace(",", ".");

    // Regulārā izteiksme.
    QRegularExpression visiTokeni("(?<![0-9])-?\\d*\\.?\\d+|[/x+÷-]");
    QRegularExpressionMatchIterator iterators = visiTokeni.globalMatch(teksts);

    while (iterators.hasNext()) {
        QRegularExpressionMatch match = iterators.next();
        QString vertiba = match.captured(0);

        Tokens tokens;
        tokens.vertiba = vertiba.toStdString();

        if (vertiba[0].isDigit() || (vertiba.size() > 1 && vertiba[1].isDigit())) {
            tokens.tips = Tokens::Tips::Numurs;
            tokens.prioritate = -1;
        } else {
            tokens.tips = Tokens::Tips::Operators;
            if (vertiba == "/" || vertiba == "x") tokens.prioritate = 3;
            else tokens.prioritate = 2;  // + un -
        }

        kaudze.push_back(tokens);
    }

    return kaudze;
}

std::string salabotNumuru(std::string x) {
    std::ostringstream tekstsIzeja;
    std::string teksts;

    double numurs = std::stod(x);
    tekstsIzeja << std::setprecision(10) << numurs;
    teksts = tekstsIzeja.str();

    size_t beigas = teksts.find_last_not_of('.') + 1;
    teksts.erase(beigas);

    return teksts;
}

std::queue<Tokens> algoritms(std::deque<Tokens> kaudze) {
    // Shunting-Yard algoritms paņemts no šejienes: https://github.com/ozgekaracam/Shunting-Yard
    // ar nelielām izmaiņām kas atbilst šīm projektam.
    Tokens tokens;
    std::queue<Tokens> izvade;
    std::stack<Tokens> operatoruKaudze;

    while (!kaudze.empty()) {
        tokens = kaudze.front();
        kaudze.pop_front();

        if (tokens.tips == Tokens::Tips::Numurs) {
            tokens.vertiba = salabotNumuru(tokens.vertiba);
            izvade.push(tokens);
        }
        else if (tokens.tips == Tokens::Tips::Operators) {
            Tokens o1 = tokens;

            while (!operatoruKaudze.empty()) {
                Tokens o2 = operatoruKaudze.top();

                if (o2.tips != Tokens::Tips::KreisaisP && (o1.prioritate <= o2.prioritate)) {
                    operatoruKaudze.pop();
                    izvade.push(o2);
                }
                else break;
            }

            operatoruKaudze.push(o1);
        }
        else if (tokens.tips == Tokens::Tips::KreisaisP) {
            operatoruKaudze.push(tokens);
        }
        else if (tokens.tips == Tokens::Tips::LabaisP) {
            while (
                !operatoruKaudze.empty() &&
                operatoruKaudze.top().tips !=
                Tokens::Tips::KreisaisP
            ) {
                izvade.push(operatoruKaudze.top());
                operatoruKaudze.pop();
            }

            if (
                !operatoruKaudze.empty() &&
                operatoruKaudze.top().tips == Tokens::Tips::KreisaisP
            ) {
                operatoruKaudze.pop();
            }
        }
        else break;
    }

    while (!operatoruKaudze.empty()) {
        if (operatoruKaudze.top().tips != Tokens::Tips::KreisaisP) {
            izvade.push(operatoruKaudze.top());
            operatoruKaudze.pop();
        }
        else break;
    }

    return izvade;
}

double aprekinat(std::queue<Tokens> izvade) {
    std::vector<double> operandi;

    while (!izvade.empty()) {
        Tokens tokens = izvade.front();
        izvade.pop();

        if (tokens.tips == Tokens::Tips::Numurs) {
            operandi.push_back(std::stod(tokens.vertiba));
        }

        if (tokens.tips == Tokens::Tips::Operators) {
            double operandsLabais = operandi.back();
            operandi.pop_back();

            double operandsKreisais = operandi.back();
            operandi.pop_back();

            if (tokens.vertiba == "x") {
                operandi.push_back(operandsKreisais * operandsLabais);
            }

            if (tokens.vertiba == "/") {
                operandi.push_back(operandsKreisais / operandsLabais);
            }

            if (tokens.vertiba == "+") {
                operandi.push_back(operandsKreisais + operandsLabais);
            }

            if (tokens.vertiba == "-") {
                operandi.push_back(operandsKreisais - operandsLabais);
            }
        }
    }

    return operandi[0];
}

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

    connect(ui->equals, &QPushButton::clicked, this, &MainWindow::equalsNospiesana);
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
        if (teksts == "-") return;

        bool pedejaisCipars = !teksts.isEmpty() && teksts.back().isDigit();
        bool pedejaisOp = (
            teksts.endsWith('/') ||
            teksts.endsWith('x') ||
            teksts.endsWith('+') ||
            teksts.endsWith('-')
        );

        bool pirmsPedejaisOp = false;

        if (teksts.length() >= 2) {
            QChar ieprieksejais = teksts[teksts.size() - 2];
            pirmsPedejaisOp = (
                ieprieksejais == '/' ||
                ieprieksejais == 'x' ||
                ieprieksejais == '+' ||
                ieprieksejais == '-'
            );
        }

        if (teksts.isEmpty() || pedejaisCipars || (pedejaisOp && !pirmsPedejaisOp)) {
            papildinajums = pogasTeksts;
        } else return;
    }
    else if (pogasTeksts == ",") {
        // Operatori atdala numurus kuriem var būt komats.
        int pedejaisOp = teksts.lastIndexOf(QRegularExpression("[/x+-]"));
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

void MainWindow::equalsNospiesana() {}

MainWindow::~MainWindow() {
    delete ui;
}
