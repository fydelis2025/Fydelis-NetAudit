#include "fydelis_server.h"
#include <QVBoxLayout>
#include <QHostInfo>
#include <QDebug>

FydelisServer::FydelisServer(QObject *parent)
    : QTcpServer(parent), bloqueado(false)
{
    if (!listen(QHostAddress::Any, 2502)) {
        qDebug() << "[ERRO] Não foi possível iniciar na porta 2502:" << errorString();
        return;
    }
    qDebug() << "[OK] Servidor ativo — porta 2502";
    qDebug() << "[OK] Aguardando conexão do painel de controle...";
    setupTelaBloqueio();
}

void FydelisServer::setupTelaBloqueio() {
    telaBloqueio = new QWidget();
    telaBloqueio->setStyleSheet("background-color: rgba(0, 15, 0, 0.95);");
    telaBloqueio->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    QLabel *aviso = new QLabel(telaBloqueio);
    aviso->setText(R"(
        <div style='font-family:Consolas; text-align:center;'>
        <br><br><br><br>
        <span style='font-size:26pt; color:#ff3333; font-weight:bold;'>
        ⚠ TELA BLOQUEADA PELO CONTROLE REMOTO ⚠
        </span><br><br>
        <span style='font-size:14pt; color:#ffcc00;'>
        Aguardando liberação do painel de controle...
        </span>
        </div>
    )");
    aviso->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(telaBloqueio);
    layout->addWidget(aviso);
    telaBloqueio->hide();
}

void FydelisServer::bloquearTela() {
    if (bloqueado) return;
    bloqueado = true;
    telaBloqueio->showFullScreen();
    telaBloqueio->raise();
}

void FydelisServer::desbloquearTela() {
    bloqueado = false;
    telaBloqueio->hide();
}

void FydelisServer::processarComando(QTcpSocket *socket, const QString &cmd) {
    QString resposta;

    if (cmd == "CMD_LOCK") {
        bloquearTela();
        resposta = "[SERVIDOR] OK → Tela BLOQUEADA\n";
        qDebug() << "[!] Executado: BLOQUEIO DE TELA";
    }
    else if (cmd == "CMD_UNLOCK") {
        desbloquearTela();
        resposta = "[SERVIDOR] OK → Tela LIBERADA\n";
        qDebug() << "[✓] Executado: LIBERAÇÃO DE TELA";
    }
    else if (cmd == "CMD_INFO") {
        QString host = QHostInfo::localHostName();
        resposta = QString(
            "[DADOS DO SISTEMA]\n"
            "  Host:     %1\n"
            "  SO:       FydelisOS v2.2\n"
            "  Usuario:  estudante\n"
            "  Status:   Conectado\n"
        ).arg(host);
        qDebug() << "[!] Dados enviados para o controlador";
    }
    else {
        resposta = "[SERVIDOR] Comando desconhecido\n";
        qDebug() << "[?] Comando não reconhecido:" << cmd;
    }

    socket->write(resposta.toUtf8());
    socket->flush();
}

void FydelisServer::incomingConnection(qintptr socketDesc) {
    QTcpSocket *socket = new QTcpSocket(this);
    if (!socket->setSocketDescriptor(socketDesc)) {
        socket->deleteLater();
        return;
    }

    QString ip = socket->peerAddress().toString();
    if (ip.startsWith("::ffff:")) ip = ip.mid(7);
    qDebug() << "[+] Conexão recebida de:" << ip;

    connect(socket, &QTcpSocket::readyRead, this, [=]() {
        while (socket->canReadLine()) {
            QString cmd = QString::fromUtf8(socket->readLine()).trimmed();
            if (cmd.isEmpty()) continue;
            processarComando(socket, cmd);
        }
    });

    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}