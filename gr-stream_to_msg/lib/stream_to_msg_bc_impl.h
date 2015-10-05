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

#ifndef INCLUDED_STREAM_TO_MSG_STREAM_TO_MSG_BC_IMPL_H
#define INCLUDED_STREAM_TO_MSG_STREAM_TO_MSG_BC_IMPL_H

#include <stream_to_msg/stream_to_msg_bc.h>

namespace gr {
  namespace stream_to_msg {

    class stream_to_msg_bc_impl : public stream_to_msg_bc
    {
     private:
      // Nothing to declare in this block.
         boost::asio::io_service d_io_service;
         std::vector<char> d_rxbuf;
         void run_io_service() { d_io_service.run(); }

         boost::asio::ip::udp::endpoint d_udp_endpoint;
         boost::asio::ip::udp::endpoint d_udp_endpoint_other;
         boost::shared_ptr<boost::asio::ip::udp::socket> d_udp_socket;
         void handle_udp_read(const boost::system::error_code& error, size_t bytes_transferred);


     public:
         stream_to_msg_bc_impl(std::string type, 
                               std::string addr,
                               std::string port,
                               int MTU);
//      ~stream_to_msg_bc_impl();

    };

  } // namespace stream_to_msg
} // namespace gr

#endif /* INCLUDED_STREAM_TO_MSG_STREAM_TO_MSG_BC_IMPL_H */

