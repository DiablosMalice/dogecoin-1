// Copyright (c) 2011-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <algorithm>
#if defined(HAVE_CONFIG_H)
#include "config/bitcoin-config.h"
#endif

#include <iostream>
#include <string>

#include "peerdialog.h"

#include "ui_helpmessagedialog.h"
#include "ui_paperwalletdialog.h"

#include "bitcoinunits.h"

#ifdef ENABLE_WALLET
#include "coincontroldialog.h"
#include "sendcoinsdialog.h"
#include "sendcoinsentry.h"
#endif

#include "bitcoingui.h"
#include "clientmodel.h"
#include "guiconstants.h"
#include "guiutil.h"
#include "intro.h"
#include "optionsmodel.h"
#include "rpcconsole.h"

#include "clientversion.h"
#include "init.h"
#include "net.h"
#include "util.h"
#include "utilstrencodings.h"

#include "chainparams.h"
#include "netbase.h"
#include "rpc/server.h"
#include "rpc/client.h"

#include <stdio.h>

#include <QCloseEvent>
#include <QFont>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegExp>
#include <QTextCursor>
#include <QTextTable>
#include <QVBoxLayout>
#include <QObject>
#include <QWidget>
#include <QMessageBox>
#include <QSignalMapper>
#include <QThread>

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#ifdef USE_QRCODE
#include <qrencode.h>
#endif

#include "walletmodel.h"
#include <QPainter>

/** Add Peer Dialog */
AddPeerDialog::AddPeerDialog(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    this->resize(466, 186);
    QVBoxLayout* verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QString("verticalLayout"));

    QLabel* label = new QLabel(this);
    label->setObjectName(QString("label"));

    QFont font;
    font.setPointSize(11);
    font.setBold(true);
    font.setWeight(75);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);

    verticalLayout->addWidget(label);

    QLabel* label_2 = new QLabel(this);
    label_2->setObjectName(QString("label_2"));
    label_2->setAlignment(Qt::AlignCenter);

    verticalLayout->addWidget(label_2);

    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(QString("lineEdit"));

    verticalLayout->addWidget(lineEdit);

    QPushButton* pushButton = new QPushButton(this);
    pushButton->setObjectName(QString("pushButton"));

    verticalLayout->addWidget(pushButton);

    this->setWindowTitle("Add Peer");
    label->setText("Enter the peer's address below.");
    label_2->setText("Be careful! Do not blindly trust anyone that tells you to add their node.");
    lineEdit->setPlaceholderText("Enter the peer's address and port if needed.");
    pushButton->setText("Add!");

    connect(pushButton, SIGNAL(clicked()), this, SLOT(on_addPeer_clicked()));
}

void AddPeerDialog::on_addPeer_clicked()
{
    QString peer = AddPeerDialog::lineEdit->text();
    QString data = "";

    if(peer.isEmpty()) 
    {
         QMessageBox::critical(this, "Add Peer", "Please enter an address.", QMessageBox::Ok, QMessageBox::Ok);
         return;
    }

    data = peer.contains(":") ? peer : peer + ":" + "22556";

    Q_EMIT cmdRequest("addnode " + data + " add");
}

void AddPeerDialog::message(int category, const QString &message)
{
    int result = QMessageBox::critical(this, "RPC Handler", message, QMessageBox::Ok, QMessageBox::Ok);

    if(result == QMessageBox::Ok)
    {
        Q_EMIT stopExecutor();
        this->close();
    }
}

