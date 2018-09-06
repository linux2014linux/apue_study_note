#!/bin/bash
rm -rf p_root mychmod p_root_na
g++ p_root.cpp -o p_root
sudo chown root p_root
sudo chgrp root p_root
g++ mychmod.cpp -o mychmod
sudo ./mychmod ./p_root
cp p_root p_root_na
sudo chown root p_root_na
sudo chgrp root p_root_na
