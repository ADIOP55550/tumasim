#!/bin/bash

# Ensure that exactly two arguments are provided
if [ "$#" -ne 2 ]; then
  echo "Usage: $0 code_file test_file"
  exit 1
fi

code_file="$1"
test_file="$2"

# Check if the code file exists
if [ ! -f "$code_file" ]; then
  echo "Code file '$code_file' does not exist."
  exit 1
fi

# Check if the test file exists
if [ ! -f "$test_file" ]; then
  echo "Test file '$test_file' does not exist."
  exit 1
fi

# Read the test file line by line
while IFS= read -r test_line; do
  # Run the command and capture the output
  output=$(./tumasim "$code_file" "$test_line")
  
  # Print the test line
  echo "$test_line"
  
  # Print the last two lines of the output
  echo "$output" | tail -n 2
done < "$test_file"

