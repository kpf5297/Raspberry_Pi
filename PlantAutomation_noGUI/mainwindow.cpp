#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDateTime>

#include <pigpio.h>
#include <ADS1115.h>

int8_t address = 0x48;

ADS1115 AD(address);
int16_t analog_counts;
int16_t max = 0x66F7; // 3.3V
int16_t min = 0x0053;   // 0V
double voltage = 1.2;
double percent = 38.2;

// GPIO SETUP ******************************************************************


const int BLUE_LED_PIN = 17;
const int GREEN_LED_PIN = 27;

const int OUTPUT = 1;
const int INPUT = 0;

// END GPIO SETUP **************************************************************

QTime currentTime = QTime::currentTime();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGUI);
    timer->start(1000);

    // Enable the pigpio library
    if (gpioInitialise() < 0) {
        ui->listWidget->addItem("Error: pigpio initialisation failed");
    }


    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    // Disable the pigpio library
    gpioTerminate();

    delete ui;
}

void MainWindow::blue_LED_selection_changed() {

    // If the checkbox is checked, turn on the LED
    if (ui->blue_led_checkBox->isChecked()) {
        gpioWrite(BLUE_LED_PIN, 1);
        ui->listWidget->addItem("Blue LED turned on.");
    } else {
        gpioWrite(BLUE_LED_PIN, 0);
        ui->listWidget->addItem("Blue LED turned off.");
    }
}

void MainWindow::green_LED_selection_changed() {

    // If the checkbox is checked, turn on the LED
    if (ui->green_led_checkBox->isChecked()) {
        gpioWrite(GREEN_LED_PIN, 1);
        ui->listWidget->addItem("Green LED turned on.");
    } else {
        gpioWrite(GREEN_LED_PIN, 0);
        ui->listWidget->addItem("Green LED turned off.");
    }

}

void MainWindow::updateGUI() {

    // map the voltage to a percentage
    percent = (AD.read0() - min) * 100 / (max - min);

    // convert the int16_t to a voltage double
    voltage = AD.read0() * 3.3 / 26359;

    ui->voltage_lcdNumber->display(voltage);

    ui->voltage_progressBar->setValue(percent);

}

void MainWindow::logEvent(const QString& eventName, const QString& details) {
    QFile file("log.csv");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);

        // Get the current date and time
        QDateTime currentDateTime = QDateTime::currentDateTime();

        // Format the log entry as a comma-separated value
        QString logEntry = QString("%1,%2,%3\n")
                               .arg(currentDateTime.toString("yyyy-MM-dd hh:mm:ss"))
                               .arg(eventName)
                               .arg(details);

        // Write the log entry to the file
        stream << logEntry;

        // Close the file
        file.close();
    } else {
        // Handle the case where the file could not be opened
        std::cout << "Failed to open log file for writing";
    }
}