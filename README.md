# N2WCONVERTER
Convert number to word.

## Dependencies
Install depencies

    sudo apt install cmake git libgtest-dev build-essential rsyslog

## Build
Server build and install

    mkdir build

    cd build

    cmake ../

    make

    sudo make install

## Configuration
The post install script does the hard work

## Start Service
Service running automatically after restart.
If you want to start the service, run:

    sudo systemctl start n2wconverter.service

To stop the service, run:

    sudo systemctl stop n2wconverter.service

## Unit Test

    cd build/tests

    ./unit-test


## Server Logs
The server logs can be viewed at:

    /opt/certi/var/log/

The logs will be managed and rotated by syslog

## Test n2wconverter with a docker image
Get docker image

    docker pull guadanha/n2wconverter:v1_0

Run the image

    docker run -it --rm guadanha/n2wconverter:v1_0

Build server

    mkdir /workspace/n2wconverter/build

    mkdir -p /opt/certi/var/log/

    cd /workspace/n2wconverter/build

    cmake ../

    make

Get machine ip

    ip -4 a

Run n2wconverter

    ./n2wconverter -f ../n2wconverter.conf &

For portuguese type in a browser (change ip to your docker ip):

    "http://127.0.0.1:30000/100"

For english type in a browser (change ip to your docker ip):

    "http://127.0.0.1:30000/en/100"

