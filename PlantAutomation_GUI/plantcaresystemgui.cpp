#include "plantcaresystemgui.h"
#include "ui_plantcaresystemgui.h"
#include <QTimer>
#include <QDateTime>
#include <QString>


int16_t ADS1115_ADDRESS = 0x48;                         // 1001 000 (ADDR = GND)

// Set the initial current time to the current date and time
time_t currentDateTime_t = time(0);                     // Get the current time in time_t     
QDateTime* currentDateTime = new QDateTime;             // Create a QDateTime object
QTimer* timer = new QTimer;                             // Create a timer object

// Set the initial light on time and off time
struct tm *tm_local = localtime(&currentDateTime_t);    // Get the current time in tm struct
time_t dailyOnTime = mktime(tm_local);                  // Convert the tm struct to time_t
time_t dailyOffTime = mktime(tm_local);                 // Convert the tm struct to time_t

int ONE_DAY = 24 * 60 * 60;                             // Number of seconds in a day
int SYSTEM_CLOCK = 1000;                                // System clock in milliseconds
int PUMP_WAIT_TIME = 3 * 3600;                          // Time to ignore the pump after activation
int PUMP_DURATION = 3;                                  // Duration to run the pump when activated
int MULTI_PUMP_WAIT_TIME = 5 * 60;                      // Time between seperate pump activations
int LIGHT_WAIT_TIME = 45;                               // Time to ignore the light after activation
int LIGHT_ON_DURATION = 5;                              // Duration to run the light when activated

int TOP_LIGHT_PIN = 27;                                 // GPIO pin for top shelf light
int TOP_PUMP_PIN = 17;                                  // GPIO pin for top shelf water pump
ADS1115::Mux TS_MUX_SELECT = ADS1115::Mux::AIN0_GND;    // Mux configuration for top shelf soil sensor
int BOTTOM_LIGHT_PIN = 24;                              // GPIO pin for bottom shelf light
int BOTTOM_PUMP_PIN = 23;                               // GPIO pin for bottom shelf water pump
ADS1115::Mux BS_MUX_SELECT = ADS1115::Mux::AIN1_GND;    // Mux configuration for bottom shelf soil sensor

// Initialize system controllers
SystemController topShelfControl(ADS1115_ADDRESS, 
                                TS_MUX_SELECT, 
                                TOP_LIGHT_PIN, 
                                dailyOnTime, 
                                dailyOffTime, 
                                TOP_PUMP_PIN, 
                                PUMP_WAIT_TIME, 
                                PUMP_DURATION);

SystemController bottomShelfControl(ADS1115_ADDRESS, 
                                BS_MUX_SELECT, 
                                BOTTOM_LIGHT_PIN, 
                                dailyOnTime + MULTI_PUMP_WAIT_TIME, 
                                dailyOffTime + MULTI_PUMP_WAIT_TIME, 
                                BOTTOM_PUMP_PIN, 
                                PUMP_WAIT_TIME, 
                                PUMP_DURATION);

// Initial calibration values for the soil sensors
int16_t TOP_CAL_DRY_DEFAULT = 0x559a;
int16_t TOP_CAL_WET_DEFAULT = 0x20a4;
int16_t BOTTOM_CAL_DRY_DEFAULT = 0x5785;
int16_t BOTTOM_CAL_WET_DEFAULT = 0x271a;

Logger logger;                                          // Create a logger object

