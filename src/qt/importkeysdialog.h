// Copyright (c) 2021 The Dogecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_OPTIONSDIALOG_H
#define BITCOIN_QT_OPTIONSDIALOG_H

#include <QDialog>

class ImportKeysDialog;
class PlatformStyle;

namespace Ui {
class ImportKeysDialog;
}

/** Preferences dialog. */
class ImportKeysDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportKeysDialog(const PlatformStyle *_platformStyle, QWidget *parent = 0);
    ~ImportKeysDialog();

private:
    Ui::ImportKeysDialog *ui;
    const PlatformStyle *platformStyle;

private Q_SLOTS:
    /* set OK button state (enabled / disabled) */
    void setOkButtonState(bool fState);
    void on_resetButton_clicked();
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void resetDialogValues();

    /* import a private key */
    void importKey();
};

#endif // BITCOIN_QT_OPTIONSDIALOG_H
