// Copyright (c) 2011-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_PEERDIALOG_H
#define BITCOIN_QT_PEERDIALOG_H

#include <QDialog>
#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include "guiutil.h"
#include "walletmodel.h"

namespace Ui {
    class AddPeerDialog;
    class RemovePeerDialog;
    class TestPeerDialog;
}

/** "Add peer" dialog box */
class AddPeerDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AddPeerDialog(const PlatformStyle *platformStyle, QWidget *parent);
    ~AddPeerDialog();
private:
    Ui::AddPeerDialog *ui;
public Q_SLOTS:
    void message(int category, const QString &message);
private Q_SLOTS:
    void on_addPeer_clicked();
Q_SIGNALS:
    // For RPC command executor
    void stopExecutor();
    void cmdRequest(const QString &command);
};

/** "Remove peer" dialog box */
class RemovePeerDialog : public QWidget
{
    Q_OBJECT

public:
    explicit RemovePeerDialog(const PlatformStyle *platformStyle, QWidget *parent);
    ~RemovePeerDialog();
private:
    Ui::RemovePeerDialog *ui;
public Q_SLOTS:
    void message(int category, const QString &message);
private Q_SLOTS:
    void on_removePeer_clicked();
Q_SIGNALS:
    // For RPC command executor
    void stopExecutor();
    void cmdRequest(const QString &command);
};

/** "Test peer" dialog box */
class TestPeerDialog : public QWidget
{
    Q_OBJECT

public:
    explicit TestPeerDialog(const PlatformStyle *platformStyle, QWidget *parent);
    ~TestPeerDialog();
private:
    Ui::TestPeerDialog *ui;
public Q_SLOTS:
    void message(int category, const QString &message);
private Q_SLOTS:
    void on_testPeer_clicked();
Q_SIGNALS:
    // For RPC command executor
    void stopExecutor();
    void cmdRequest(const QString &command);
};

#endif
