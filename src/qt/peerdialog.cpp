// Copyright (c) 2011-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/bitcoin-config.h"
#endif

#include <iostream>
#include <string>

#include "peerdialog.h"

#include "ui_addpeerdialog.h"
#include "ui_removepeerdialog.h"
#include "ui_testpeerdialog.h"

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
#include <QTextCursor>
#include <QTextTable>
#include <QVBoxLayout>
#include <QObject>
#include <QWidget>
#include <QMessageBox>
#include <QSignalMapper>
#include <QThread>

#ifdef USE_QRCODE
#include <qrencode.h>
#endif

#include "walletmodel.h"
#include <QPainter>

/** Add Peer Dialog */
AddPeerDialog::AddPeerDialog(const PlatformStyle *_platformStyle, QWidget *parent) : 
    QWidget(parent),
    ui(new Ui::AddPeerDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_addPeer_clicked()));
}

AddPeerDialog::~AddPeerDialog()
{
    delete ui;
}

void AddPeerDialog::on_addPeer_clicked()
{
    QString peer = ui->lineEdit->text();
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
    int result = QMessageBox::information(this, "RPC Handler", message, QMessageBox::Ok, QMessageBox::Ok);

    if(result == QMessageBox::Ok)
    {
        Q_EMIT stopExecutor();
        this->close();
    }
}

/** Remove Peer Dialog */
RemovePeerDialog::RemovePeerDialog(const PlatformStyle *_platformStyle, QWidget *parent) : 
    QWidget(parent),
    ui(new Ui::RemovePeerDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_removePeer_clicked()));
}

RemovePeerDialog::~RemovePeerDialog()
{
    delete ui;
}

void RemovePeerDialog::on_removePeer_clicked()
{
    QString peer = ui->lineEdit->text();
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
    int result = QMessageBox::information(this, "RPC Handler", message, QMessageBox::Ok, QMessageBox::Ok);

    if(result == QMessageBox::Ok)
    {
        Q_EMIT stopExecutor();
        this->close();
    }
}

/** Add Test Peer Dialog */
TestPeerDialog::TestPeerDialog(const PlatformStyle *_platformStyle, QWidget *parent) : 
    QWidget(parent),
    ui(new Ui::TestPeerDialog)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_testPeer_clicked()));
}

TestPeerDialog::~TestPeerDialog()
{
    delete ui;
}

void TestPeerDialog::on_testPeer_clicked()
{
    QString peer = ui->lineEdit->text();
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
    int result = QMessageBox::information(this, "RPC Handler", message, QMessageBox::Ok, QMessageBox::Ok);

    if(result == QMessageBox::Ok)
    {
        Q_EMIT stopExecutor();
        this->close();
    }
}