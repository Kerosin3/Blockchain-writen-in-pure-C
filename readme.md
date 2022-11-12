# BLOCKCHAIN WRITTEN IN C LANGUAGE DRAFT PROJECT

## Overview

This is a draft project that was created in order to provide practically-based experience with 
block-chain technology.
The created blockchain app utilizes fast I/O by means of Linux specific system call io_uring.
Blacke2b cryptographic hashing function is also being used throughout the project due to its huge 
hashing speed (about 1 Gb/sec).


## Motivation
The blockchain technology is a revolutionary technology and sooner or later it is going to take a huge place it our lives.
I have read several books about blockchain and got an idea. However, raw knowledge is not enough for a deep understanding. 
Thus, main motivation to me was to to get in close touch with blockchain technology by writing a piece of this complex and autonomous system only by myself.

## Blockchain features
* Proof of Work based blockchain
* Fast and efficient hashing (blake2b cryptohashing)
* Vulnerability to Birthday hash attack probability reduced
* block consist of 512 asymmetrically signed messages
* Classical Merkle tree block hash generation
* Embedded fast message verification for light nodes (Merkle message traverse) (this kind of nodes not yet been implemented)

## Used technologies
* Blockchain
  * Cassical POW with black2b hash function
* fast asyncronius Linux-based I/O
  * [io_uring]( https://unixism.net/loti/index.html )
* fast and modern crypto library (assymetric cryptography, hashing)
  * libsodium
* easy-to-use and platform-neutral data ser/des
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
## Usage (How to install)

1. compile with CMAKE -DWRITE_LOG=1 FLAG
2. make sure ports 2220,2221,2222 are free on your host machine
3. fix Vagrantfile in "vagrant_testing" folder (lines 6,18,29) according to name of used archlinux box (download and install last arch for vagrant via **vagrant box add archlinux/archlinux** ) watch the box name here /home/{USERNAME}/.vagrant.d/boxes and write in to Vagrantfile
4. use vagrant up in "vagrant_testing" folder to up virtual machines
5. use vagrant ssh testmachine to login into server, then in home dir launch app.out
6. use vagrant ssh client1 and vagrant ssh client2 to log into client machines, run **./client.app 172.16.1.3** for client1  and **./client_app 172.16.1.2** for client2 in home dir to start client application 
7. (in need) run "ansible-playbook --limit %testmachine/client1/client2% load_soft.yaml" (choose one machine per update) to update recompiled soft on machines

## test stand
![](https://github.com/Kerosin3/C_blockchain-draft/blob/main_experimental/docs/pictures/blockchain.jpg)


