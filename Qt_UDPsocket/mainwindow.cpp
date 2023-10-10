#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	//ui->te_result->document()->setMaximumBlockCount(10);
    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);
	connect(udpWorker, &UDPworker::sig_sendTextToGUI, this, &MainWindow::DisplayText);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

		udpWorker->SendDatagram(dataToSend, BIND_PORT);
        timer->start(TIMER_DELAY);

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}


void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));

}

void MainWindow::DisplayText(QString text)
{
	ui->te_result->append(text);
}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}


void MainWindow::on_pb_send_clicked()
{
	QString text = ui->te_transmit->toPlainText();
	QByteArray dataToSend;
	QDataStream outStr(&dataToSend, QIODevice::WriteOnly);
	outStr << text;
	udpWorker->SendDatagram(dataToSend, UDP_TEXT_PORT);
}

