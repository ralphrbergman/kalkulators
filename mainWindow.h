#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QString operatori = "÷x+-,";
    QPushButton* skaitluPogas[10] = {};
    QPushButton* simboluPogas[5] = {};

private slots:
    void ciparuNospiesana();
    void simboluNospiesana();
};

#endif
