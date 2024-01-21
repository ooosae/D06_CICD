#!/bin/bash

file_stream_in="${1}"
file_stream_out_expect="${2}"
file_app_arg=""

if [ $# -gt 2 ]; then
    file_app_arg=$(cat ${3})
fi

if [[ -n $USE_VALGRIND ]]; then
    if ! valgrind --log-file="./out/valgrind_out.txt" ./s21_grep "$file_app_arg"<"$file_stream_in">out.txt; then
        exit 1
    fi
else
    if ! ./s21_grep $file_app_arg<"$file_stream_in">out.txt; then
        exit 1
    fi
fi

bash ./func_tests/scripts/comparator.sh "$file_stream_out_expect" out.txt

exit $?


