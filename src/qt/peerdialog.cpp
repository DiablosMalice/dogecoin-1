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
#include "ui_testpeerdialog.h"

#include "net.h"
#include "net_processing.h"
#include "netbase.h"
#include "protocol.h"
#include "util.h"

#include <stdio.h>

#include <QMessageBox>
#include <QHostAddress>
#include <QAbstractSocket>

/** Function to manage peers */
QString PeerTools::ManagePeer(QString type, QString peer)
{
    std::string peerAddress = peer.toStdString();

    if(!g_connman)
        return "Error: Peer-to-peer functionality missing or disabled";

    if (type == "onetry")
    {
        CAddress addr;
        g_connman->OpenNetworkConnection(addr, false, NULL, peerAddress.c_str());
        return "Attempted to one try node.";
    }

    if (type == "add")
    {
        if(!g_connman->AddNode(peerAddress))
            return "Error: Node already added";
    }
    else if(type == "remove")
    {
        if(!g_connman->RemoveAddedNode(peerAddress))
            return "Error: Node has not been added.";
    }

    return "Returned OK.";
}

/** Check if IP is vaild */
bool PeerTools::CheckIPAddress(QString ip)
{
    QHostAddress address(ip);

    if (QAbstractSocket::IPv4Protocol == address.protocol())
    {
        return true;
    }
    else if (QAbstractSocket::IPv6Protocol == address.protocol())
    {
        return true;
    }
    else
    {
        return false;
    }
}

/** Add Peer Dialog */
AddPeerDialog::AddPeerDialog(QWidget *parent) : 
    QWidget(parent),
    ui(new Ui::AddPeerDialog)
{
    ui->setupUi(this);

    ui->peerPort->setValidator( new QIntValidator(1, 65535, this) );

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_addPeer_clicked()));
}

AddPeerDialog::~AddPeerDialog()
{
    delete ui;
}

void AddPeerDialog::on_addPeer_clicked()
{
    QString address = ui->peerAddress->text();
    QString port = ui->peerPort->text();
    QString data = "";

    if(address.isEmpty()) 
    {
        QMessageBox::critical(this, "Add Peer", "Please enter an address.", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if(port.isEmpty()) 
    {
        QMessageBox::critical(this, "Add Peer", "Please enter a port. The default port is 22556 or 44556 for the testnet.", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if(!PeerTools::CheckIPAddress(address))
    {
        QMessageBox::critical(this, "Add Peer", "Please enter a vaild IP address.", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    data = address + ":" + port;

    if(QMessageBox::Ok == QMessageBox::information(this, "Add Peer", PeerTools::ManagePeer("add", data), QMessageBox::Ok, QMessageBox::Ok))
        this->close();
}

/** Add Test Peer Dialog */
TestPeerDialog::TestPeerDialog(QWidget *parent) : 
    QWidget(parent),
    ui(new Ui::TestPeerDialog)
{
    ui->setupUi(this);

    ui->peerPort->setValidator( new QIntValidator(1, 65535, this) );

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_testPeer_clicked()));
}

TestPeerDialog::~TestPeerDialog()
{
    delete ui;
}

void TestPeerDialog::on_testPeer_clicked()
{
    QString address = ui->peerAddress->text();
    QString port = ui->peerPort->text();
    QString data = "";

    if(address.isEmpty()) 
    {
        QMessageBox::critical(this, "Test Peer", "Please enter an address.", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if(port.isEmpty()) 
    {
        QMessageBox::critical(this, "Test Peer", "Please enter a port. The default port is 22556 or 44556 for the testnet.", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if(!PeerTools::CheckIPAddress(address))
    {
        QMessageBox::critical(this, "Test Peer", "Please enter a vaild IP address.", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    data = address + ":" + port;

    if(QMessageBox::Ok == QMessageBox::information(this, "Try Peer", PeerTools::ManagePeer("onetry", data), QMessageBox::Ok, QMessageBox::Ok))
        this->close();
}