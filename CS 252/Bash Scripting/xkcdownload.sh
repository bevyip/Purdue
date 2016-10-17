#!/bin/bash

if [ $1 ] && [ $2 ]; then
	echo "Correct number of arguments"
else
	echo "Usage: $0 start_range end_range"
	exit
fi

if [ $(($2 - $1)) -gt 20 ]; then
	echo "Range, i.e. end_range - start_range, cannot be greater than 20"
	exit
fi

if [ $2 -lt $1 ]; then
	echo "start_range cannot be after end_range"
	exit
fi

#Checking done. Get started on the real stuff.

#Step 1: Start a loop from the start range to the end range

start_r=$1
end_r=$2

start_range=$1
end_range=$2
let end_range=$end_range+1

while [ $start_range -ne $end_range ]; do
	wget -O - "http://xkcd.com/$start_range" > output.html
	img_url=$(egrep -o 'http://imgs.xkcd.com/comics/\w+.*' output.html)
	echo $img_url
	wget $img_url -P "./xkcd_comics/"

	let start_range=$start_range+1
	echo "Start range is : $start_range"
done

#Remove the output.html file as there is no use for it now
rm -rf output.html

#Check if the folder for the imageshas been created or not
if [ -d "xkcd_comics" ]; then
	echo "Director exists"
else
	echo "Directory doesn't exist"
	exit
fi

#If the program reaches here, you can make it into a tar because the directory exists
tar -zcvf xkcd_comics.tar xkcd_comics/

#Remove the extra files
rm -rf xkcd_comics/

#Check if there is a provided email
if [ $4 ]; then
	email=$4
else
	email=$USER
fi

#Check if the mail tag is on
if [ $3 ]; then
	if [ "$3" == "-m" ]; then
		echo "The images are attached" | mutt -a "xkcd_comics.tar" -s "XKCDownload images from $start_r to $end_r" -- $email
	fi
fi
