# BMS-MONITOR

This library contains all the functionality needed to communicate with a series
of BMS monitor Integrated Circuits.

The purpose of this library is to implement the drivers of each supported IC and
to abstract them using a common interface to simplify development and maintainance
of different BMS.

> [!WARNING]
> This library is still a work in progress as for now it only supports the LTC6811-1 (and not the LTC6811-2)
> and it also does not implement the abstract interface yet.

## Supported ICs

- [LTC6811-1](/docs/bms-monitor/assets/ltc6811-1-6811-2.pdf)

## General architecture

For each IC its **driver** is implemented independently from the others by
providing one or more of the following functionalities:
1. **Encoding** functions: that encodes information into bytes which can be sent to the ICs
2. **Decoding** functions: that decodes the bytes received from the ICs into useful information
