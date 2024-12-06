- Run terminal
- Use command sudo socat PTY,link=/dev/ttyp0,raw,echo=0 PTY,link=/dev/ttyp1,raw,echo=0
- Open 2 new terminals
- Use command cd "/Users/opiad/Desktop/School Related/YR4/Term 1/DIGDACM/Project/uart" for both
- Use cat tx_linecoding_output.txt | sudo ./uart_tx on the first to transmit and open port ttyp0
- Use sudo ./uart_rx on the second to receive and on porty ttyp1

To test if it's working, use iTerm:
- Use sudo cat /dev/ttyp1 to open port ttyp1 on one iTerm window to see if it matches ./uart_rx terminal
- Use sudo tee /dev/ttyp0 on another iTerm window to transmit to see if it reflects.

NOTE: Transmission message should be the same across all receivers.