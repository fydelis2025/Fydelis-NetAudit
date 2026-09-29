#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void novaConexao();
    void respostaRecebida();
    void enviarComando();

private:
    QTcpServer *servidor;
    QTcpSocket *clienteConectado;
    QTextEdit *painel;
    QLineEdit *entradaComando;
};

#endif // MAINWINDOW_H