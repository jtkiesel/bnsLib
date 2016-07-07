#pragma systemFile

#if !defined(UART_C_)
#define UART_C_

/**
 * Get next character from UART port.
 *
 * @param 	port	UART port to get from.
 *
 * @return	Next character.
 */
unsigned char getNextChar(TUARTs port) {
	short c = getChar(port);

	// Wait for valid character.
	while (c == -1) {
		sleep(3);

		c = getChar(port);
	}
	return (unsigned char)c;
}

/**
 * Get next word from UART port.
 *
 * @param 	port	UART port to get from.
 *
 * @return	Next word from UART port.
 */
unsigned short getNextWord(TUARTs port) {
	// This routine assumes little-endian.
	unsigned char c = getNextChar(port);
	unsigned short w = getNextChar(port);
	w <<= 8;
	w |= c;
	return w;
}

/**
 * Send character array to UART port.
 *
 * @param 	port	Port to send to.
 * @param 	data	Character array to send.
 * @param 	len 	Array length.
 *
 * @return	Array length.
 */
short sendChars(TUARTs port, unsigned char *data, short len) {
	for (short i = 0; i < len; i++) {
		// Wait for transmit buffer to be empty or timeout.
		short timeout = 0;
		while (!bXmitComplete(port) && timeout++ < 20) {
		}
		sendChar(port, data[i]);
	}
	return len;
}

#endif  // UART_C_
