#!/usr/bin/env bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <n>"
    exit 1
fi

n=$1

for ((i=0; i<=n-1; i++)); do
    port=$((61499 + i))           # Add 61499 to loop index
    arg="localhost:$port"         # Prepend localhost:
    echo "Starting forte instance $i using port $port"
    ../bin/tb/src/forte -c "$arg" &
done

wait
echo "All $n instances finished."
