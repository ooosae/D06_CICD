#!/bin/bash

c=1
err=0
t=""
curr="./func_tests/data/pos_${t}${c}"
if ! [ -f "${curr}_in.txt" ]; then
    t="0";
fi
curr="./func_tests/data/pos_${t}${c}"

echo

while [ -f "${curr}_in.txt"  ]; do
    if [ -f "${curr}_args.txt" ]; then
        if bash ./func_tests/scripts/pos_case.sh "${curr}_in.txt" "${curr}_out.txt" "${curr}_args.txt"; then
            echo -e "POS Test №${c} : \e[32mSUCCESSED\e[0m"
        else
            echo -e "POS Test №${c} : \e[31mFAILED\e[0m"
            err=$((err+1))
        fi   
    else
        if bash ./func_tests/scripts/pos_case.sh "${curr}_in.txt" "${curr}_out.txt"; then
            echo -e "POS Test №${c} : \e[32mSUCCESSED\e[0m"
        else
            echo -e "POS Test №${c} : \e[31mFAILED\e[0m"
            err=$((err+1))
        fi 
    fi    
    c=$((c+1))
    if [ $c -gt 9 ]; then
        t=""    	
   	fi
   	
    curr="./func_tests/data/pos_${t}${c}"    
done

echo

c=1
curr="./func_tests/data/neg_${t}${c}"
if ! [ -f "${curr}_in.txt" ]; then
    t="0";
fi
curr="./func_tests/data/neg_${t}${c}"

while [ -f "${curr}_in.txt"  ]; do
    if [ -f "${curr}_args.txt" ]; then
        if bash ./func_tests/scripts/neg_case.sh "${curr}_in.txt" "${curr}_args.txt"; then
            echo -e "NEG Test №${c} : \e[32mSUCCESSED\e[0m"
        else
            echo -e "NEG Test №${c} : \e[31mFAILED\e[0m"
            err=$((err+1))
        fi
    else
        if bash ./func_tests/scripts/neg_case.sh "${curr}_in.txt"; then
            echo -e "NEG Test №${c} : \e[32mSUCCESSED\e[0m"
        else
            echo -e "NEG Test №${c} : \e[31mFAILED\e[0m"
            err=$((err+1))
        fi
    fi
    
    c=$((c+1))
    if [ $c -gt 9 ]; then
        t=""    	
   	fi
   	
    curr="./func_tests/data/neg_${t}${c}" 
done

rm -f out.txt valgrind_out.txt

exit $err

