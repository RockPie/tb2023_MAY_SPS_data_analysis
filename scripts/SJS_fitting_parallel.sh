# ./build/SJS_fitting_general_main -n <value> -t <value> -e <value>

# Define the number of parallel processes
max_processes=10
event_total=100000

# Function to execute the command
execute_command() {
    ../build/SJS_fitting_general_main -n $1 -t $2 -e $3
}

# Export the function so it can be called by xargs
export -f execute_command

# Run the command in parallel using xargs
seq 1 $max_processes | xargs -n 1 -P $max_processes -I {} bash -c "execute_command {} $max_processes $event_total"
