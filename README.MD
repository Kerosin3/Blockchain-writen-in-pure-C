# BLOCKCHAIN WRITTEN IN C LANGUAGE DRAFT PROJECT

## Overview

This is a draft project that was created in order to provide practically-based experience with 
block-chain technology.
The created blockchain app utilizes fast I/O by means of Linux specific system call io_uring.
Blacke2b cryptographic hashing function is also being used throghout the project due to its huge 
hashing speed (about 1 Gb/sec).


## Motivation
The blockchain technology is a revolutionary technology and sooner or later it is going to take a huge place it our lives.
I have readed several books about blockain and got an idea. Hovewer, raw knowledge is not enough for a deep understanding. 
Thus, main motivation to me was to to get in close touch with blockchain technology by writing a piece of this complex and automonus system only by myself.

## Used technologies

* fast asyncronius Linux-based I/O
  * [io_uring]( https://unixism.net/loti/index.html )
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
* Virtualization (for P2P testing)
  * Vagrant
* IT automation system 
  * Ansible



