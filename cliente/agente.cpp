#include "agente.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHostInfo>
#include <QTimer>

Agente::Agente(QObject *parent) : QObject(parent), bloqueado(false) {
    socket = new QTcpSocket(this);

    // Tela de bloqueio — escondida por padrão
    telaBloqueio = new QWidget();
    telaBloqueio->setStyleSheet("background-color: rgba(0,20,0,0.95);");
    QLabel *aviso = new QLabel(telaBloqueio);
    aviso->setText(R"(
        <div style='font-family:Consolas; color:#ff3333; font-size:24pt; text-align:center;'>
        <br><br><br>
        ⚠ TELA BLOQUEADA ⚠<br>
        <span style='font-size:14pt; color:#ffcc00;'>Controle remoto ativo</span>
        </div>
    )");
    aviso->setAlignment(Qt::AlignCenter);
    QVBoxLayout *bl = new QVBoxLayout(telaBloqueio);
    bl->addWidget(aviso);
    telaBloqueio->hide();

    connect(socket, &QTcpSocket::readyRead, this, &Agente::processarComandos);
    connect(socket, &QTcpSocket::disconnected, this, &Agente::tentarReconectar);

    conectarAoServidor();
}

void Agente::conectarAoServidor() {
    // Coloque o IP do seu painel/servidor aqui
    socket->connectToHost("127.0.0.1", 2502);
}

void Agente::tentarReconectar() {
    // Reconecta automaticamente se cair
    QTimer::singleShot(2000, this, &Agente::conectarAoServidor);
}

void Agente::processarComandos() {
    while (socket->canReadLine()) {
        QString cmd = QString::fromUtf8(socket->readLine()).trimmed();

        if (cmd == "CMD_LOCK") {
            bloquearTela();
            socket->write("[AGENTE] Tela BLOQUEADA\n");
        }
        else if (cmd == "CMD_UNLOCK") {
            desbloquearTela();
            socket->write("[AGENTE] Tela LIBERADA\n");
        }
        else if (cmd == "CMD_INFO") {
            coletarEEnviarDados();
        }
    }
}

void Agente::bloquearTela() {
    if (bloqueado) return;
    bloqueado = true;
    telaBloqueio->showFullScreen();
}

void Agente::desbloquearTela() {
    bloqueado = false;
    telaBloqueio->hide();
}

void Agente::coletarEEnviarDados() {
    QString dados = QString(
        "[DADOS DO SISTEMA]\n"
        "  Host:     %1\n"
        "  SO:       FydelisOS v2.2\n"
        "  Usuario:  estudante\n"
        "  Status:   Conectado\n"
    ).arg(QHostInfo::localHostName());
    
    socket->write(dados.toUtf8());
}