#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345
#define UDP_TEXT_PORT 12346

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void ReadDatagram( QNetworkDatagram datagram);
	void SendDatagram(QByteArray data, int port);


private slots:
    void readPendingDatagrams(void);
	void readTextDatagrams( void );

private:
    QUdpSocket* serviceUdpSocket;
	QUdpSocket* textUdpSocket;

signals:
    void sig_sendTimeToGUI(QDateTime data);
	void sig_sendTextToGUI(QString text);

};

#endif // UDPWORKER_H