/**
 * @brief PlantCareSystemGUI::PlantCareSystemGUI
 * @param parent
*/
PlantCareSystemGUI::PlantCareSystemGUI(QWidget *parent)
    : QMainWindow(parent)                               // Initialize the QMainWindow class
    , ui(new Ui::PlantCareSystemGUI)                    // Initialize the UI class
{
    // Set the initial soil sensor cal values
    topShelfControl.setSoilMoistureCalibrationValues(TOP_CAL_WET_DEFAULT, TOP_CAL_DRY_DEFAULT);
    bottomShelfControl.setSoilMoistureCalibrationValues(BOTTOM_CAL_WET_DEFAULT, BOTTOM_CAL_DRY_DEFAULT);

    // Connect the timer to the update function
    connect(timer, &QTimer::timeout, this, [this]() {

        // Update date and time
        *currentDateTime = QDateTime::currentDateTime();

        // Get the current time in seconds since epoch
        time_t working_time = currentDateTime->toSecsSinceEpoch();

        // Update the GUI values
        update_gui_values();

        // Update the system inputs
        update_system_inputs();

        // Control the system if manual checboxes are not checked
        
        // Top shelf automatic light control method call
        if (!ui->top_light_checkBox->isChecked())
        {
            topShelfControl.controlLight(working_time);
        }

        // Top shelf automatic water pump control method call
        if (!ui->top_pump_checkBox->isChecked() && ui->top_pump_enable_checkbox->isChecked())
        {
            topShelfControl.controlWaterPump(working_time);
        }

        // Bottom shelf automatic light control method call
        if (!ui->bottom_light_checkbox->isChecked())
        {
            bottomShelfControl.controlLight(working_time);
        }

        // Bottom shelf automatic water pump control method call
        if (!ui->bottom_pump_checkBox->isChecked() && ui->bott_pump_enable_checkbox->isChecked())
        {
            bottomShelfControl.controlWaterPump(working_time);
        }

    });

    // Start the timer
    timer->start(SYSTEM_CLOCK);

    // Initialize the UI
    ui->setupUi(this);
}

/**
 * @brief Destuctor for the GUI.
*/
PlantCareSystemGUI::~PlantCareSystemGUI()
{
    delete ui;
}

/**
 * @brief Move to the main screen.
 */
void PlantCareSystemGUI::on_mainView_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

    // Log the screen change
    ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Screen Change", "Main Screen")));
}


/**
 * @brief move to the manual screen.
 */
void PlantCareSystemGUI::on_manual_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    // Log the screen change
    ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Screen Change", "Manual Screen")));
}

/**
 * @brief Move to the log screen.
 */
void PlantCareSystemGUI::on_log_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

    // Log the screen change
    ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Screen Change", "Log Screen")));
}

/**
 * @brief Update the state of the top light.
 */
void PlantCareSystemGUI::on_top_light_checkBox_stateChanged(int arg1)
{
    // arg1 is '0' when not checked and '2' when checked
    if (arg1 == 0)
    {
        // Turn off top shelf light
        topShelfControl.setLightOn(false);

        // Log the manual light change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Manual Light Change", "Top Shelf Light Off")));
    }
    else if (arg1 == 2)
    {
        // Turn on top shelf light
        topShelfControl.setLightOn(true);

        // Log the manual light change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Manual Light Change", "Top Shelf Light On")));
    }
}

/**
 * @brief Update the state of the bottom light.
 */
void PlantCareSystemGUI::on_bottom_light_checkbox_stateChanged(int arg1)
{
    // arg1 is '0' when not checked and '2' when checked
    if (arg1 == 0)
    {
        // Turn off bottom shelf light
        bottomShelfControl.setLightOn(false);

        // Log the manual light change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Manual Light Change", "Bottom Shelf Light Off")));
    }
    else if (arg1 == 2)
    {
        // Turn on bottom shelf light
        bottomShelfControl.setLightOn(true);

        // Log the manual light change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Manual Light Change", "Bottom Shelf Light On")));
    }

}

/**
 * @brief Update the state of the top pump.
 */
void PlantCareSystemGUI::on_top_pump_checkBox_stateChanged(int arg1)
{
    // arg1 is '0' when not checked and '2' when checked
    if (arg1 == 0)
    {
        // Turn off top water pump
        topShelfControl.setWaterPumpOn(false);

        // Log the manual pump change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Manual Pump Change", "Top Shelf Pump Off")));
    }
    else if (arg1 == 2)
    {
        // Turn on top water pump
        topShelfControl.setWaterPumpOn(true);

        // Log the manual pump change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Manual Pump Change", "Top Shelf Pump On")));
    }
}

