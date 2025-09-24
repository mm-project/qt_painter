#!/usr/bin/env bash

check_mismatches() {
    local mismatch_file="$1"
    local all="0"

    base_dir=$(dirname $mismatch_file)
    output_dir="$base_dir/output"
    while IFS= read -r line; do
        if [[ "$line" =~ ^MISMATCH: ]]; then
            # Extract left and right filenames
            left=$(echo "$line" | awk '{print $2}')
            right=$(echo "$line" | awk '{print $4}')
            # checksum of their diff
            d=$(diff -u "$output_dir/$right" "$output_dir/$left" | sha256sum | cut -c1-8)
            all+="$d"
        fi
    done < "$mismatch_file"
    total=$(echo -n "$all" | sha256sum | cut -c1-8)
    echo $total
}

filename_str=$1
filename=$2

file_hash=$(sha256sum $filename | cut -c1-8)
mismatches_hash=$(check_mismatches "$filename")
echo "$file_hash:$mismatches_hash"