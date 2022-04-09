#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
	switch (ALUControl) {
		case 0:
			/* ALU Control: 000
			 * Addition: Z = A + B */
			*ALUresult = A + B;
			break;
		case 1:
			/* ALU Control: 001
			 * Subtraction: Z = A - B */
			*ALUresult = A - B;
			break;
		case 2:
			/* ALU Control: 010
			 * Comparison: Z = 1 if A < B otherwise Z = 0 */
			*ALUresult = (A < B) ? 1 : 0;
			break;
		case 3:
			/* ALU Control: 011
			 * Comparison: Z = 1 if A < B otherwise Z = 0 (Unsigned) */
			*ALUresult = ((signed)A < (signed)B) ? 1 : 0;
			break;
		case 4:
			/* ALU Control: 100
			 * AND: Z = A & B */
			*ALUresult = A & B;
			break;
		case 5:
			/* ALU Control: 101
			 * OR: Z = A | B */
			*ALUresult = A | B;
			break;
		case 6:
			/* ALU Control: 110
			 * SHIFT: Z = B << 16 */
			*ALUresult = B << 16;
			break;
		case 7:
			/* ALU Control: 111
			 * Z = ~A */
			*ALUresult = ~A;
			break;
	}

	// Set Zero to true (1) if *ALUresult is zero. Otherwise, it is false.
	*Zero = (*ALUresult == 0) ? 1 : 0;
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	/* Ensure that the instruction is word-aligned. As the instructions state,
	 * it must have an address that is a multiple of 4. Halt if it is not.*/
	if (!(PC % 4)) return 1;

	/* Get and set the instruction from the Memory array.
	 * As explained in the FAQ doc, the decimal form of the
	 * instruction is at the index of PC right shifted by 2 bits
	 * or divided by 2^2. */
	*instruction = Mem[(PC >> 2)];
	return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	/* As demonstrated in line 41 of spimcore.c The data is stored at the following positions,
	 * where 0 is the rightmost bit and 31 the leftmost, and has the following constraints:
	 * | Part    | Bits    | # of Bits | Max decimal value     |
	 * |---------+---------+-----------+-----------------------|
	 * | op      | 31 - 26 |  6        |  (2^6) - 1 =       63 |
	 * | r1      | 25 - 21 |  5        |  (2^5) - 1 =       31 |
	 * | r2      | 20 - 16 |  5        |  (2^5) - 1 =       31 |
	 * | r3      | 15 - 11 |  5        |  (2^5) - 1 =       31 |
	 * | funct   |  5 - 0  |  6        |  (2^6) - 1 =       63 |
	 * | offset  | 15 - 0  | 16        | (2^16) - 1 =    65535 |
	 * | jsec    | 25 - 0  | 26        | (2^26) - 1 = 67108863 |
	 * |-------------------------------------------------------|
	 * To find the appropriate values from the data listed in the above table, the value of
	 * instruction is right shifted by the rightmost bit index in order to set the appropriate
	 * value to the 0th bit position. Then to exclude any extra bits to the left, a bitwise
	 * AND is performed with the maximum decimal value that can be stored in that size.
	 */
	*op = (instruction >> 26) & 63;
	*r1 = instruction >> 21 & 31;
	*r2 = instruction >> 16 & 31;
	*r3 = instruction >> 11 & 31;
	*funct = instruction & 63;
	*offset = instruction & 65535;
	*jsec = instruction & 67108863;
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
	switch(op) {
		// R-type instruction
		case 0:
			controls->RegDst = 1;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 7;
			controls->MemWrite = 0;
			controls->ALUSrc = 0;
			controls->RegWrite = 1;
			break;

		// Jump
		case 2:
			controls->RegDst = 0;
			controls->Jump = 1;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 0;
			controls->MemWrite = 0;
			controls->ALUSrc = 0;
			controls->RegWrite = 0;
			break;

		// Branch eq
		case 4:
			controls->RegDst = 2;
			controls->Jump = 0;
			controls->Branch = 1;
			controls->MemRead = 0;
			controls->MemtoReg = 2;
			controls->ALUOp = 1;
			controls->MemWrite = 0;
			controls->ALUSrc = 0;
			controls->RegWrite = 0;
			break;

		// Add immediate
		case 8:
			controls->RegDst = 0;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 0;
			controls->MemWrite = 0;
			controls->ALUSrc = 1;
			controls->RegWrite = 1;
			break;

		// Set less than imm
		case 10:
			controls->RegDst = 1;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 2;
			controls->MemWrite = 0;
			controls->ALUSrc = 0;
			controls->RegWrite = 1;
			break;

		// Set less than imm unsigned
		case 11:
			controls->RegDst = 1;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 3;
			controls->MemWrite = 0;
			controls->ALUSrc = 0;
			controls->RegWrite = 1;
			break;

		// Load upper imm
		case 15:
			controls->RegDst = 0;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 0;
			controls->ALUOp = 6;
			controls->MemWrite = 0;
			controls->ALUSrc = 1;
			controls->RegWrite = 1;
			break;

		// Load word
		case 35:
			controls->RegDst = 0;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 1;
			controls->MemtoReg = 1;
			controls->ALUOp = 0;
			controls->MemWrite = 0;
			controls->ALUSrc = 1;
			controls->RegWrite = 1;
			break;

		// Store word
		case 43:
			controls->RegDst = 2;
			controls->Jump = 0;
			controls->Branch = 0;
			controls->MemRead = 0;
			controls->MemtoReg = 2;
			controls->ALUOp = 0;
			controls->MemWrite = 1;
			controls->ALUSrc = 1;
			controls->RegWrite = 0;
			break;

		// If invalid value for op, halt.
		default:
			return 1;
	}
	return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	// The value of rNum, where Num is a number, points to the index in reg of the data.
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	// Sign is the 16th bit
	int sign = offset >> 15;

	/* If sign is 1, value is negative. Left pad the extended_value with 1.
	 * The value 4294901760 is binary value where 16 leftmost bits are 1
	 * and 16 rightmost bits are 0. */
	if (sign) *extended_value = offset | 4294901760;

	/* If sign is 0, value is positive. Left pad the extended_value with 0.
	 * The value 65535 is binary value where 16 leftmost bits are 0 and 16
	 * rightmost bits are 1. */
	else *extended_value = offset & 65535;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	// Halt if invalid APU operation
	if (ALUOp < 0 || ALUOp > 7) return 1;

	// Determine what we have to do when we have an R-type operation using the value of funct.
	else if (ALUOp == 7) {
		/* Modify the value of ALUOp based on the value of funct. This is based on the values
		 * found in the table in the Module 5ppt on slide 108. */
		switch (funct) {
			case 4:
				//Shift left by 16 bits
				ALUOp = 6;
				break;

			case 32:
				// Addition
				ALUOp = 0;
				break;

			case 34:
				// Subtraction
				ALUOp = 1;
				break;

			case 36:
				// AND
				ALUOp = 4;
				break;

			case 37:
				// OR
				ALUOp = 5;
				break;

			case 39:
				// NOT
				ALUOp = 7;
				break;

			case 42:
				// Less than
				ALUOp = 2;
				break;

			case 43:
				// Less than unsigned
				ALUOp = 3;
				break;

			// Halt if funct has an invalid value
			default:
				return 1;
		}

		// If ALUSrc is true, second value is extended_value instead of data2.
		if (ALUSrc) ALU(data1, extended_value, ALUOp, ALUresult, Zero);

		// Perform ALU with data2 as second value.
		else ALU(data1, data2, ALUOp, ALUresult, Zero);
	}

	// If ALUSrc is true, second value is extended_value instead of data2.
	else if (ALUSrc) ALU(data1, extended_value, ALUOp, ALUresult, Zero);

	// Perform ALU with data2 as second value.
	else ALU(data1, data2, ALUOp, ALUresult, Zero);

	return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	// Halt if ALUresult is an address and not a multiple of 4.
	// ALUresult is an address when MemWrite or MemRead is true.
	if ((MemWrite || MemRead) && (ALUresult % 4)) return 1;

	/* As explained in the FAQ document, indexes are right shifted by 2 bits
	 * when accessing Mem to reference the proper index. */

	// Write data stored by data2 to Mem index pointed to by ALU result.
	else if (MemWrite) Mem[ALUresult >> 2] = data2;

	// Write data stored at Mem index pointed to by ALUresult to memdata.
	else if (MemRead) *memdata = Mem[ALUresult >> 2];
	return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	// If not writing to register, quit immediately.
	if (!RegWrite) return;

	// RegDst Value of 0 is r2, 1 is r3.
	unsigned registers[] = {r2, r3};

	// MemtoReg value of 1 means to write from Memory, value of 0 means to write from ALUresult.
	unsigned data[] = {ALUresult, memdata};

	// Write the appropriate data to the appropriate register.
	Reg[registers[(int)RegDst]] = data[(int)MemtoReg];
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	// Increment the value of PC by a word
	*PC += 4;

	// Handle branch when zero value.
	if (Zero && Branch) {
		// Add the value of extended value to the PC multiplied by size of word.
		*PC += (extended_value << 2);
	}

	// When Jumping, set PC equal to jsec left shifted by 2 and use the 4 upper bits of the value of PC.
	if (Jump) {
		// 4026531840 is a 32 bit number, like PC, with only the 4 leftmost bits having a value of 1.
		*PC = (jsec << 2) | (*PC & 4026531840);
	}
}

