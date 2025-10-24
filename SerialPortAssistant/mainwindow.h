#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <qtmetamacros.h>
#include <QDebug>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	QSerialPort serial;

private slots:
	void on_scan_clicked(void);
	void on_start_clicked(void);
	void on_clear_send_clicked(void);
	void on_send_clicked(void);
	void on_clear_receive_clicked(void);
	void readSerialData(void);



};
#endif // MAINWINDOW_H
