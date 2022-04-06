#!/usr/bin/env bash

set -e

tarball_name="drake-20220328-$(lsb_release -cs).tar.gz"
wget https://github.com/RobotLocomotion/drake/releases/download/v1.1.0/${tarball_name}
mkdir .drake && tar -xvzf ${tarball_name} -C ./.drake && rm ${tarball_name}
