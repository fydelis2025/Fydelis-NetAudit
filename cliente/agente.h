#ifndef AGENTE_H
#define AGENTE_H

#include <QObject>
#include <QTcpSocket>
#include <QWidget>

class Agente : public QObject {
    Q_OBJECT
public:
    explicit Agente(QObject *parent = nullptr);

private slots:
    void conectarAoServidor();
    void tentarReconectar();
    void processarComandos();

private:
    QTcpSocket *socket;
    QWidget *telaBloqueio;
    bool bloqueado;

    void bloquearTela();
    void desbloquearTela();
    void coletarEEnviarDados();
};

#endif // AGENTE_H