#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(640, 480); // Set the size of the window to 640x480 pixels.

    // Create a QPushButton and set its properties.
    QPushButton *button = new QPushButton(this);
    button->setText("Perturb");
    button->move(200, 420);  // adjust the position as needed

    // Create QLabel and QLineEdit for X
    QLabel *xLabel = new QLabel(this);
    xLabel->setText("X:");
    xLabel->move(20, 415); // adjust the position as needed
    QLineEdit *xLineEdit = new QLineEdit(this);
    xLineEdit->move(50, 415); // adjust the position as needed

    // Create QLabel and QLineEdit for Y
    QLabel *yLabel = new QLabel(this);
    yLabel->setText("Y:");
    yLabel->move(20, 450); // adjust the position as needed
    QLineEdit *yLineEdit = new QLineEdit(this);
    yLineEdit->move(50, 450); // adjust the position as needed

    // Initialize the timer.
    timer = new QTimer(this);

    // Connect the timeout() signal from the timer to the MainWindow's update() slot.
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    // Start the timer to emit timeout() signal every 1000 milliseconds (1 second).
    // You can adjust this interval to whatever suits your needs.
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete timer;
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
            // Get the grid value, clamp it to [-1, 1] and map it to [0, 1].
            float gridValue = grid.getGridValue(i, 1);  // 1 for y since we are interested in the x,1 value
            gridValue = std::min(std::max(gridValue, -1.0f), 1.0f);
            gridValue = (gridValue + 1.0f) / 2.0f;

            // Map the [0, 1] grid value to a hue value (0-359).
            int hue = static_cast<int>(gridValue * 359);

            painter.fillRect(i * cellSize, j * cellSize, cellSize, cellSize, QColor::fromHsv(hue, 255, 255));

            // Draw white border
            painter.setPen(QPen(Qt::white));
            painter.drawRect(i * cellSize, j * cellSize, cellSize, cellSize);
        }
    }
}
void MainWindow::update()
{
    // TODO: Insert your update logic here.
    printf("Hello world\n");
    grid.update();

    // Redraw the window.
    QWidget::update();
}
