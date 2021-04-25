// Copyright (c) 2021 The Dogecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/bitcoin-config.h"
#endif

#include "importkeysdialog.h"
#include "ui_importkeysdialog.h"

#include "guiutil.h"
#include "platformstyle.h"
#include "validation.h"
#include "walletmodel.h"

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
    accept();
    importKey();
}

void ImportKeysDialog::on_cancelButton_clicked()
{
    reject();
}

void ImportKeysDialog::on_resetButton_clicked()
{
    resetDialogValues();
}

void ImportKeysDialog::importKey()
{
    QString privateKey, privateKeyLabel;

    privateKey      = ui->privateKey->text();
    privateKeyLabel = ui->privateKeyLabel->text();
    resetDialogValues();
}

void ImportKeysDialog::resetDialogValues()
{
    ui->privateKey->setText("");
    ui->privateKeyLabel->setText("");
    ui->rescanCheckBox->setCheckState(Qt::Unchecked);
}

void ImportKeysDialog::setOkButtonState(bool fState)
{
    ui->okButton->setEnabled(fState);
}
