#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/prithvirajhi/sdn_sdr/gr-stream_to_msg/python
export GR_CONF_CONTROLPORT_ON=False
export PATH=/home/prithvirajhi/sdn_sdr/gr-stream_to_msg/build/python:$PATH
export LD_LIBRARY_PATH=/home/prithvirajhi/sdn_sdr/gr-stream_to_msg/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/prithvirajhi/sdn_sdr/gr-stream_to_msg/build/swig:$PYTHONPATH
/usr/bin/python2 /home/prithvirajhi/sdn_sdr/gr-stream_to_msg/python/qa_stream_to_msg_bc.py 
