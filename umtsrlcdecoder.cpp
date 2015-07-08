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

#include "umtsrlcdecoder.h"

UmtsRlcDecoder::UmtsRlcDecoder()
{

}

UmtsRlcDecoder::~UmtsRlcDecoder()
{

}

/* UMTS RLC PDU decoding starts here */

void UmtsRlcDecoder::start_decoder(QString encoded_pdu, QString protocol_selected)
{
    if(protocol_selected.contains("AM"))
    {
        //It is a UMTS RLC AM PDU and will be decoded
        decode_rlc_acknowledged_pdu(encoded_pdu);
    }
    else if(protocol_selected.contains("UM"))
    {
        //It is a UMTS RLC UM PDU
        decode_rlc_unacknowledged_pdu(encoded_pdu);
    }
}

void UmtsRlcDecoder::decode_rlc_acknowledged_pdu(QString encoded_pdu)
{
    QString data_or_control;
    data_or_control = check_control_or_data_pdu(encoded_pdu);
    if(data_or_control == "data")
    {
        decode_am_data_pdu(encoded_pdu);
    }
    else
    {
        decode_am_control_pdu(encoded_pdu);
    }
    //decode_data_pdu();
    //decode_control_pdu();
}

QString UmtsRlcDecoder::check_control_or_data_pdu(QString encoded_pdu)
{
    QString data_or_control;
    int first_hex_data;
    first_hex_data = encoded_pdu.left(1).toInt();
    if((first_hex_data >> 3) == 1)
    {
        data_or_control = "data";
    }
    else
    {
        data_or_control = "control";
    }
    return data_or_control;
}

/* Decode RLC AM Pdus header
 * Data part can not be decoded as it is ciphered
 */

void UmtsRlcDecoder::decode_am_data_pdu(QString encoded_pdu)
{
    //This file stores the decoded pdu information
    QFile decoded_data_file("decode_output_temp.txt");
    int fixed_am_header;
    int header_item;
    bool ok;
    //decoded_data_file.resize(0);
    QTextStream stream(&decoded_data_file);
    //First 2 octet contains sequence number and extension bit information
    fixed_am_header = encoded_pdu.left(4).toInt(&ok, 16);
    qDebug() << "AM data pdu header information" << fixed_am_header;

    //Feed the file with information
    if (decoded_data_file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        stream << "Data or Control (D/C) [1 Bit ]\t:: 1\tAcknowledged mode data PDU \n";
        header_item = (fixed_am_header & 32767) >> 3; // Sequence number is 12 bit long
        stream << "Sequence Number [12 Bits]\t:: " << header_item <<"\n";
        header_item = (fixed_am_header & 7) >> 2; // Polling bit - 1 bit to know if polling is requested or not
        stream << "Polling [1 bit]\t\t:: " << header_item;
        if(header_item == 0)
        {
            stream << "\tPolling not requested\n";
        }
        else
        {
            stream << "\tPolling requested\n";
        }
        header_item = fixed_am_header & 3;
        stream << "Header Extension [2 bits]\t:: " << header_item << "\t";
        if(header_item == 0)
        {
            stream << "Next octetct onward data or padding follows \n\n";
            stream << "Data + Padding :: " << encoded_pdu.remove(0,4);
        }
        else if(header_item == 1)
        {
            stream << "Next octect contanins length indicator \n";
            //Extension bit 1 means more length indicator present and 0 means data and padding follows
            int extension_bit = 1;
            int length_indicator_extension_bit;
            encoded_pdu = encoded_pdu.remove(0,4);
            while(extension_bit != 0)
            {
                length_indicator_extension_bit = encoded_pdu.left(2).toInt(&ok, 16); // First byte is length indicator + extension bit
                header_item = length_indicator_extension_bit >> 1; // 7 bits length indicator. We have not handled for 15 bit. support will be added in future
                stream << "Lenth Indicator [7 bits]\t:: " << header_item << "\t";
                if(header_item == 126)
                {
                    stream << "Piggybacked status PDU is present instead of padding\n";
                }
                else if(header_item == 127)
                {
                    stream << "After data padding is present\n";
                }
                else if(header_item == 125)
                {
                    stream << "Reserved length indicator. Not valid\n";
                }
                else
                {
                    stream << "\n";
                }
                extension_bit = length_indicator_extension_bit & 1;
                stream << "Extension bit [1 bit]\t:: " << extension_bit << "\t";
                if(extension_bit == 1)
                {
                   stream << "Next octect contanins length indicator \n";
                }
                else
                {
                    stream << "Next octetct onward data or padding follows \n\n";
                    stream << "Data + Padding :: " << encoded_pdu.remove(0,2);
                }
                encoded_pdu = encoded_pdu.remove(0,2);
            }
        }
        else
        {
            stream << "Invalid header extension. Value should be 00 or 01 \n";
        }
    }
   decoded_data_file.close();
}


