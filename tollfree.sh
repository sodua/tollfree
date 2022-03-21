#!/usr/bin/env bash

MAXCOUNT=5
RANGE=10
count=1
calledfile=/home/cth/.tollfree/800.called
scanfile=/home/cth/.tollfree/800.scan

# Exchange validity: Allowed ranges: [2-9] for the first digit, and [0-9] for both the
# second and third digits (however, in geographic area codes the third digit of the
# exchange cannot be 1 if the second digit is also 1).


digit=$(shuf -i 2-9 -n 1)
number="$number$digit"
digit=$(shuf -i 2-9 -n 1)
number="$number$digit"

while [ "$count" -le $MAXCOUNT ]
do
    digit=$RANDOM
    let "digit %= $RANGE"
    number="$number$digit"
    let "count += 1"
done

if [ -f "$calledfile" ]
then
    if ! grep -q "$number" $calledfile; then
        echo "$number" >> "$calledfile"
    else 
        echo "You hit a bingo! 800 number already called!" && exit 1 
    fi
fi

number="800-$(sed 's/./&-/3' <<< $number)"
echo "$number"
echo -n "Comments: "
read comments
if [ -f "$scanfile" ]
then
    echo "$number $comments" >> "$scanfile"
fi