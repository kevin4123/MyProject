#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qcontainerfwd.h>
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 连接串口接收信号
    connect(&serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_scan_clicked(void)
{
    // 清空下拉框
    ui->comboBox1->clear();

    // 获取所有可用串口的信息
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    // 遍历并显示
    for (const QSerialPortInfo &port : ports) {
        QString info = QString("%1")
                           .arg(port.portName());

        ui->comboBox1->addItem(info); // 加到下拉框中
    }

    if (ports.isEmpty()) {
        ui->comboBox1->addItem("No ports found");
    }
}

void MainWindow::on_start_clicked()
{
	if (serial.isOpen()) {
		ui->label->setText("🔴");

		// 设置文本
		ui->start->setText("open");

		// 关闭串口
		serial.close();
	} else{ 
		ui->label->setText("🟢");
		
		// 设置文本
		ui->start->setText("close");

		// 获取选中的串口名称
		QString portName = ui->comboBox1->currentText(); // 取 "COM3" 之类的名字

		// 设置串口名 
		serial.setPortName(portName);

		// 波特率
		int baudRate = ui->comboBox2->currentText().toInt();
		serial.setBaudRate(baudRate);

		// 数据位
		int dataBits = ui->comboBox3->currentText().toInt();
		switch (dataBits) {
		case 5: serial.setDataBits(QSerialPort::Data5); break;
		case 6: serial.setDataBits(QSerialPort::Data6); break;
		case 7: serial.setDataBits(QSerialPort::Data7); break;
		case 8: serial.setDataBits(QSerialPort::Data8); break;
		}

		// 停止位
		QString stopBits = ui->comboBox4->currentText();
		if (stopBits == "1") serial.setStopBits(QSerialPort::OneStop);
		else if (stopBits == "1.5") serial.setStopBits(QSerialPort::OneAndHalfStop);
		else if (stopBits == "2") serial.setStopBits(QSerialPort::TwoStop);
		
		// 校验位
		QString parity = ui->comboBox5->currentText();
		if (parity == "None") serial.setParity(QSerialPort::NoParity);
		else if (parity == "Even") serial.setParity(QSerialPort::EvenParity);
		else if (parity == "Odd") serial.setParity(QSerialPort::OddParity);

		// 打开串口
		if (!serial.open(QIODevice::ReadWrite)) {
			QMessageBox::warning(this, "Error", "Failed to open serial port!");
		}
	}
}


void MainWindow::on_send_clicked()
{
    // 先检查串口是否打开
    if (!serial.isOpen()) {
        QMessageBox::warning(this, "Error", "Please open the serial port first!");
        return;
    }

	QString text = ui->text_send->toPlainText();
	QStringList lines = text.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);

	for (QString line : lines) {
		QStringList bytes = line.split(" ", Qt::SkipEmptyParts);
		QByteArray sendData;
		for (QString b : bytes) {
			bool ok;
			char byte = b.toUInt(&ok, 16);
			if (ok) sendData.append(byte);
		}
		sendData.append('\n');  // 每行末尾加换行符
		serial.write(sendData);
		qDebug () << sendData;
	}
}

void MainWindow::readSerialData()
{	
    QByteArray data = serial.readAll();
    QString hexStr;

    for (unsigned char c : data) {
        if (c == '\n') {
            hexStr += "\n"; // 换行
        } else {
            if (c < 16) hexStr += "0";
            hexStr += QString::number(c, 16).toUpper();
            hexStr += " ";
        }
    }

    ui->text_receive->append(hexStr);

}


void MainWindow::on_clear_send_clicked(void)
{
	ui->text_send->clear();
	
}

void MainWindow::on_clear_receive_clicked(void)
{
	ui->text_receive->clear();
}
