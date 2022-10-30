# BLOCKCHAIN WRITTEN IN C LANGUAGE DRAFT PROJECT

## Overview

This is a draft project that was created in order to provede practic-based experience with 
block-chain technology.
The created blockain utilezes fast I/O by means Linux specific system call io_uring.
Blacke2b cryptographic hashing function is being used throghout the project due to its huge 
hashing speed (about 1 Gb/sec).


## Motivation

The main motivation about this project is to get in touch with block chain technology and usege of modern C language means of development.

## Used technologies

* fast asyncronius Linux-based I/O
  * io_uring
* fast and modern crypto library 
  * libsodium
* easy-to-use and platforn-neutral data ser/des
  * protobuf-c
* fast and thread-safe logging system
  * zlog
* self-documentation
  * Doxygen
* Cross-platform build-automation system
  * CMake
* virtualization 
  * Vagrant
* IT automation system
  * Ansible



