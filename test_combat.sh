#!/bin/bash
cd /workspaces/Helicopter-Combat/build

# Test sequence: Enter free flight mode, check tactical advice, return to menu, exit
echo "Testing tactical combat features..."
echo "2
6
0
0" | ./HelicopterCombat
