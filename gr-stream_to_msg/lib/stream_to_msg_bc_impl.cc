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
#include "stream_to_msg_bc_impl.h"

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
        d_started = true;

    }

    void
    socket_pdu_impl::handle_tcp_read(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if(!error) {
            pmt::pmt_t vector = pmt::init_u8vector(bytes_transferred, (const uint8_t *)&d_rxbuf[0]);
            uint8_t *buf = (uint8_t *)&d_rxbuf[0];
            GNU_SS_MESSAGE *msg = (GNU_SS_MESSAGE *) buf;

            if(bytes_transferred < offsetof(GNU_SS_MESSAGE, u) {
                GR_LOG_WARN(d_logger, "Received bad message");
            }

            switch(msg->type) {
                case GNU_SS_NOTIFY_REQUEST:
                    break;
                case GNU_SS_STATS_REQ:
                    break;
                case GNU_SS_CONFIG:
                    ae_process_config(&msg->u.config,
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
            GR_LOG_WARN(d_logger, "Error received while reading message %d ", error);
        }  
    }
             

    /*
     * Our virtual destructor.
     */
    stream_to_msg_bc_impl::~stream_to_msg_bc_impl()
    {
    }

    void
    stream_to_msg_bc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    stream_to_msg_bc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const <+ITYPE*> *in = (const <+ITYPE*> *) input_items[0];
        <+OTYPE*> *out = (<+OTYPE*> *) output_items[0];

        // Do <+signal processing+>
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace stream_to_msg */
} /* namespace gr */

