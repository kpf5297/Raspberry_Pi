#ifndef PLANTCARESYSTEMGUI_H
#define PLANTCARESYSTEMGUI_H

#include <QMainWindow>
#include <ADS1115.h>
#include <LightController.h>
#include <Logging.h>
#include <SoilSensor.h>
#include <WaterPump.h>
#include <SystemController.h>
#include <gpiod.h>
#include <chrono>
#include <ctime>
#include <iomanip>

QT_BEGIN_NAMESPACE
namespace Ui { class PlantCareSystemGUI; }
QT_END_NAMESPACE

/**
 * @brief The PlantCareSystemGUI class
 */
class PlantCareSystemGUI : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief Constructor for PlantCareSystemGUI.
     * @param parent Parent widget.
     */
    PlantCareSystemGUI(QWidget *parent = nullptr);

    /**
     * @brief Destructor for PlantCareSystemGUI.
     */
    ~PlantCareSystemGUI();

private slots:

    /**
     * @brief Move to the main screen.
     */
    void on_mainView_pushButton_clicked();

    /**
     * @brief move to the manual screen.
     */
    void on_manual_pushButton_clicked();

    /**
     * @brief Move to the log screen.
     */
    void on_log_pushButton_clicked();

    /**
     * @brief Update the state of the top light.
     */
    void on_top_light_checkBox_stateChanged(int arg1);

    /**
     * @brief Update the state of the bottom light.
     */
    void on_bottom_light_checkbox_stateChanged(int arg1);

    /**
     * @brief Update the state of the top pump.
     */
    void on_top_pump_checkBox_stateChanged(int arg1);

    /**
     * @brief Update the state of the bottom pump.
     */
    void on_bottom_pump_checkBox_stateChanged(int arg1);

    /**
     * @brief Update the calibration of the top soil sensor.
     */
    void on_cal_top_button_clicked();

    /**
     * @brief Update the calibration of the bottom soil sensor.
     */
    void on_cab_bottom_button_clicked();

    /**
     * @brief Update the setpoint of the top shelf water pump.
     */
    void on_top_setpoint_dial_actionTriggered(int action);

    /**
     * @brief Update the setpoint of the bottom shelf water pump.
     */
    void on_bott_setpoint_dial_actionTriggered(int action);

    /**
     * @brief Update the values displayed on the GUI.
     */
    void update_gui_values();

    /**
     * @brief Update the values utilized by the program.
     */
    void update_system_inputs();

private:

    /**
     * @brief Pointer to the GUI.
     */
    Ui::PlantCareSystemGUI *ui;

};
#endif // PLANTCARESYSTEMGUI_H
