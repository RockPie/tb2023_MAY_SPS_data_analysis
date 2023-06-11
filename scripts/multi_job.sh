# ./build/mixed_2d_reconstruct_main -n <value>
# loop over 1 to 10

#!/bin/bash

# Define the number of parallel processes
max_processes=8

# Function to execute the command
execute_command() {
    ../build/mixed_2d_reconstruct_main -n $1 -t 8 -e 80000
}

# Export the function so it can be called by xargs
export -f execute_command

# Run the command in parallel using xargs
seq 1 $max_processes | xargs -P $max_processes -I {} bash -c 'execute_command "$@"' _ {}
