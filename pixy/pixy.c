#pragma systemFile

#if !defined(PIXY_C_)
#define PIXY_C_

// Communication/miscellaneous parameters.
#define PIXY_ARRAY_SIZE    		30
#define PIXY_START_WORD    		0xaa55
#define PIXY_START_WORD_CC 		0xaa56
#define PIXY_START_WORDX   		0x55aa
#define PIXY_MAX_SIGNATURE 		7

// Pixy x-y position values.
#define PIXY_MIN_X         		0
#define PIXY_MAX_X         		319
#define PIXY_MIN_Y         		0
#define PIXY_MAX_Y         		199

// RC-servo values.
#define PIXY_RCS_MIN_POS   		0
#define PIXY_RCS_MAX_POS   		1000
#define PIXY_RCS_CENTER_POS		((PIXY_RCS_MAX_POS - PIXY_RCS_MIN_POS) / 2)

#include "../communication/uart.c"

typedef enum PixyBlockType {
	NORMAL_BLOCK,
	CC_BLOCK  // Color code block.
} PixyBlockType;

typedef struct {
	unsigned short signature;
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
	unsigned short angle;  // Angle is only available for color coded blocks.
} PixyBlock;

typedef struct {
	TUARTs port;
	bool skipStart;
	PixyBlockType blockType;
	unsigned short blockCount;
	PixyBlock blocks[PIXY_ARRAY_SIZE];
} Pixy;

/**
 * Initialize Pixy.
 *
 * @param 	this	Pointer to Pixy struct.
 * @param 	port	UART port the Pixy is plugged into.
 *
 * @return	Pointer to Pixy struct.
 */
Pixy *newPixy(Pixy *this, TUARTs port) {
	this->port = port;
	this->skipStart = false;
	this->blockCount = 0;

	return this;
}

/**
 * Initialize Pixy.
 *
 * @param 	this    	Pointer to Pixy struct.
 * @param 	port    	UART port the Pixy is plugged into.
 * @param 	baudRate	Baud rate the Pixy is set to use.
 *
 * @return	Pointer to Pixy struct.
 */
Pixy *newPixy(Pixy *this, TUARTs port, TBaudRate baudRate) {
	setBaudRate(port, baudRate);

	this->port = port;
	this->skipStart = false;
	this->blockCount = 0;

	return this;
}

/**
 * Get start word from Pixy.
 *
 * @param 	this	Pointer to Pixy struct.
 *
 * @return	Pointer to Pixy struct.
 */
bool getPixyStart(Pixy *this) {
	TUARTs port = this->port;
	unsigned short w, lastW = 0xffff;  // Some inconsequential initial value.

	while (true) {
		w = getNextWord(port);
		if (w == 0 && lastW == 0) {
			sleep(10);
			return false;  // No start code.
		} else if (w == PIXY_START_WORD && lastW == PIXY_START_WORD) {
			this->blockType = NORMAL_BLOCK;
			return true;  // Code found!
		} else if (w == PIXY_START_WORD_CC && lastW == PIXY_START_WORD_CC) {
			this->blockType = CC_BLOCK;  // Found color code block.
			return true;
		} else if (w == PIXY_START_WORDX) {  // We're out of sync (backwards)!
			writeDebugStream("Reorder.\n");
			getChar(port);  // Resync.
		}
		lastW = w;
	}
}

/**
 * Update block data for Pixy.
 *
 * @param 	this	Pointer to Pixy struct.
 *
 * @return	Pointer to Pixy struct.
 */
