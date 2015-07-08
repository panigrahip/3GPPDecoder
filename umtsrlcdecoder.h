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

#ifndef UMTSRLCDECODER_H
#define UMTSRLCDECODER_H

#include <QString>
#include <QFile>
#include <QDebug>


class UmtsRlcDecoder
{
public:
    UmtsRlcDecoder();
    ~UmtsRlcDecoder();
    void start_decoder(QString encoded_pdu, QString protocol_selected);
private:
    void decode_rlc_acknowledged_pdu(QString encoded_pdu);
    void decode_rlc_unacknowledged_pdu(QString encoded_pdu);
    QString check_control_or_data_pdu(QString encoded_pdu);
    void decode_am_data_pdu(QString encoded_pdu);
    //Control PDU Decoding
    void decode_am_control_pdu(QString encoded_pdu);
    void decode_status_pdu(QString encoded_pdu);
    void decode_reset_pdu(QString encoded_pdu);
    void decode_reset_ack_pdu(QString encoded_pdu);
};

#endif // UMTSRLCDECODER_H
