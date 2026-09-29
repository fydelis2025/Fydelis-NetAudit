#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), clienteConectado(nullptr) {
    setWindowTitle("FYDELIS CONTROLE — PAINEL DE COMANDO");
    setMinimumSize(800, 500);

    setStyleSheet(R"(
        QMainWindow { background-color: #0a1a0a; }
        QTextEdit {
            background-color: #0a1a0a;
            color: #00ff41;
            border: 2px solid #00aa22;
            padding: 12px;
            font-family: Consolas, Monospace;
            font-size: 11pt;
        }
        QLineEdit {
            background-color: #0f2f0f;
            color: #88ffaa;
            border: 2px solid #00aa22;
            padding: 8px;
            font-family: Consolas;
            font-size: 11pt;
        }
        QLabel {
            color: #88ffaa;
            font-family: Consolas;
        }
    )");

    painel = new QTextEdit(this);
    painel->setReadOnly(true);

    entradaComando = new QLineEdit(this);
    entradaComando->setPlaceholderText("Comandos: CMD_LOCK | CMD_UNLOCK | CMD_INFO | SAIR");

    QLabel *label = new QLabel("COMANDO > ");
    QHBoxLayout *linha = new QHBoxLayout;
    linha->addWidget(label);
    linha->addWidget(entradaComando);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(painel);
    layout->addLayout(linha);

    QWidget *central = new QWidget(this);
    central->setLayout(layout);
    setCentralWidget(central);

    // Iniciar Servidor — aguarda o cliente conectar
    servidor = new QTcpServer(this);
    if (servidor->listen(QHostAddress::Any, 2502)) {
        painel->append("╔══════════════════════════════════════════════╗");
        painel->append("║   FYDELIS — PAINEL DE CONTROLE REMOTO         ║");
        painel->append("╚══════════════════════════════════════════════╝");
        painel->append("");
        painel->append(">> Aguardando conexão do agente...");
        painel->append(">> Porta 2502 aberta");
    } else {
        painel->append("[ERRO] Não foi possível iniciar: " + servidor->errorString());
    }

    connect(servidor, &QTcpServer::newConnection, this, &MainWindow::novaConexao);
    connect(entradaComando, &QLineEdit::returnPressed, this, &MainWindow::enviarComando);
}

void MainWindow::novaConexao() {
    clienteConectado = servidor->nextPendingConnection();
    
    painel->append("");
    painel->append("<< AGENTE CONECTADO: " + clienteConectado->peerAddress().toString());

    connect(clienteConectado, &QTcpSocket::readyRead, this, &MainWindow::respostaRecebida);
    connect(clienteConectado, &QTcpSocket::disconnected, this, [=](){
        painel->append(">> Agente desconectado. Aguardando...");
        clienteConectado = nullptr;
    });
}

void MainWindow::enviarComando() {
    if (!clienteConectado) {
        painel->append("[!] Nenhum agente conectado!");
        return;
    }

    QString cmd = entradaComando->text().trimmed();
    if (cmd.isEmpty()) return;

    painel->append(">> ENVIANDO: " + cmd);
    clienteConectado->write((cmd + "\n").toUtf8());
    clienteConectado->flush();
    entradaComando->clear();
}

void MainWindow::respostaRecebida() {
    QByteArray dados = clienteConectado->readAll();
    QString resp = QString::fromUtf8(dados).trimmed();
    painel->append("<< RESPOSTA:\n" + resp);
}