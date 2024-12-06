INSTRUCTIONS ON HOW TO RUN THE LINE_CODING.CPP

Running the command:


1. Compile the program
2. Exit the program
3. Go to command prompt and run as administrator
4. Type the following commands

cd "FILEDIRECTORY"
gcc -o Line_coding.exe Line_coding.cpp

If you want to read from a file use: Line_coding.exe nameofthettextfile.txt
If you want to echo/pipe the string: echo String | Line_coding.exe



Example commands:

cd C:\Users\Anton\Documents\DIGDACS\s
gcc -o Line_coding.exe Line_coding.cpp
echo DIGDACM | Line_coding.exe
*Check the Line_coding_sample.png to see the sample output

*IMPORTANT!

Take note of the result, you will use it for the line decoder 

Example:
Manchester-Encoded Frame: A0 E0 B0 B0 B0 98 B0 C0 B0 B0 B0 90 B0 

(You need to copy: A0 E0 B0 B0 B0 98 B0 C0 B0 B0 B0 90 B0)
