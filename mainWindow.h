#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <map>

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

    QString operatori = "/x+-";
    QPushButton* skaitluPogas[10] = {};
    QPushButton* simboluPogas[6] = {};

    bool isJoke = false;

    // Karte jokiem balstoties uz lietotāja instrukcijām.
    std::map<std::string, std::string> instrukcijuJoki = {
        {"1+1", "11"},
        {"6+7", "67"},
        {"9+10", "21"}
    };

    // Arī joku karte bet ar gala rezultātu tulkošanu uz joku.
    std::map<std::string, std::string> rezultatuJoki = {
        {"67", "SIX SEVENNNN"},
        {"69", "Nice"},
        {"420", "WEED"}
    };

private slots:
    void ciparuNospiesana();
    void simboluNospiesana();
    void equalsNospiesana();
};

#endif
