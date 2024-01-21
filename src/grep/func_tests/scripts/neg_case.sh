#!/bin/bash

file_stream_in="${1}"
file_app_arg=""

if [ $# -gt 1 ]; then
    file_app_arg=$(cat ${2})
fi

if  [[ -n $USE_VALGRIND ]]; then
    if ! valgrind --log-file="./out/valgrind_out.txt" ./s21_grep "$file_app_arg"<"$file_stream_in">out.txt; then
        exit 0
    fi
fi
    if ! ./s21_grep $file_app_arg<"$file_stream_in">out.txt; then
        exit 0
    fi

exit 1


