
/**

----- Instruction Decoding -----
- Intel manual: https://edge.edx.org/c4x/BITSPilani/EEE231/asset/8086_family_Users_Manual_1_.pdf

- Things to parse:
d - the reg field is the destination (1) or not (0).
w - the operation is to be done on 8 (0) or 16 (1) bits of data.
s - immediate data is signed (1) or not (0).
v - the shift count is one (0) or specified in the CL register (1).
z - repeat while zero flag is clear (0) or set (1).
mod - register to register to register (11) or register to memory with no (00), 8 (01), or 16 (10) displacement. If mod is 00 and rm 110, then there is 16 bit displacement.
reg - register operand.
rm - register/memory operand. Usually used for effective address calculations.

disp - displacement value.
data - additional data.

ip - instruction pointer value.
ip inc - increment value to the instruction pointer.
cs - code segment value.
----- Instruction Decoding -----

*/


// -- Includes
#define CUSTOM_CMD_LINE_ARGS_PARAMS part1_custom_cmd_line_args_params
#include "lib/base/inc.h"
// -- Includes


// -- Preprocessor Defines
#define REGISTER_N 8
#define SEGREG_N   4
// -- Preprocessor Defines


// -- Enums/Flags

// ---- Intermediate Representation (IR)
typedef enum IRFlag {
	IRFlag_ParseMod             = 1 << 0,
	IRFlag_ParseReg             = 1 << 1,
	IRFlag_ParseRm              = 1 << 2,
	IRFlag_ParseDispLo          = 1 << 3,
	IRFlag_ParseDispHi          = 1 << 4,
	IRFlag_ParseDataLo          = 1 << 5,
	IRFlag_ParseDataHi          = 1 << 6,
	IRFlag_SkipByte2            = 1 << 7,
	IRFlag_Prefix               = 1 << 8,
	IRFlag_NoExplicitSizeNeeded = 1 << 9,

	IRFlag_HasD      					  = 1 << 10,
	IRFlag_HasW      					  = 1 << 11,
	IRFlag_HasS      					  = 1 << 12,
	IRFlag_HasV      					  = 1 << 13,
	IRFlag_HasZ      					  = 1 << 14,
	IRFlag_HasMod    					  = 1 << 15,
	IRFlag_HasReg    					  = 1 << 16,
	IRFlag_HasRm     					  = 1 << 17,
	IRFlag_HasDisp  					  = 1 << 18,
	IRFlag_HasData  					  = 1 << 19,

	IRFlag_RegIsSegReg          = 1 << 20,
	IRFlag_RegIsOp              = 1 << 21,
	IRFlag_RegIsPrefix          = 1 << 22,
	IRFlag_RmIs16Bits           = 1 << 23,  // ignores w when getting the register as if it was set to 1
	IRFlag_RegIsOpInd           = 1 << 24,
	IRFlag_DataIsIp             = 1 << 25,
	IRFlag_DataIsIpInc          = 1 << 26,
	IRFlag_DataIsCs             = 1 << 27,
	IRFlag_DataIsAddress        = 1 << 28,
	IRFlag_RegIsAccum           = 1 << 29,
	IRFlag_RmIsAccum            = 1 << 30,


	// -- Compounds
	IRFlag_ParseModRegRm = IRFlag_ParseMod | IRFlag_ParseReg | IRFlag_ParseRm,
	IRFlag_ParseDisp     = IRFlag_ParseDispLo | IRFlag_ParseDispHi,
	IRFlag_ParseData     = IRFlag_ParseDataLo | IRFlag_ParseDataHi,
	IRFlag_HasDW         = IRFlag_HasD | IRFlag_HasW,
	// -- Compounds
} IRFlag;

typedef enum IRVarReg {
	IRVarReg_0  = 1 << 0,
	IRVarReg_1  = 1 << 1,
	IRVarReg_2  = 1 << 2,
	IRVarReg_3  = 1 << 3,
	IRVarReg_4  = 1 << 4,
	IRVarReg_5  = 1 << 5,
	IRVarReg_6  = 1 << 6,
	IRVarReg_7  = 1 << 7,
} IRVarReg;
// ---- Intermmediate Representation (IR)

