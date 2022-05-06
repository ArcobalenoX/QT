#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLabel* label1;
    QLabel* label2;
    QLabel* label3;
    QVBoxLayout* vboxlayout;
    QHBoxLayout* hboxlayout;
    QWidget* wid;

};
#endif // MAINWINDOW_H
