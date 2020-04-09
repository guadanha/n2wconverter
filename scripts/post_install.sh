#!/bin/bash

echo "Configuring n2wconverter service ..."
cp $1/scripts/n2wconverter.service /etc/systemd/system/
systemctl daemon-reload
systemctl enable n2wconverter.service
