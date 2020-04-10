# N2WCONVERTER
Convert number to word.

## Dependencies
sudo apt install cmake git

## Build

    mkdir build

    cd build

    cmake ../

    make

    make install

## Configuration

The post install script does the hard work

```
#!/bin/bash

echo "Configuring n2wconverter service ..."
cp $1/scripts/n2wconverter.service /etc/systemd/system/
systemctl daemon-reload
systemctl enable n2wconverter.service
```

## Start Service

    sudo systemctl start n2wconverter.service
