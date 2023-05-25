#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(640, 480); // Set the size of the window to 640x480 pixels.
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    int numRows = 4;
    int numCols = 120;

    // Calculate the width and height of each cell (pixel), and make them square.
    int cellSize = std::min(width() / numCols, height() / numRows);

    for(int i = 0; i < numCols; ++i) {
        for(int j = 0; j < numRows; ++j) {
            // Calculate a color based on the cell's position in the array.
            int red = (255 * i) / numCols;
            int green = (255 * j) / numRows;
            int blue = (255 * (i + j)) / (numCols + numRows);

            painter.fillRect(i * cellSize, j * cellSize, cellSize, cellSize, QColor(red, green, blue));

            // Draw white border
            painter.setPen(QPen(Qt::white));
            painter.drawRect(i * cellSize, j * cellSize, cellSize, cellSize);
        }
    }
}