/** Remove Peer Dialog */
RemovePeerDialog::RemovePeerDialog(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    this->resize(466, 186);
    QVBoxLayout* verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QString("verticalLayout"));

    QLabel* label = new QLabel(this);
    label->setObjectName(QString("label"));

    QFont font;
    font.setPointSize(11);
    font.setBold(true);
    font.setWeight(75);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);

    verticalLayout->addWidget(label);

    QLabel* label_2 = new QLabel(this);
    label_2->setObjectName(QString("label_2"));
    label_2->setAlignment(Qt::AlignCenter);

    verticalLayout->addWidget(label_2);

    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(QString("lineEdit"));

    verticalLayout->addWidget(lineEdit);

    QPushButton* pushButton = new QPushButton(this);
    pushButton->setObjectName(QString("pushButton"));

    verticalLayout->addWidget(pushButton);

    this->setWindowTitle("Remve Peer");
    label->setText("Enter the peer's address below.");
    label_2->setText("Be careful! Do not blindly trust anyone that tells you to add their node.");
    lineEdit->setPlaceholderText("Enter the peer's address and port if needed.");
    pushButton->setText("Remove!");

    connect(pushButton, SIGNAL(clicked()), this, SLOT(on_removePeer_clicked()));
}

void RemovePeerDialog::on_removePeer_clicked()
{
    QString peer = RemovePeerDialog::lineEdit->text();
    QString data = "";

    if(peer.isEmpty()) 
    {
         QMessageBox::critical(this, "Remove Peer", "Please enter an address.", QMessageBox::Ok, QMessageBox::Ok);
         return;
    }

    data = peer.contains(":") ? peer : peer + ":" + "22556";

    // Q_EMIT cmdRequest("getaddednodeinfo");
    Q_EMIT cmdRequest("addnode " + data + " remove");
}

void RemovePeerDialog::message(int category, const QString &message)
{
    //** Todo, get all peers and parse via JSON */
    // QJsonDocument doc(QJsonDocument::fromJson(message.toUtf8()));
    // QJsonObject json = doc.object();

    // for (const QString& key : json.keys())
    // {
    //     QJsonValue value = json.value(key);
    //     QMessageBox::critical(this, "RPC Json Loop", value.toString(), QMessageBox::Ok, QMessageBox::Ok);
    // }
    
    int result = QMessageBox::critical(this, "RPC Handler", message, QMessageBox::Ok, QMessageBox::Ok);

    if(result == QMessageBox::Ok)
    {
        Q_EMIT stopExecutor();
        this->close();
    }
}

/** Add Test Peer Dialog */
TestPeerDialog::TestPeerDialog(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    this->resize(466, 186);
    QVBoxLayout* verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QString("verticalLayout"));

    QLabel* label = new QLabel(this);
    label->setObjectName(QString("label"));

    QFont font;
    font.setPointSize(11);
    font.setBold(true);
    font.setWeight(75);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);

    verticalLayout->addWidget(label);

    QLabel* label_2 = new QLabel(this);
    label_2->setObjectName(QString("label_2"));
    label_2->setAlignment(Qt::AlignCenter);

    verticalLayout->addWidget(label_2);

    lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(QString("lineEdit"));

    verticalLayout->addWidget(lineEdit);

    QPushButton* pushButton = new QPushButton(this);
    pushButton->setObjectName(QString("pushButton"));

    verticalLayout->addWidget(pushButton);

    this->setWindowTitle("Test Peer");
    label->setText("Enter the peer's address below.");
    label_2->setText("Be careful! Do not blindly trust anyone that tells you to add their node.");
    lineEdit->setPlaceholderText("Enter the peer's address and port if needed.");
    pushButton->setText("Test!");

    connect(pushButton, SIGNAL(clicked()), this, SLOT(on_testPeer_clicked()));
}

void TestPeerDialog::on_testPeer_clicked()
{
    QString peer = TestPeerDialog::lineEdit->text();
    QString data = "";

    if(peer.isEmpty()) 
    {
         QMessageBox::critical(this, "Test Peer", "Please enter an address.", QMessageBox::Ok, QMessageBox::Ok);
         return;
    }

    data = peer.contains(":") ? peer : peer + ":" + "22556";

    Q_EMIT cmdRequest("addnode " + data + " onetry");
}

void TestPeerDialog::message(int category, const QString &message)
{
    int result = QMessageBox::critical(this, "RPC Handler", message, QMessageBox::Ok, QMessageBox::Ok);

    if(result == QMessageBox::Ok)
    {
        Q_EMIT stopExecutor();
        this->close();
    }
}