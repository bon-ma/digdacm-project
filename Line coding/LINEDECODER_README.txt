INSTRUCTIONS FOR LINE DECODER:


1. Open command prompt as admin
2. Change your directory again to where the .exe and code is placed
3. Run this command:
gcc -o Line_decoder.exe Line_decoder.cpp

4. After compiling the program use the result from the line coding program as the pipe input, for this case example:
echo -05 05 05 -05 | Line_decoder.exe

[Again check the sample image from line coding on which to copy]


5. The output or the decoded output should be now seen at Read buffer: (For the case of echo -05 05 05 -05)

it should be:

No input file provided. Reading from stdin.
10 which is the same as the input from earlier