Pixy *update(Pixy *this) {
	TUARTs port = this->port;
	unsigned short w, blockCount = 0, checksum, sum;
	PixyBlock *block;

	if (!this->skipStart) {
		if (getPixyStart(this) == 0) {
			this->blockCount = 0;
			return this;
		}
	} else {
		this->skipStart = false;
	}
	while (blockCount < PIXY_ARRAY_SIZE) {
		checksum = getNextWord(port);
		if (checksum == PIXY_START_WORD) {
			// We've reached the beginning of the next frame.
			this->skipStart = true;
			this->blockType = NORMAL_BLOCK;
			this->blockCount = blockCount;
			break;
		} else if (checksum == PIXY_START_WORD_CC) {
			this->skipStart = true;
			this->blockType = CC_BLOCK;
			this->blockCount = blockCount;
			break;
		} else if (checksum == 0) {
			this->blockCount = blockCount;
			break;
		}
		block = &this->blocks[blockCount];

		w = getNextWord(port);
		sum = w;
		block->signature = w;

		w = getNextWord(port);
		sum += w;
		block->x = w;

		w = getNextWord(port);
		sum += w;
		block->y = w;

		w = getNextWord(port);
		sum += w;
		block->width = w;

		w = getNextWord(port);
		sum += w;
		block->height = w;

		// No angle for regular block.
		w = (this->blockType == NORMAL_BLOCK) ? 0 : getNextWord(port);
		sum += w;
		block->angle = w;

		// Check checksum.
		if (checksum == sum) {
			blockCount++;
		} else {
			writeDebugStream("Checksum error!\n");
		}
		w = getNextWord(port);
		if (w == PIXY_START_WORD) {
			this->blockType = NORMAL_BLOCK;
		} else if (w == PIXY_START_WORD_CC) {
			this->blockType = CC_BLOCK;
		} else {
			this->blockCount = blockCount;
			break;
		}
	}
	return this;
}

/**
 * Get number of blocks last recorded by Pixy via update().
 *
 * @param 	this	Pointer to Pixy struct.
 *
 * @return	Number of blocks.
 */
unsigned short getBlockCount(Pixy *this) {
	return this->blockCount;
}

/**
 * Set Pixy camera brightness.
 *
 * @param 	this      	Pointer to Pixy struct.
 * @param 	beightness	Brightness value.
 *
 * @return	Number of characters sent via sendChars().
 */
short setBrightness(Pixy *this, unsigned char brightness) {
	unsigned char outBuf[3];

	outBuf[0] = 0x00;
	outBuf[1] = 0xfe;
	outBuf[2] = brightness;

	return sendChars(this->port, outBuf, sizeof(outBuf) / sizeof(outBuf[0]));
}

/**
 * Set Pixy LED color.
 *
 * @param 	this	Pointer to Pixy struct.
 * @param 	r   	Red intensity.
 * @param 	g   	Green intensity.
 * @param 	b   	Blue intensity.
 *
 * @return	Number of characters sent via sendChars().
 */
short setLED(Pixy *this, unsigned char r, unsigned char g, unsigned char b) {
	unsigned char outBuf[5];

	outBuf[0] = 0x00;
	outBuf[1] = 0xfd;
	outBuf[2] = r;
	outBuf[3] = g;
	outBuf[4] = b;

	return sendChars(this->port, outBuf, sizeof(outBuf) / sizeof(outBuf[0]));
}

/**
 * Set pan/tilt servo positions.
 *
 * @param 	this	Pointer to Pixy struct.
 * @param 	s0  	Servo 0 (pan) position.
 * @param 	s1  	Servo 1 (tilt) position.
 *
 * @return	Number of characters sent via sendChars().
 */
short setServos(Pixy *this, unsigned short s0, unsigned short s1) {
	unsigned char outBuf[6];

	outBuf[0] = 0x00;
	outBuf[1] = 0xff;
	outBuf[2] = s0;
	outBuf[4] = s1;

	return sendChars(this->port, outBuf, sizeof(outBuf) / sizeof(outBuf[0]));
}

/**
 * Print Pixy block data to debug stream.
 *
 * @param 	this	Pointer to Pixy block struct.
 */
void print(PixyBlock *this) {
	unsigned short signature = this->signature;

	if (signature > PIXY_MAX_SIGNATURE) {  // Color code (CC)!
		writeDebugStream(
				"CC block! sig: %o (%d decimal) x: %d y: %d width: %d height: %d angle: %d\n",
				signature, signature, this->x, this->y, this->width, this->height, this->angle);
	} else {  // Regular block. Note, angle is always zero, so no need to print.
		writeDebugStream("sig: %d x: %d y: %d width: %d height: %d\n", signature,
				this->x, this->y, this->width, this->height);
	}
}

/**
 * Print Pixy block data to debug stream.
 *
 * @param 	this	Pointer to Pixy struct.
 */
void print(Pixy *this) {
	unsigned short blockCount = this->blockCount;

	writeDebugStream("Detected %d:\n", blockCount);

	for (unsigned short i = 0; i < blockCount; i++) {
		writeDebugStream("  block %d: ", i);
		print(&this->blocks[i]);
	}
}

#endif
