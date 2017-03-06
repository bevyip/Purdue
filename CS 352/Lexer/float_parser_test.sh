#!/bin/bash

NEGATIVE_TESTS=0
TOTAL_NEGATIVE=0
POSITIVE_TESTS=0
TOTAL_POSITIVE=0
test_output=""

function test {
	if [ "$2" = "" ]
		then
			((TOTAL_NEGATIVE++))
			x="$(timeout 1 java FloatParser "$1" 2> /dev/null)"
			exit_status=$?
			if [ "$exit_status" -eq 124 ]
				then
					echo -e "\033[1;31m$1 timed out\033[0m"
			elif [ "$exit_status" -ne 0 ]
				then
					echo -e "\033[1;32m$1 passed\033[0m"
					((NEGATIVE_TESTS++))
			else 
				val=$(echo "$x" | grep -e "Value=")
				echo -e "\033[1;31m$1 failed\033[0m, \texpected failure, got $val"
					fi
	else 
		((TOTAL_POSITIVE++))
			x="$(timeout 1 java FloatParser $1 2> /dev/null)"
			exit_status=$?
			x=$(echo "$x" | grep -e "Value=")
			if [ "$exit_status" -eq 124 ]
				then
					echo -e "\033[1;31m$1 timed out\033[0m"
			elif [ "$x" = "Value=$2" ]
				then
					 echo -e "\033[1;32m$1 passed\033[0m"
					((POSITIVE_TESTS++))
			else
				echo -e "\033[1;31m$1 failed\033[0m, \texpected Value=$2, got $x"
					fi
					fi
}

echo -e "\n\033[1;33mTESTING POSITIVE TESTS\033[0m\n"

test 0			0.0
test 720		720.0
test -6			-6.0
test +.99		0.99
test .2             	0.2		
test 1.2            	1.2		
test -1.5           	-1.5	
test 3e4            	30000.0	
test 3E4            	30000.0	
test 1E-1           	0.1		
test 2e-90          	2.0E-90	
test 2e+20          	2.0E20	
test 9.9e9		9.9E9
test -1.2e45        	-1.2E45	
test .455e+20       	4.55E19	
test +.455e+20      	4.55E19	
test -.455e+20      	-4.55E19	
test -0.455E+20     	-4.55E19	
test 0.0455E+20     	4.55E18	
test -989.455e+20   	-9.89455E22	

echo -e "\n\033[1;33mTESTING NEGATIVE TESTS\033[0m\n"

#non-accepting states
test ""				
test E
test .				
test -				
test -.
test -6.
test 4.				
test +9e
test 8E
test 7e-
test 4.2E			
test 4.2e-			

#invalid state transitions
test +-0.3			
test --0.4			
test +E9
test ..4
test 0.0.8			
test e9
test eE20			
test 0.+6			
test 4.e2			
test 3+E6
test 5e--5			
test 5e+5+
test 5e+5.
test 6e+7e
test 6e+7e9
test 4e5.2			
test 0.7+7e-20			
test 9eE20			

total_points=$[$POSITIVE_TESTS+$NEGATIVE_TESTS]
total_tests=$[$TOTAL_POSITIVE+$TOTAL_NEGATIVE]
echo -e "Positive Tests: $POSITIVE_TESTS / $TOTAL_POSITIVE "
echo -e "Negative Tests: $NEGATIVE_TESTS / $TOTAL_NEGATIVE "
echo -e "Total: $total_points / $total_tests"
echo "$total_points / $total_tests"
