#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{



}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
	textUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);
	textUdpSocket->bind(QHostAddress::LocalHost, UDP_TEXT_PORT);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
	connect(textUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readTextDatagrams);

}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{
	QByteArray data;
	data = datagram.data();

	QDataStream inStr(&data, QIODevice::ReadOnly);
	if(datagram.destinationPort() == BIND_PORT)
	{
		QDateTime dateTime;
		inStr >> dateTime;
		emit sig_sendTimeToGUI(dateTime);
	}
	else if (datagram.destinationPort() == UDP_TEXT_PORT)
	{
		QString text;
		inStr >> text;
		//int bytes = data.size();//совпадает с Wireshark
		int bytes = text.toUtf8().size();

		QString	msg = "Принято сообщение от ";
		msg.push_back(datagram.senderAddress().toString());
		msg.push_back(", размер сообщения(байт) ");
		msg.push_back(QString::number(bytes));
		//msg.push_back('\n' + text);

		emit sig_sendTextToGUI(msg);
	}
}
/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data, int port)
{
	if(port == BIND_PORT)
	{
		serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, port);
	}
	else if(port == UDP_TEXT_PORT)
	{
		textUdpSocket->writeDatagram(data, QHostAddress::LocalHost, port);
	}
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}
void UDPworker::readTextDatagrams( void )
{
	while(textUdpSocket->hasPendingDatagrams()){
			QNetworkDatagram datagram = textUdpSocket->receiveDatagram();
			ReadDatagram(datagram);
	}
}
