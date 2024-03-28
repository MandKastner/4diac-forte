#!/usr/bin/env bash

../bin/tb/src/forte -c localhost:61499 &
../bin/tb/src/forte -c localhost:61500 &
../bin/tb/src/forte -c localhost:61501

wait

echo "Run finished!"