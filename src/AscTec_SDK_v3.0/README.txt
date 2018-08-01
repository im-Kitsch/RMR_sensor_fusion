For documentation visit http://wiki.asctec.de/

Files added to template (or changed):
- COBS.c/.h
- benchmark.c/.h
- protocol.c/.h (all protocol functions / structs)
- ringbuffer.c/.h
- sdk.c/.h (Mainloop)
- spi0.c/.h (Driver)
- system.c/.h (Initialization)
- timeStamp.c/.h

Additional Nodes:
- The 'makefile' is also changed from the template (You needed to change line 71 and following to add new c/h-files and use interrupts)
- If the import of this project is not working you can try the identical zipped project in 'data/archive/HLP_Protocol', which is not affected by gitignore.
- If the received message is not valid the whole 'Startbyte' is set to '0x00' (not just the first Bit of this Byte). If you want to do like that (just the first Bit) -> Change l. 237 in 'protocol.c' to " protocol_received.Cprotocol_s.startByte &= ~(1); " 
