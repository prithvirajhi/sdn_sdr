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


#ifndef INCLUDED_STREAM_TO_MSG_STREAM_TO_MSG_BC_H
#define INCLUDED_STREAM_TO_MSG_STREAM_TO_MSG_BC_H

#include <stream_to_msg/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace stream_to_msg {

    /*!
     * \brief <+description of block+>
     * \ingroup stream_to_msg
     *
     */
   // enum GNU_CONFIG_TYPE
   // {
   //     GNU_GAIN = 0,
   //     GNU_FREQ
   // };

    struct GNU_SS_MESSAGE_CONFIG
    {
        uint8_t gain;
        uint8_t freq;
    }; 

    enum GNU_SS_MESSAGE_TYPE
    {
        GNU_SS_EVENT = 0,
        GNU_SS_STATS_REQ,
        GNU_SS_STATS_RES,
        GNU_SS_CONFIG,
        GNU_SS_PORTS_NOTIFY,
        GNU_SS_PORTS_NOTIFY_REQUEST
    };

    struct GNU_SS_MESSAGE
    {
        enum GNU_SS_MESSAGE_TYPE type;
        union u 
        {
            GNU_SS_MESSAGE_CONFIG config;
        };
    };
    class STREAM_TO_MSG_API stream_to_msg_bc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<stream_to_msg_bc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of stream_to_msg::stream_to_msg_bc.
       *
       * To avoid accidental use of raw pointers, stream_to_msg::stream_to_msg_bc's
       * constructor is in a private implementation
       * class. stream_to_msg::stream_to_msg_bc::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace stream_to_msg
} // namespace gr

#endif /* INCLUDED_STREAM_TO_MSG_STREAM_TO_MSG_BC_H */

