#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QSlider>

#define FRAME_RATE 120

float mapf(float input, float minx, float maxx, float outmin, float outmax) {
    // Clamp the input value within the specified range
    input = std::max(minx, std::min(maxx, input));

    // Calculate the normalized position of the input within the range
    float normalized = (input - minx) / (maxx - minx);

    // Map the normalized value to the output range
    float output = outmin + normalized * (outmax - outmin);

    return output;
}

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
    xLineEdit = new QLineEdit(this);
    xLineEdit->move(50, 415); // adjust the position as needed
    xLineEdit->setText("50");

    // Create QLabel and QLineEdit for Y
    QLabel *yLabel = new QLabel(this);
    yLabel->setText("Y:");
    yLabel->move(20, 450); // adjust the position as needed
    yLineEdit = new QLineEdit(this);
    yLineEdit->move(50, 450); // adjust the position as needed
    yLineEdit->setText("0");
    yLineEdit->setReadOnly(true);

    // Create a slider for the Courant number
    QSlider *slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, 1000);
    slider->setValue(50); // Assuming the initial Courant number is 50%
    slider->setGeometry(300, 420, 100, 20);  // adjust the position and size as needed

    // Create a label for the Courant number
    QLabel *sliderLabel = new QLabel(this);
    sliderLabel->setText("Courant Val:");
    sliderLabel->move(400, 420); // adjust the position as needed

    // Create a slider for the dampening factor
    QSlider *dampeningSlider = new QSlider(Qt::Horizontal, this);
    dampeningSlider->setRange(0, 1000); // Dampening factor can range from 0 to 100
    dampeningSlider->setValue(50); // Set the initial dampening factor to 50
    dampeningSlider->setGeometry(300, 450, 100, 20); // adjust the position and size as needed

    // Create a label for the dampening factor
    QLabel *dampeningLabel = new QLabel(this);
    dampeningLabel->setText("Dampening: ");
    dampeningLabel->move(400, 450); // adjust the position as needed

    // Connect the slider valueChanged signal to a slot to handle changing the Courant number
    connect(slider, &QSlider::valueChanged, [this, slider, sliderLabel]() {
        float courantNum = slider->value() / 1000.0f;
        // TODO: Use courantNum here.
        courantNum = mapf(courantNum, 0.0f, 1.0f, 0.0f, 2.0f);
        grid.setSpeed(courantNum);
        //printf("setting courant to %f\n", courantNum);
        sliderLabel->setText("Val: " + QString::number(courantNum, 'f', 2));
    });

    // Connect the slider valueChanged signal to a slot to handle changing the dampening factor
    connect(dampeningSlider, &QSlider::valueChanged, [this, dampeningSlider, dampeningLabel]() {
        float dampeningFactor = dampeningSlider->value() / 1000.0f;  // Convert slider value to dampening factor in range 0.0 to 1.0
        // TODO: Use dampeningFactor here. Depending on your application, you may need to store this value or use it immediately.
        dampeningFactor = mapf(dampeningFactor, 0.0f, 1.0f, 0.0f, 16.0f);
        //printf("setting dampening factor to %f\n", dampeningFactor);
        dampeningLabel->setText("Dampening: " + QString::number(dampeningFactor, 'f', 2));
        grid.setDampenening(dampeningFactor);
    });

    // Set initial value for dampeningLabel
    float initialDampeningFactor = dampeningSlider->value() / 100.0f;
    dampeningLabel->setText("Dampening: " + QString::number(initialDampeningFactor, 'f', 2));

    // Connect the clicked() signal from the button to the MainWindow's perturb() slot.
    connect(button, &QPushButton::clicked, this, &MainWindow::perturb);

    // Initialize the timer.
    timer = new QTimer(this);

    // Connect the timeout() signal from the timer to the MainWindow's update() slot.
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    // Start the timer to emit timeout() signal every 1000/FRAME_RATE milliseconds.
    timer->start(int(1000.0f/FRAME_RATE));
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
            // Get the grid value, clamp it to [-1, 1] and map it to [0, 255].
            float gridValue = grid.getGridValue(i, 0);  // 0 for y since we are interested in the x,0 value
            gridValue = std::min(std::max(gridValue, -1.0f), 1.0f);
            gridValue = ((gridValue + 1.0f) / 2.0f) * 255.0f;
            // Map the grid value to a brightness value (0-255).
            int value = std::max<int>(0, std::min<int>(255, gridValue));
            // Always use white hue (0) and saturation (0). The brightness is determined by the grid value.
            painter.fillRect(i * cellSize, j * cellSize, cellSize, cellSize, QColor::fromHsv(0, 0, value));
            // Draw white border
            painter.setPen(QPen(Qt::white));
            painter.drawRect(i * cellSize, j * cellSize, cellSize, cellSize);
        }
    }
}

void MainWindow::update()
{
    // TODO: Insert your update logic here.
    //printf("update running\n");
    grid.update();

    // Redraw the window.
    QWidget::update();
}

void MainWindow::perturb()
{
    int x = xLineEdit->text().toInt();
    int y = yLineEdit->text().toInt();

    grid.perturb(x, y, 1.0f);
    this->update();
}