/* Decode RLC AM Control Pdus header
 */

void UmtsRlcDecoder::decode_am_control_pdu(QString encoded_pdu)
{
    QFile decoded_data_file("decode_output_temp.txt");
    QString pdu_for_sufi_decoding;
    int fixed_control_pdu_header;
    int header_item;
    int sufi_field;
    int sufi_list_sni; // Sufi list Sni value
    int sufi_list_li; // Sufi list Li value
    int sufi_length; // Length filed for different Sufi
    int sufi_rlist_cw; // CW for Sufi Rlist
    bool ok;
    //decoded_data_file.resize(0);
    QTextStream stream(&decoded_data_file);
    //First 2 octet contains sequence number and extension bit information
    fixed_control_pdu_header = encoded_pdu.left(2).toInt(&ok, 16);
    qDebug() << "AM control pdu header information" << fixed_control_pdu_header;
    if (decoded_data_file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        stream << "Data or Control (D/C) [1 Bit ]\t:: 0\tAcknowledged mode Control PDU \n";
        header_item = fixed_control_pdu_header >> 4; // Status PDU, Reset or Reset-Ack PDU
        /*switch (header_item) {
        case 0:
            decode_status_pdu(encoded_pdu);
            break;
        case 1:
            decode_reset_pdu(encoded_pdu);
            break;
        case 2:
            decode_reset_ack_pdu(encoded_pdu);
            break;
        default:
            break;
        }*/

        // Decode for AM status PDU
        if(header_item == 0)
        {
            stream << "PDU Type [3 Bits ]\t:: 000\tStatus PDU \n";
            sufi_field = fixed_control_pdu_header & 15;

            if(sufi_field == 0) //Sufi No_more
            {
                stream << "Sufi Type [4 bits]      :: 0000        Sufi No_More\n\n";
                stream << "Rest part of the PDU is data or part of a SDU\n\n";
                stream << encoded_pdu.remove(0,2);
            }
            else if(sufi_field == 1) //Sufi_window
            {
                //Add sufi_window decoding
            }
            else if(sufi_field == 2) //Sufi_Ack
            {
                //Sufi_ack decoding: Get Last Sequence Number
                stream << "Sufi Type [4 bits]\t:: 0010\tSufi_ack\n";
                pdu_for_sufi_decoding = encoded_pdu.remove(0,2);
                header_item = pdu_for_sufi_decoding.left(3).toInt(&ok, 16);
                stream <<"Last Sequence Number\t::" << header_item << "\tAll Pdus with Sequence number < Lsn are correctly received\n ";
            }
            else if(sufi_field == 3) //Sufi_list
            {
                /* Sufi_list has the following structure
                 * Type: Sufi List
                 * Length: Number of SNi,Li pairs
                 * SNi: "Sequence Number" of AMD PDU, which was not correctly received
                 * Li: Number of consecutive AMD PDUs not correctly received following AMD PDU with "Sequence Number" SNi.
                 */
                stream << "Sufi Type [4 bits]\t:: 0011\tSufi_list\n";
                pdu_for_sufi_decoding = encoded_pdu.remove(0,2);
                sufi_length = pdu_for_sufi_decoding.left(1).toInt(&ok, 16);
                stream << "Sufi_list length [4 bits]\t:: " << sufi_length << "\tNumber of SNi,Li pairs/n";
                pdu_for_sufi_decoding = pdu_for_sufi_decoding.remove(0,1); // Remove the length field as it is already decoded
                for(int i = 0; i < sufi_length; i++)
                {
                    sufi_list_sni = pdu_for_sufi_decoding.left(3).toInt(&ok, 16);
                    sufi_list_li = pdu_for_sufi_decoding.mid(4,1).toInt(&ok, 16);
                    stream << "Sufi_list Sni [12 bits]\t:: " << sufi_list_sni << "\tMissed sequence number\n";
                    stream << "Sufi_list Li [4 bits]\t:: " << sufi_list_li << "\tNumber of consecutive Pdus not received correctky after Sni\n";
                    pdu_for_sufi_decoding = pdu_for_sufi_decoding.remove(0,4);
                }
            }
            else if(sufi_field == 4) //Sufi bitmap
            {
                //Sufi bitmap decoding
            }
            else if(sufi_field == 5)
            {
                //Sufi Rlist decoding
                stream << "Sufi Type [4 bits]\t:: 0101\tSufi_Rlist\n";
                pdu_for_sufi_decoding = encoded_pdu.remove(0,2);
                sufi_length = pdu_for_sufi_decoding.left(1).toInt(&ok, 16);
                stream << "Sufi_rlist length [4 bits]\t:: " << sufi_length << "\tNumber of codewords/n";
                pdu_for_sufi_decoding = pdu_for_sufi_decoding.remove(0,1);
                for(int i = 0; i < sufi_length; i++)
                {
                    sufi_rlist_cw = pdu_for_sufi_decoding.left(1).toInt(&ok, 16);
                    stream << "Sufi_rlist CW [4 bits]\t:: " << sufi_rlist_cw << "/n";
                    pdu_for_sufi_decoding = pdu_for_sufi_decoding.remove(0,1);
                }
            }
            else if(sufi_field == 6)
            {
                //Sufi Mrw decoding
            }
            else if(sufi_field == 6)
            {
                //Sufi Mrw ack decoding
            }

        }
        else if(header_item == 1)
        {
            stream << "PDU Type [3 Bits ]         :: 001          Reset PDU \n";
        }
        else if(header_item == 2)
        {
            stream << "PDU Type [3 Bits ]         :: 010          Reset Ack PDU \n";
        }
        else
        {
            stream << "PDU Type [3 Bits ]         ::" << header_item << "           Invalid Control PDU \n";
        }
    }
   decoded_data_file.close();
}