/**
 * @brief Update the state of the bottom pump.
 */
void PlantCareSystemGUI::on_bottom_pump_checkBox_stateChanged(int arg1)
{
    // arg1 is '0' when not checked and '2' when checked
    if (arg1 == 0)
    {
        // Turn off bottom water pump
        bottomShelfControl.setWaterPumpOn(false);

        // Log the manual pump change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Manual Pump Change", "Bottom Shelf Pump Off")));
    }
    else if (arg1 == 2)
    {
        // Turn on bottom water pump
        bottomShelfControl.setWaterPumpOn(true);

        // Log the manual pump change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Manual Pump Change", "Bottom Shelf Pump On")));
    }

}

/**
 * @brief Update the calibration of the top soil sensor.
 */
void PlantCareSystemGUI::on_cal_top_button_clicked()
{
    topShelfControl.calibrateSoilSensor();

    // Log the manual calibration
    ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Manual Calibration", "Top Shelf Soil Sensor")));
}

/**
 * @brief Update the calibration of the bottom soil sensor.
 */
void PlantCareSystemGUI::on_cab_bottom_button_clicked()
{
    bottomShelfControl.calibrateSoilSensor();

    // Log the manual calibration
    ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Manual Calibration", "Bottom Shelf Soil Sensor")));
}

/**
 * @brief Update the setpoint of the top shelf water pump.
 */
void PlantCareSystemGUI::on_top_setpoint_dial_actionTriggered(int action)
{
    ui->top_setpoint_lcd->display(ui->top_setpoint_dial->value());
}

/**
 * @brief Update the setpoint of the bottom shelf water pump.
 */
void PlantCareSystemGUI::on_bott_setpoint_dial_actionTriggered(int action)
{
    ui->bott_setpoint_lcd->display(ui->bott_setpoint_dial->value());
}

/**
 * @brief Update the values displayed on the GUI.
 */
void PlantCareSystemGUI::update_gui_values() {
    // Update moisture values
    ui->top_soilSensor_lcd->display(topShelfControl.readSoilMoisture());
    ui->bottom_soilSensor_lcd->display(bottomShelfControl.readSoilMoisture());

    // Update date and time
    ui->current_dateTime->setDateTime(*currentDateTime);

    // Update clock values if they are less than 1/1/2023
    if (ui->top_light_on_time->dateTime().toSecsSinceEpoch() < 1672531200)
    {
        // Set the initial light on time and off time of 5:00 AM and 9:00 PM
        struct tm *tm_local = localtime(&currentDateTime_t);    // Get the current time in tm struct
        tm_local->tm_hour = 5;                                  // Set the hour to 5
        tm_local->tm_min = 0;                                   // Set the minute to 0
        tm_local->tm_sec = 0;                                   // Set the second to 0
        dailyOnTime = mktime(tm_local);                  // Convert the tm struct to time_t

        // Set the initial light off time to 9:00 PM
        tm_local->tm_hour = 21;                                 // Set the hour to 21
        tm_local->tm_min = 0;                                   // Set the minute to 0
        tm_local->tm_sec = 0;                                   // Set the second to 0
        dailyOffTime = mktime(tm_local);                 // Convert the tm struct to time_t

        // cout dailyOnTime and dailyOffTime time_t values in correct format
        std::cout << "dailyOnTime: " << std::put_time(std::localtime(&dailyOnTime), "%c") << std::endl;
        std::cout << "dailyOffTime: " << std::put_time(std::localtime(&dailyOffTime), "%c") << std::endl;

        // Set the initial light on and off times
        ui->top_light_on_time->setDateTime(QDateTime::fromTime_t(dailyOnTime));
        ui->top_light_off_time->setDateTime(QDateTime::fromTime_t(dailyOffTime));
        ui->bott_light_on_time->setDateTime(QDateTime::fromTime_t(dailyOnTime));
        ui->bott_light_off_time->setDateTime(QDateTime::fromTime_t(dailyOffTime));
    }   

    // Update top shelf light time to the following day if it is less than the current time
    if (ui->top_light_off_time->dateTime().toSecsSinceEpoch() < currentDateTime->toSecsSinceEpoch())
    {
        ui->top_light_on_time->setDateTime(ui->top_light_on_time->dateTime().addSecs(ONE_DAY));
        ui->top_light_off_time->setDateTime(ui->top_light_off_time->dateTime().addSecs(ONE_DAY));
    }

    // Update bottom shelf light time to the following day if it is less than the current time
    if (ui->bott_light_off_time->dateTime().toSecsSinceEpoch() < currentDateTime->toSecsSinceEpoch())
    {
        ui->bott_light_on_time->setDateTime(ui->bott_light_on_time->dateTime().addSecs(ONE_DAY));
        ui->bott_light_off_time->setDateTime(ui->bott_light_off_time->dateTime().addSecs(ONE_DAY));
    }


    // Scroll to the bottom of the log
    // ui->log_listWidget->scrollToBottom();
}

