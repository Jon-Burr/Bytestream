==========
Bytestream
==========

Low-level code to read and interpret data from bytestreams

.. contents::
    
--------
Overview
--------

This package is designed to help convert the data returned by detector bytestreams to formats native to a normal computer.
There are two chief (connected) differences between how computers treat memory layout and how detector bytestreams do:

- Most modern CPUs store data in little-endian format, while bytestreams and other network communication protocols use big-endian
- CPUs store data from left-to-right, i.e. the lowest indexed byte is in lowest valued memory location. In bytestreams it is the reverse.
  This can be seen clearly e.g. from CAEN TDC manuals where the data layout is enumerate from 0-N, right to left.