/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-20     Rick       the first version
 */
#include "rtthread.h"
#include "flashwork.h"
#include "radio_protocol.h"
#include "radio_protocol_mainunit_factory.h"

#define DBG_TAG "RADIO_PROTOCOL_FACTORY"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static void radio_frame_mainunit_factory_parse_heart(rx_format *rx_frame)
{
    tx_format tx_frame = {0};
    uint8_t data[3] = {0};
    data[0] = (rx_frame->rssi >> 8) & 0xFF;
    data[1] = (rx_frame->rssi) & 0xFF;
    data[2] = (rx_frame->snr) & 0xFF;

    tx_frame.msg_ack = RT_TRUE;
    tx_frame.msg_type = MSG_CONFIRMED_DOWNLINK;
    tx_frame.dest_addr = rx_frame->source_addr;
    tx_frame.source_addr = get_local_address();
    tx_frame.command = FACTORY_RF_TEST_CMD;
    tx_frame.tx_data = data;
    tx_frame.tx_len = 3;
    radio_mainunit_factory_command_send(&tx_frame);
    LOG_I("radio_mainunit_factory_send\r\n");
}

void radio_frame_mainunit_factory_parse(rx_format *rx_frame)
{
    if(rx_frame->dest_addr != get_local_address())
    {
        return;
    }

    switch(rx_frame->rx_data[0])
    {
    case FACTORY_RF_TEST_CMD:
        radio_frame_mainunit_factory_parse_heart(rx_frame);
        break;
    default:
        break;
    }
}

void radio_mainunit_factory_command_send(tx_format *tx_frame)
{
    unsigned short send_len = 0;

    send_len = set_lora_tx_byte(send_len,0xEF);
    send_len = set_lora_tx_byte(send_len,(NETID_TEST_ENV << 4) | NETWORK_VERSION);
    send_len = set_lora_tx_byte(send_len,(tx_frame->msg_ack << 7) | (DEVICE_TYPE_FACTORY_TOOL << 3) | tx_frame->msg_type);
    send_len = set_lora_tx_word(send_len,tx_frame->dest_addr);
    send_len = set_lora_tx_word(send_len,tx_frame->source_addr);
    send_len = set_lora_tx_byte(send_len,tx_frame->command);
    send_len = set_lora_tx_byte(send_len,tx_frame->tx_len);
    send_len = set_lora_tx_buffer(send_len,tx_frame->tx_data,tx_frame->tx_len);
    send_len = set_lora_tx_crc(send_len);
    lora_tx_enqueue(get_lora_tx_buf(),send_len,tx_frame->parameter);
}
