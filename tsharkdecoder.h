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

#ifndef TSHARKDECODER_H
#define TSHARKDECODER_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QStringRef>
#include <QDebug>


class TSharkDecoder
{
public:
    TSharkDecoder();
    ~TSharkDecoder();
    void startDecoder(QString strEncodedData, QString strProtocol);
protected:

private:
    void format_file_for_text2pcap(QString strData);
    QString preformatData(QString strEncodedData);
    void call_text2pacp(QString strTsharkPath);
    void call_tshark(QString strTsharkPath, QString strProtocol);
    void clean_output();
    QString getTsharkPath();
};

#endif // TSHARKDECODER_H
