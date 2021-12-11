#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QRegularExpression>
#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <iostream>
#include <vector>
#include <string>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<QString> stack;
    std::vector<QString> steps;
    QString input;
    QString converted;
    QString GetSteps() const;
    QString GetStack() const;
};
#endif // MAINWINDOW_H