/**
 * @brief Update the values utilized by the program.
 */
void PlantCareSystemGUI::update_system_inputs() {

    // Get the current time in seconds since epoch
    time_t working_time = currentDateTime->toSecsSinceEpoch();

    // Check if the top shelf on time has changed
    if (ui->top_light_on_time->dateTime().toSecsSinceEpoch() != topShelfControl.getLightOnTime())
    {
        // Update top shelf on time
        topShelfControl.setLightOnTime(ui->top_light_on_time->dateTime().toSecsSinceEpoch());

        // Log the top shelf on time change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Light On Time Change", "Top Shelf Light On Time")));
    }

    // Check if the top shelf off time has changed
    if (ui->top_light_off_time->dateTime().toSecsSinceEpoch() != topShelfControl.getLightOffTime())
    {
        topShelfControl.setLightOffTime(ui->top_light_off_time->dateTime().toSecsSinceEpoch());

        // Log the top shelf off time change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Light Off Time Change", "Top Shelf Light Off Time")));
    }

    // Check if the bottom shelf on time has changed
    if (ui->bott_light_on_time->dateTime().toSecsSinceEpoch() != bottomShelfControl.getLightOnTime())
    {
        bottomShelfControl.setLightOnTime(ui->bott_light_on_time->dateTime().toSecsSinceEpoch());

        // Log the bottom shelf on time change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Light On Time Change", "Bottom Shelf Light On Time")));
    }

    // Check if the bottom shelf off time has changed
    if (ui->bott_light_off_time->dateTime().toSecsSinceEpoch() != bottomShelfControl.getLightOffTime())
    {
        bottomShelfControl.setLightOffTime(ui->bott_light_off_time->dateTime().toSecsSinceEpoch());

        // Log the bottom shelf off time change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Light Off Time Change", "Bottom Shelf Light Off Time")));
    }

    // Check if the top shelf setpoint has changed
    if (ui->top_setpoint_dial->value() - 1.0 != topShelfControl.getSoilMoistureThreshold())
    {
        topShelfControl.setSoilMoistureThreshold(ui->top_setpoint_dial->value() - 1.0);

        // Log the top shelf setpoint change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Setpoint Change", "Top Shelf Setpoint")));
    }

    // Check if the bottom shelf setpoint has changed
    if (ui->bott_setpoint_dial->value() - 1.0 != bottomShelfControl.getSoilMoistureThreshold())
    {
        bottomShelfControl.setSoilMoistureThreshold(ui->bott_setpoint_dial->value() - 1.0);

        // Log the bottom shelf setpoint change
        ui->log_listWidget->addItem(QString::fromStdString(logger.logEvent("INFO", "Setpoint Change", "Bottom Shelf Setpoint")));
    }
}