// ---- Operand
typedef enum OperandFlag {
	OperandFlag_Signed       = 1 << 0,
	OperandFlag_Address      = 1 << 1,
	OperandFlag_Immediate    = 1 << 2,
	OperandFlag_Register     = 1 << 3,
	OperandFlag_SegReg       = 1 << 4,
	OperandFlag_OpInd        = 1 << 5,
	OperandFlag_ExplicitSize = 1 << 6,
	OperandFlag_16bits       = 1 << 7,
	OperandFlag_8bits        = 1 << 8,
	OperandFlag_Cs           = 1 << 9,
} OperandFlag;
// ---- Operand

// ---- Flags Register
typedef enum FlagsReg {
	FlagsReg_Carry      = bit0,
	FlagsReg_Parity     = bit2,  // 1 -> number of bits in the lower 8 bits that are set to 1 is a pair number
	FlagsReg_Auxiliary  = bit4,
	FlagsReg_Zero       = bit6,
	FlagsReg_Sign       = bit7,
	FlagsReg_Trap       = bit8,
	FlagsReg_Interrupt  = bit9,
	FlagsReg_Direction  = bit10,
	FlagsReg_Overflow   = bit11,
} FlagsReg;
// ---- Flags Register

// -- Enums/Flags


// -- Types
typedef struct Operand Operand;
struct Operand {
	u16 val;
	s16 disp;
	enum16(OperandFlag) flags;
};

typedef struct IR IR;
struct IR {

	// -- Initial params
	enum32(IRFlag) flags;
	enum32(IRFlag) var_flags;
	String8 mnemonic;
	u8 d;
  u8 w;
	u8 s;
	u8 v;
	u8 z;
	u8 mod;
  u8 reg;
  u8 rm;
	enum8(IRVarReg) var_regs;
	// -- Initial params

	// -- Parsing temporary data
	s16 disp;
	u16 data;
	// -- Parsing temporary data

	// -- Artifacts
	Operand src;
	Operand dest;
	// -- Artifacts
};

typedef struct IRArray IRArray;
struct IRArray {
	Array* array;
};
// -- Types


// -- Functions

// ------ IR Intialization
internalf IR   ir_create(String8 mnemonic, u8 d, u8 w, u8 s, u8 v, u8 z, u8 mod, u8 reg, u8 rm, enum32(IRFlag) flags);
internalf IR   ir_create_with_var_flags(String8 mnemonic, u8 d, u8 w, u8 s, u8 v, u8 z, u8 mod, u8 reg, u8 rm, enum32(IRFlag) flags, enum32(IRFlag) var_flags, enum8(IRVarReg) var_regs);
internalf IR   ir_nil(void);
internalf void ir_instructions_init(void);
// ------ IR Intialization

// ------ Decode
internalf IRArray ir_array_alloc(void);
internalf b8      ir_array_push_back(IRArray* array, IR el);
internalf u32     asm_decode_instruction(IR* instruction, u8* bytes);
internalf IRArray asm_decode(u8* bytes, u32 count);
// ------ Decode

// ------ Disassembly
internalf String8 disasm_register(u8 reg, u8 w);
internalf String8 disasm_segreg(u8 segreg);
internalf String8 disasm_ea_calc(Allocator* allocator, u8 rm, u8 w, u8 mod, s16 disp);
internalf void    disasm_instruction(IR instruction, b8 newline);
internalf void    disasm(IRArray instructions);
// ------ Disassembly

// ------ Execution
internalf void set_flags_reg(u16 reg_val, u16 prev_reg_val, enum16(FlagsReg) flags);
internalf u32  calc_ea(u8 rm, u8 w, u8 mod, s16 disp, u32* clocks);
internalf u32  calc_instruction_clocks(IR instruction, u32* transfer_clocks);
internalf void exec_instruction(IR instruction, b8 is_8086);
internalf void print_registers(void);
// ------ Execution

// -- Functions
