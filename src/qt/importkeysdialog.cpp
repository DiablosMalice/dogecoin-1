// Copyright (c) 2021 The Dogecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/bitcoin-config.h"
#endif

#include "importkeysdialog.h"
#include "ui_importkeysdialog.h"

#include "base58.h"
#include "guiutil.h"
#include "platformstyle.h"
#include "validation.h"
#include "wallet/wallet.h"
#include "walletmodel.h"
#include "util.h"

ImportKeysDialog::ImportKeysDialog(const PlatformStyle *_platformStyle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportKeysDialog),
    platformStyle(_platformStyle)
{
    ui->setupUi(this);

    /* Main elements init */
    if (fPruneMode) {
        ui->rescanCheckBox->setEnabled(false);
    }
}

ImportKeysDialog::~ImportKeysDialog()
{
    delete ui;
}

void ImportKeysDialog::on_okButton_clicked()
{
    if (importKey()) {
        resetDialogValues();
        accept();
    };
}

void ImportKeysDialog::on_cancelButton_clicked()
{
    reject();
}

void ImportKeysDialog::on_resetButton_clicked()
{
    resetDialogValues();
}

bool ImportKeysDialog::importKey()
{
    const QString privateKey      = ui->privateKey->text();
    const QString privateKeyLabel = ui->privateKeyLabel->text();
    const bool rescan             = ui->rescanCheckBox->isChecked();

    resetDialogValues();

    CBitcoinSecret vchSecret;
    bool fGood = vchSecret.SetString(privateKey.toStdString());
    if (!fGood) {
        vchSecret.SetString("");
        ui->privateKeyImportTextMessage->setText(tr("Invalid private key; please check and try again!"));
        return false;
    }

    CKey key = vchSecret.GetKey();
    if (!key.IsValid()) {
        vchSecret.SetString("");
        ui->privateKeyImportTextMessage->setText(tr("Invalid private key; please check and try again!"));
        return false;
    }

    CPubKey pubkey = key.GetPubKey();
    assert(key.VerifyPubKey(pubkey));
    CKeyID vchAddress = pubkey.GetID();

    pwalletMain->MarkDirty();
    pwalletMain->SetAddressBook(vchAddress, privateKeyLabel.toStdString(), "receive");

    if (pwalletMain->HaveKey(vchAddress)) {
        vchSecret.SetString("");
        ui->privateKeyImportTextMessage->setText(
            tr("Invalid address generated from private key; please check and try again!"
        ));
        return false;
    }

    pwalletMain->mapKeyMetadata[vchAddress].nCreateTime = 1;

    if (!pwalletMain->AddKeyPubKey(key, pubkey)) {
        vchSecret.SetString("");
        ui->privateKeyImportTextMessage->setText(tr("Couldn't add private key; does it already exist?"));
        return false;
    }

    pwalletMain->UpdateTimeFirstKey(1);

    if (rescan)
        pwalletMain->ScanForWalletTransactions(chainActive.Genesis(), true);

    vchSecret.SetString("");
    return true;
}

void ImportKeysDialog::resetDialogValues()
{
    ui->privateKey->clear();
    ui->privateKeyLabel->clear();
    ui->rescanCheckBox->setCheckState(Qt::Unchecked);
}

void ImportKeysDialog::setOkButtonState(bool fState)
{
    ui->okButton->setEnabled(fState);
}
