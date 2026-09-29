#ifndef FYDELIS_SERVER_H
#define FYDELIS_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QWidget>
#include <QLabel>

class FydelisServer : public QTcpServer {
    Q_OBJECT
public:
    explicit FydelisServer(QObject *parent = nullptr);

private:
    QWidget *telaBloqueio;
    bool bloqueado;
    void setupTelaBloqueio();
    void bloquearTela();
    void desbloquearTela();
    void processarComando(QTcpSocket *socket, const QString &cmd);

protected:
    void incomingConnection(qintptr socketDesc) override;
};

#endif