#include <QCoreApplication>
#include "agente.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    Agente agente;
    return a.exec();
}