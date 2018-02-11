#!/usr/bin/bash
EXC='/Users/kirillkorolev/Library/Developer/Xcode/DerivedData/internship-awijnijjnpysvmdpivxltpyejtok/Build/Products/Debug/internship'
FILENAME='logs.log'
(time $EXC > /dev/null) 2>> $FILENAME
