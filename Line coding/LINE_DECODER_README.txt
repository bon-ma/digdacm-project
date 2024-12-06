INSTRUCTIONS FOR LINE DECODER:


1. Open command prompt as admin
2. Change your directory again to where the .exe and code is placed
3. Run this command:
gcc -o Line_decoder.exe Line_decoder.cpp

4. After compiling the program use the result from the line coding program as the pipe input, for this case example:
echo A0 E0 BO BO BO 98 B0 C0 B0 B0 B0 90 BO DO BO 88 AO C0 C8 A0 | Line_decoder.exe

[Again check the sample image from line coding on which to copy]


5. The output or the decoded output should be now seen at Read buffer: (For the case of echo A0 E0 BO BO BO 98 B0 C0 B0 B0 B0 90 BO DO BO 88 AO C0 C8 A0)

it should be:

No input file provided. Reading from stdin.
Read Buffer: 22 41 30 20 45 30 20 42 4F 20 42 4F 20 42 4F 20 39 38 20 42 30 20 43 30 20 42 30 20 42 30 20 42 30 20 39 30 20 42 4F 20 44 4F 20 42 4F 20 38 38 20 41 4F 20 43 30 20 43 38 20 41 30 22 20 0A
Invalid frame: missing delimiters.

