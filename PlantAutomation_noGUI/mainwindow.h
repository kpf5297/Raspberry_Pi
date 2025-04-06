#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void blue_LED_selection_changed();
    void green_LED_selection_changed();
    void updateGUI();
    void logEvent(const QString& eventName, const QString& details)

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
