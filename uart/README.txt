- Run terminal
- Use command sudo socat PTY,link=/dev/ttyp0,raw,echo=0 PTY,link=/dev/ttyp1,raw,echo=0
- Open 2 new terminals
- Use command cd "/Users/opiad/Desktop/School Related/YR4/Term 1/DIGDACM/Project/Pipe" for both
- Use sudo ./uart_tx on the first to transmit and open port ttyp0
- Use sudo ./uart_rx on the second to receive and on porty ttyp1

NOTE: Change directory depending on where you stored the files. This will not work on a Windows machine. Additional dependencies had to be installed in order to open virtual ports on MacOS and M1.
