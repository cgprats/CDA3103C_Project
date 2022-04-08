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
	char halt = 0;

	/* Get and set the instruction from the Memory array.
	 * As explained in the FAQ doc, this is stored at the
	 * index equivalent to PC right shifted by 2 bits
	 * or divided by 2^2
	 */
	*instruction = Mem[(PC >> 2)];
	return halt;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

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

