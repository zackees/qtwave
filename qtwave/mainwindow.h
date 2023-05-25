#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "grid.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void update(); // This function will be called regularly by the QTimer.

private:
    Ui::MainWindow *ui;
    Grid<120> grid;
    QTimer *timer; // Timer to trigger updates.
};
#endif // MAINWINDOW_H
