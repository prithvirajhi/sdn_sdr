/* -*- c++ -*- */

#define STREAM_TO_MSG_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "stream_to_msg_swig_doc.i"

%{
#include "stream_to_msg/stream_to_msg_bc.h"
%}


%include "stream_to_msg/stream_to_msg_bc.h"
GR_SWIG_BLOCK_MAGIC2(stream_to_msg, stream_to_msg_bc);
