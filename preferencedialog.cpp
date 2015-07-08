/* This file is part of 3GPP Decoder project.
 * Copyright (C) 2015  Prashant Panigrahi prashant@3glteinfo.com
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "preferencedialog.h"
#include "ui_preferencedialog.h"

PreferenceDialog::PreferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);
    setDefaults();
}

PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

void PreferenceDialog::setDefaults()
{
    QString strCurrentWiresharkPath = "C:\\Program Files\\Wireshark\\";
    QFile wiresharkpath("config.txt");
    //Default Wireshark Path
    if (wiresharkpath.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&wiresharkpath);
        while (!stream.atEnd()){
            strCurrentWiresharkPath = stream.readLine();
            if(strCurrentWiresharkPath.contains("wireshark:"))
            {
                strCurrentWiresharkPath = strCurrentWiresharkPath.remove(0,10);
            }
        }
    }
    ui->lineEditWireshark->setText(strCurrentWiresharkPath);
}

void PreferenceDialog::on_buttonBox_accepted()
{
    QString strWiresharkDir;
    QFile configFile("config.txt");
    strWiresharkDir = "C:\\Program Files\\Wireshark\\";
    if(ui->lineEditWireshark->text().length()> 0)
    {
        strWiresharkDir = ui->lineEditWireshark->text();
        if (configFile.open(QIODevice::ReadWrite)) {
              QTextStream stream(&configFile);
              stream << "wireshark:" << strWiresharkDir;
          }
    }
}
