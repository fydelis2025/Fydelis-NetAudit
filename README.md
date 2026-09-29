# Fydelis NetAudit / Lab-Socket

> Ferramenta educacional de simulação de comunicação TCP (Cliente-Servidor) desenvolvida para estudos de engenharia de redes, análise de tráfego e mitigação de vulnerabilidades através de firewall.

## 📌 Sobre o Projeto

O **Fydelis NetAudit** é um laboratório prático desenvolvido em **C++ utilizando o framework Qt**, projetado para demonstrar como opera a arquitetura de sockets TCP em redes locais. O projeto separa claramente as responsabilidades entre um nó controlado (Servidor) e um painel de controle administrativo (Cliente), servindo como base acadêmica para entender fluxos de pacotes, persistência de conexão e, crucialmente, **métodos de defesa ativa**.

---

## 🛠️ Arquitetura do Sistema

O laboratório é dividido em duas aplicações nativas independentes:

1. **Servidor (`/servidor`):** 
   * Compilado na máquina alvo.
   * Utiliza `QTcpServer` para escutar conexões de entrada na porta padrão `2502`.
   * Processa comandos remotos de forma assíncrona e executa gatilhos visuais/operacionais locais.
2. **Cliente (`/cliente`):** 
   * Executado na máquina controladora (painel do operador).
   * Conecta-se via `QTcpSocket` ao IP do alvo para despachar comandos de diagnóstico (`CMD_INFO`, `CMD_LOCK`, etc.) e receber o feedback em tempo real.

---

## 🚀 Comandos Suportados

| Comando | Descrição |
| :--- | :--- |
| `CMD_LOCK` | Dispara o bloqueio visual de tela no nó remoto. |
| `CMD_UNLOCK` | Libera a interface operacional do nó remoto. |
| `CMD_INFO` | Coleta e exibe metadados do sistema alvo (Hostname, SO, etc.). |
| `CMD_EXEC <ação>`| Simula a execução de comandos remotos controlados. |

---

## 🛡️ Enfoque Defensivo e Mitigação

Mais do que demonstrar a camada de transporte via sockets, o objetivo central deste laboratório é o estudo de **contramedidas de segurança**:

1. **Monitoramento de Conexões:** Uso de ferramentas de sistema (`netstat`, `ss`) para identificar conexões estabelecidas em portas atípicas.
2. **Defesa de Perímetro (Firewall):** Restrição de tráfego na porta `2502` utilizando regras de firewall (como UFW no Linux ou Firewall do Windows) para demonstrar a interrupção imediata do canal de comunicação do atacante.

---

⚠️ Aviso Legal
Este repositório tem finalidade estritamente educacional, acadêmica e de pesquisa em cibersegurança. O autor não se responsabiliza pelo uso indevido do código fora de ambientes controlados e com consentimento prévio.

## ⚙️ Compilação e Execução

Ambos os projetos utilizam o sistema de build do Qt (`qmake`):

### Servidor (Alvo)

```bash
cd servidor/
qmake servidor.pro
make
./servidor
