#!/bin/bash

# Define the two files
file1="Pi-1000000.txt"
file2="pi.txt"

# Use cmp to find the byte position of the first difference
diff_pos=$(cmp -l $file1 $file2 | awk 'NR==1{print $1}')

if [[ -n $diff_pos ]]; then
    # Decrement diff_pos by 5 to start displaying characters 5 positions before the difference
    let "start_pos=$diff_pos-5 > 0 ? $diff_pos-5 : 0"

    # Display ten characters from each file starting from start_pos
    echo "Difference found at position: $diff_pos"
    echo "Displaying ten characters from each file starting five chars before the difference..."
    
    dd if=$file1 bs=1 count=10 skip=$start_pos 2>/dev/null | od -An -c
    dd if=$file2 bs=1 count=10 skip=$start_pos 2>/dev/null | od -An -c
    
else 
    echo "No differences found."
fi

