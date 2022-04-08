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
	 * it must have an address that is a multiple of 4.*/
	if (!(PC % 4)) return 1;

	/* Get and set the instruction from the Memory array.
	 * As explained in the FAQ doc, the decimal form of the
	 * instruction is at the index of PC right shifted by 2 bits
	 * or divided by 2^2 */
	*instruction = Mem[(PC >> 2)];
	return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	/* As demonstrated in line 41 of spimcore.c The data is stored at the following positions,
	 * where 0 is the rightmost bit and 31 the leftmost, and has the following constraints:
	 *| Part    | Bits    | # of Bits | Max decimal value     |
	 *|---------+---------+-----------+-----------------------+
	 *| op      | 31 - 26 |  6        |  (2^6) - 1 =       63 |
	 *| r1      | 25 - 21 |  5        |  (2^5) - 1 =       31 |
	 *| r2      | 20 - 16 |  5        |  (2^5) - 1 =       31 |
	 *| r3      | 15 - 11 |  5        |  (2^5) - 1 =       31 |
	 *| funct   |  5 - 0  |  6        |  (2^6) - 1 =       63 |
	 *| offset  | 15 - 0  | 16        | (2^16) - 1 =    65535 |
	 *| jsec    | 25 - 0  | 26        | (2^26) - 1 = 67108863 |
	 * -------------------------------------------------------+
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
	char halt = 0;
	return halt;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	char halt = 0;
	return halt;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	char halt = 0;
	return halt;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

