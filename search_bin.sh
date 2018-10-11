#!/bin/bash
find $1 -type f -executable -exec file -i '{}' \; | grep -v 'git' | grep 'x-executable; charset=binary'
# | awk -F: '{print $1}' | xargs rm