/* Decode RLC UM pdu header part
 * data part is ciphered so can not be decoded */

void UmtsRlcDecoder::decode_rlc_unacknowledged_pdu(QString encoded_pdu)
{
    QFile decoded_data_file("decode_output_temp.txt");
    QTextStream stream(&decoded_data_file);
    int umd_sequence_number; //next header item to be decoded
    int extension_bit;
    int um_length_indicator;
    int rlc_um_fixed_header; //First byte is the fixed header for UM.
    int rlc_um_length_indicator_extension_bit;
    bool ok;
    if (decoded_data_file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        rlc_um_fixed_header = encoded_pdu.left(2).toInt(&ok,16);
        umd_sequence_number = rlc_um_fixed_header >> 1; //Sequence number is 7 bit
        stream << "Sequence Number[7 bits]\t:: " << umd_sequence_number <<"\n";
        extension_bit = rlc_um_fixed_header & 1; // Last bit is the extension bit
        stream << "Extension bit[1 bit]\t:: " << extension_bit <<"\t";
        if(extension_bit == 0)
        {
            stream << "Rest part of the Pdu is data and padding\n";
            stream << "Data + Padding :: \t" << encoded_pdu.remove(0,2);
        }
        else
        {
            stream << "Length indicator follows\n";
            encoded_pdu = encoded_pdu.remove(0,2);
            while(extension_bit == 1)
            {
                rlc_um_length_indicator_extension_bit = encoded_pdu.left(2).toInt(&ok,16);
                um_length_indicator = rlc_um_length_indicator_extension_bit >> 1;
                stream << "Length indicator[7 bits]\t:: " << um_length_indicator <<"\n";
                extension_bit = rlc_um_length_indicator_extension_bit & 1;
                stream << "Extension bit[1 bit]\t:: " << extension_bit <<"\t";
                if(extension_bit == 0)
                {
                    stream << "Rest part of the Pdu is data and padding\n";
                    stream << "Data + Padding :: \t" << encoded_pdu.remove(0,2);
                }
                else
                {
                    stream << "Length indicator follows\n";
                    encoded_pdu = encoded_pdu.remove(0,2);
                }
            }
        }
    }
}
