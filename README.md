# N2WCONVERTER
Convert number to word.

## Dependencies
sudo apt install cmake git libgtest-dev build-essential rsyslog

## Build

    mkdir build

    cd build

    cmake ../

    make

    make install

## Configuration

The post install script does the hard work

## Start Service

    sudo systemctl start n2wconverter.service

## Unit Test

    cd build/tests

    ./unit-test

## Test n2wconverter
Run the server
    sudo systemctl stop n2wconverter.service

    cd build

    ./n2wconverter -f ../n2wconverter.conf &

For portuguese type in a browser:
    "http://127.0.0.1:30000/100"

For english type in a browser:
    "http://127.0.0.1:30000/en/100"

## Server Logs
The server logs can be viewed at:
    /opt/certi/var/log/

The logs will be managed and rotated by syslog
