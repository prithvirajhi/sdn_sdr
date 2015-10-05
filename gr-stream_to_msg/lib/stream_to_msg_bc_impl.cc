/* -*- c++ -*- */
/* 
 * Copyright 2015 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <gnuradio/blocks/pdu.h>
#include <gnuradio/blocks/socket_pdu.h>
#include <gnuradio/thread/thread.h>
#include <pmt/pmt.h>
#include <boost/asio.hpp>

#include "stream_to_msg_bc_impl.h"
//#include "socket_pdu_impl.h"


//#define PDU_PORT_ID   pmt::mp("pdus")

namespace gr {
  namespace stream_to_msg {

    stream_to_msg_bc::sptr
    stream_to_msg_bc::make(std::string type, std::string addr, std::string port, int MTU)
    {
      return gnuradio::get_initial_sptr
        (new stream_to_msg_bc_impl(type, addr, port, MTU));
    }

    /*
     * The private constructor
     */
    stream_to_msg_bc_impl::stream_to_msg_bc_impl(std::string type, std::string addr, std::string port, int MTU)
      : gr::block("stream_to_msg_bc",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        d_rxbuf.resize(MTU);
        message_port_register_out(PDU_PORT_ID);

        if((addr.empty()) || (addr == "0.0.0.0")) {
            int port_num = atoi(port.c_str());
            if (port_num == 0)
                throw std::invalid_argument("gr::blocks:socket_pdu: invalid port for UDP_SERVER");
            d_udp_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port_num);
        } else {
            boost::asio::ip::udp::resolver resolver(d_io_service);
            boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(),
                                                        addr, port,
                                                        boost::asio::ip::resolver_query_base::passive);
            d_udp_endpoint = *resolver.resolve(query);
        }

        d_udp_socket.reset(new boost::asio::ip::udp::socket(d_io_service, d_udp_endpoint));
        d_udp_socket->async_receive_from(boost::asio::buffer(d_rxbuf), d_udp_endpoint_other,
                                         boost::bind(&stream_to_msg_bc_impl::handle_udp_read, this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
        d_thread = gr::thread::thread(boost::bind(&stream_to_msg_bc_impl::run_io_service, this));
        //d_started = true;

    }

    void 
    stream_to_msg_bc_impl::ae_process_config(GNU_SS_MESSAGE_CONFIG* cfg, size_t len)
    {
        size_t i;
        int change = 0;

        GR_LOG_DEBUG(d_logger,"Process config message");

        /*if (len < offsetof(GNU_SS_MESSAGE_CONFIG, u))
        {
            GR_LOG_WARN(d_logger, "Bad config message");
            return;
        }*/

        //len -= offsetof(GNU_SS_MESSAGE_CONFIG, u);

        //switch(cfg->config_type){
        //
        if (len < 2)
        {
            GR_LOG_WARN(d_logger, "Bad config message, type = ");
            return;
        } 
        if (cfg == NULL)
        {
            GR_LOG_WARN(d_logger, "cfg is null ");
            return;
        }
        pmt::pmt_t command = pmt::make_dict();
        
        if (cfg->gain != 0) {
             pmt::dict_add(command, pmt::mp("gain"),pmt::mp(cfg->gain));
             change++;
        }
        if (cfg->freq != 0) {
             pmt::dict_add(command, pmt::mp("freq"),pmt::mp(cfg->freq));
             change++;
        }
        //d_pdu_vector = pdu::make_pdu_vector(d_type, in, ninput_items[0]);

        // Send msg
        //pmt::pmt_t msg = pmt::cons(d_pdu_meta, d_pdu_vector);
        if (change != 0) {
            GR_LOG_DEBUG(d_logger, "Sending message to UHD port");
            message_port_pub(PDU_PORT_ID, command);
        }

    }
    void
    stream_to_msg_bc_impl::handle_udp_read(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if(!error) {
            pmt::pmt_t vector = pmt::init_u8vector(bytes_transferred, (const uint8_t *)&d_rxbuf[0]);
            uint8_t *buf = (uint8_t *)&d_rxbuf[0];
            GNU_SS_MESSAGE *msg = (GNU_SS_MESSAGE *) buf;

            if(bytes_transferred < offsetof(GNU_SS_MESSAGE, u) {
                GR_LOG_WARN(d_logger, "Received bad message");
                //return;
            }

            switch(msg->type) {
                case GNU_SS_NOTIFY_REQUEST:
                    break;
                case GNU_SS_STATS_REQ:
                    break;
                case GNU_SS_CONFIG:
                    ae_process_config(&msg->config.u,
                                      bytes_transferred - offset(GNU_SS_MESSAGE, u));
                    break;
                default:
                    GR_LOG_WARN(d_logger, "Received unknown message");
            }
            d_udp_socket->async_receive_from(boost::asio::buffer(d_rxbuf), d_udp_endpoint_other,
                                         boost::bind(&stream_to_msg_bc_impl::handle_udp_read, this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
        } else {
            GR_LOG_WARN(d_logger, "Error received while reading message");
        }  
    }
             

    /*
     * Our virtual destructor.
     */
//    stream_to_msg_bc_impl::~stream_to_msg_bc_impl()
//    {
//    }

  } /* namespace stream_to_msg */
} /* namespace gr */

