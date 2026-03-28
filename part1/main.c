

#include "main.h"


// -- Global State
global CmdLineArgParams part1_custom_cmd_line_args_params[] = {
	{ str8_lit_comp("-disasm8086"), 1 },
	{ str8_lit_comp("-exec8086"), 0 },
};

global IR ir_table[256];

global u8 seg_override_prefix;

global u16 registers[REGISTER_N];
global u16 segregs[SEGREG_N];
global u16 flags_reg;
global u16 ip_reg;

global u8 memory[1024*1024];

global String8 flags_reg_str = { (u8*)"C P A ZSTIDO    ", 16 };

global u32 clocks_total = 0;
// -- Global State


// -- Body Includes
#include "lib/base/inc.c"
// -- Body Includes


// -- Functions

// ---- IR Intialization
internalf IR ir_create(String8 mnemonic, u8 d, u8 w, u8 s, u8 v, u8 z, u8 mod, u8 reg, u8 rm, enum32(IRFlag) flags) {

	IR result = ir_nil();

	result.mnemonic = mnemonic;
	result.d = d;
  result.w = w;
	result.s = s;
	result.v = v;
	result.z = z;
  result.mod = mod;
  result.reg = reg;
  result.rm = rm;
	result.flags = flags;
	result.var_flags = 0;
	result.var_regs = 0;

	return result;
}

internalf IR ir_create_with_var_flags(String8 mnemonic, u8 d, u8 w, u8 s, u8 v, u8 z, u8 mod, u8 reg, u8 rm, enum32(IRFlag) flags, enum32(IRFlag) var_flags, enum8(IRVarReg) var_regs) {
	IR result = ir_create(mnemonic, d, w, s, v, z, mod, reg, rm, flags);
	result.var_regs = var_regs;
	result.var_flags = var_flags;
	return result;
}

internalf IR ir_nil(void) {
	IR result;
	mem_zero(&result, sizeof(result));
	return result;
}

internalf void ir_instructions_init(void) {

	// -- Unused
	ir_table[15] = ir_nil();
	ir_table[96] = ir_nil();
	ir_table[97] = ir_nil();
	ir_table[98] = ir_nil();
	ir_table[99] = ir_nil();
	ir_table[100] = ir_nil();
	ir_table[101] = ir_nil();
	ir_table[102] = ir_nil();
	ir_table[103] = ir_nil();
	ir_table[104] = ir_nil();
	ir_table[105] = ir_nil();
	ir_table[106] = ir_nil();
	ir_table[107] = ir_nil();
	ir_table[108] = ir_nil();
	ir_table[109] = ir_nil();
	ir_table[110] = ir_nil();
	ir_table[111] = ir_nil();
	ir_table[192] = ir_nil();
	ir_table[193] = ir_nil();
	ir_table[200] = ir_nil();
	ir_table[201] = ir_nil();
	ir_table[214] = ir_nil();
	ir_table[241] = ir_nil();
	// -- Unused


	// -- Static (1 Byte)

	// ---- Adjusts
	ir_table[39] = ir_create(str8_lit("daa"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[47] = ir_create(str8_lit("das"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[55] = ir_create(str8_lit("aaa"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[63] = ir_create(str8_lit("aas"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	// ---- Adjusts

	// ---- Processor Control
	ir_table[155] = ir_create(str8_lit("wait"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[240] = ir_create(str8_lit("lock"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_Prefix);
	ir_table[244] = ir_create(str8_lit("hlt"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[245] = ir_create(str8_lit("cmc"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[248] = ir_create(str8_lit("clc"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[249] = ir_create(str8_lit("stc"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[250] = ir_create(str8_lit("cli"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[251] = ir_create(str8_lit("sti"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[252] = ir_create(str8_lit("cld"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[253] = ir_create(str8_lit("std"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	// ---- Processor Control

	// ---- Return
	ir_table[195] = ir_create(str8_lit("ret"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[203] = ir_create(str8_lit("retf"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	// ---- Return

	// ---- Interrupt
	ir_table[204] = ir_create(str8_lit("int3"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[206] = ir_create(str8_lit("into"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[207] = ir_create(str8_lit("iret"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	// ---- Interrupt

	// ---- Flags
	ir_table[215] = ir_create(str8_lit("xlat"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[158] = ir_create(str8_lit("sahf"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[159] = ir_create(str8_lit("lahf"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[156] = ir_create(str8_lit("pushf"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[157] = ir_create(str8_lit("popf"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	// ---- Flags

	// ---- Conversions
	ir_table[152] = ir_create(str8_lit("cbw"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	ir_table[153] = ir_create(str8_lit("cwd"), 0, 0, 0, 0, 0, 0, 0, 0, 0);
	// ---- Conversions

	// ---- Register

	// ------ Push/Pop
	ir_table[80] = ir_create(str8_lit("push"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[81] = ir_create(str8_lit("push"), 0, 1, 0, 0, 0, 0, 1, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[82] = ir_create(str8_lit("push"), 0, 1, 0, 0, 0, 0, 2, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[83] = ir_create(str8_lit("push"), 0, 1, 0, 0, 0, 0, 3, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[84] = ir_create(str8_lit("push"), 0, 1, 0, 0, 0, 0, 4, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[85] = ir_create(str8_lit("push"), 0, 1, 0, 0, 0, 0, 5, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[86] = ir_create(str8_lit("push"), 0, 1, 0, 0, 0, 0, 6, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[87] = ir_create(str8_lit("push"), 0, 1, 0, 0, 0, 0, 7, 0, IRFlag_HasW | IRFlag_HasReg);
	
	ir_table[88] = ir_create(str8_lit("pop"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[89] = ir_create(str8_lit("pop"), 0, 1, 0, 0, 0, 0, 1, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[90] = ir_create(str8_lit("pop"), 0, 1, 0, 0, 0, 0, 2, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[91] = ir_create(str8_lit("pop"), 0, 1, 0, 0, 0, 0, 3, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[92] = ir_create(str8_lit("pop"), 0, 1, 0, 0, 0, 0, 4, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[93] = ir_create(str8_lit("pop"), 0, 1, 0, 0, 0, 0, 5, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[94] = ir_create(str8_lit("pop"), 0, 1, 0, 0, 0, 0, 6, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[95] = ir_create(str8_lit("pop"), 0, 1, 0, 0, 0, 0, 7, 0, IRFlag_HasW | IRFlag_HasReg);
	// ------ Push/Pop

	// ------ Increment/Decrement
	ir_table[64] = ir_create(str8_lit("inc"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[65] = ir_create(str8_lit("inc"), 0, 1, 0, 0, 0, 0, 1, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[66] = ir_create(str8_lit("inc"), 0, 1, 0, 0, 0, 0, 2, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[67] = ir_create(str8_lit("inc"), 0, 1, 0, 0, 0, 0, 3, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[68] = ir_create(str8_lit("inc"), 0, 1, 0, 0, 0, 0, 4, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[69] = ir_create(str8_lit("inc"), 0, 1, 0, 0, 0, 0, 5, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[70] = ir_create(str8_lit("inc"), 0, 1, 0, 0, 0, 0, 6, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[71] = ir_create(str8_lit("inc"), 0, 1, 0, 0, 0, 0, 7, 0, IRFlag_HasW | IRFlag_HasReg);
	
	ir_table[72] = ir_create(str8_lit("dec"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[73] = ir_create(str8_lit("dec"), 0, 1, 0, 0, 0, 0, 1, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[74] = ir_create(str8_lit("dec"), 0, 1, 0, 0, 0, 0, 2, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[75] = ir_create(str8_lit("dec"), 0, 1, 0, 0, 0, 0, 3, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[76] = ir_create(str8_lit("dec"), 0, 1, 0, 0, 0, 0, 4, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[77] = ir_create(str8_lit("dec"), 0, 1, 0, 0, 0, 0, 5, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[78] = ir_create(str8_lit("dec"), 0, 1, 0, 0, 0, 0, 6, 0, IRFlag_HasW | IRFlag_HasReg);
	ir_table[79] = ir_create(str8_lit("dec"), 0, 1, 0, 0, 0, 0, 7, 0, IRFlag_HasW | IRFlag_HasReg);
	// ------ Increment/Decrement

	// ---- Exchange Register with Accumulator
	ir_table[144] = ir_create(str8_lit("xchg"), 0, 1, 0, 0, 0, 3, 0, 0, IRFlag_HasW | IRFlag_HasMod | IRFlag_HasReg | IRFlag_HasRm | IRFlag_RmIsAccum);  // NOP
	ir_table[145] = ir_create(str8_lit("xchg"), 0, 1, 0, 0, 0, 3, 1, 0, IRFlag_HasW | IRFlag_HasMod | IRFlag_HasReg | IRFlag_HasRm | IRFlag_RmIsAccum);
	ir_table[146] = ir_create(str8_lit("xchg"), 0, 1, 0, 0, 0, 3, 2, 0, IRFlag_HasW | IRFlag_HasMod | IRFlag_HasReg | IRFlag_HasRm | IRFlag_RmIsAccum);
	ir_table[147] = ir_create(str8_lit("xchg"), 0, 1, 0, 0, 0, 3, 3, 0, IRFlag_HasW | IRFlag_HasMod | IRFlag_HasReg | IRFlag_HasRm | IRFlag_RmIsAccum);
	ir_table[148] = ir_create(str8_lit("xchg"), 0, 1, 0, 0, 0, 3, 4, 0, IRFlag_HasW | IRFlag_HasMod | IRFlag_HasReg | IRFlag_HasRm | IRFlag_RmIsAccum);
	ir_table[149] = ir_create(str8_lit("xchg"), 0, 1, 0, 0, 0, 3, 5, 0, IRFlag_HasW | IRFlag_HasMod | IRFlag_HasReg | IRFlag_HasRm | IRFlag_RmIsAccum);
	ir_table[150] = ir_create(str8_lit("xchg"), 0, 1, 0, 0, 0, 3, 6, 0, IRFlag_HasW | IRFlag_HasMod | IRFlag_HasReg | IRFlag_HasRm | IRFlag_RmIsAccum);
	ir_table[151] = ir_create(str8_lit("xchg"), 0, 1, 0, 0, 0, 3, 7, 0, IRFlag_HasW | IRFlag_HasMod | IRFlag_HasReg | IRFlag_HasRm | IRFlag_RmIsAccum);
	// ---- Exchange Register with Accumulator

	// ---- Register

	// ---- Segment Register

	// ------ Push/Pop
	ir_table[6] = ir_create(str8_lit("push"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasReg | IRFlag_RegIsSegReg);
	ir_table[14] = ir_create(str8_lit("push"), 0, 0, 0, 0, 0, 0, 1, 0, IRFlag_HasReg | IRFlag_RegIsSegReg);
	ir_table[22] = ir_create(str8_lit("push"), 0, 0, 0, 0, 0, 0, 2, 0, IRFlag_HasReg | IRFlag_RegIsSegReg);
	ir_table[30] = ir_create(str8_lit("push"), 0, 0, 0, 0, 0, 0, 3, 0, IRFlag_HasReg | IRFlag_RegIsSegReg);

	ir_table[7] = ir_create(str8_lit("pop"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasReg | IRFlag_RegIsSegReg);
	ir_table[23] = ir_create(str8_lit("pop"), 0, 0, 0, 0, 0, 0, 2, 0, IRFlag_HasReg | IRFlag_RegIsSegReg);
	ir_table[31] = ir_create(str8_lit("pop"), 0, 0, 0, 0, 0, 0, 3, 0, IRFlag_HasReg | IRFlag_RegIsSegReg);
	// ------ Push/Pop

	// ------ Override prefix
	ir_table[38] = ir_create(str8_nil(), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasReg | IRFlag_RegIsSegReg | IRFlag_RegIsPrefix);
	ir_table[46] = ir_create(str8_nil(), 0, 0, 0, 0, 0, 0, 1, 0, IRFlag_HasReg | IRFlag_RegIsSegReg | IRFlag_RegIsPrefix);
	ir_table[54] = ir_create(str8_nil(), 0, 0, 0, 0, 0, 0, 2, 0, IRFlag_HasReg | IRFlag_RegIsSegReg | IRFlag_RegIsPrefix);
	ir_table[62] = ir_create(str8_nil(), 0, 0, 0, 0, 0, 0, 3, 0, IRFlag_HasReg | IRFlag_RegIsSegReg | IRFlag_RegIsPrefix);
	// ------ Override prefix

	// ---- Segment Register

	// ---- Variable Port
	ir_table[236] = ir_create(str8_lit("in"), 1, 0, 0, 0, 0, 0, 0, 2, IRFlag_HasDW | IRFlag_HasReg | IRFlag_HasRm | IRFlag_RmIs16Bits);
	ir_table[237] = ir_create(str8_lit("in"), 1, 1, 0, 0, 0, 0, 0, 2, IRFlag_HasDW | IRFlag_HasReg | IRFlag_HasRm | IRFlag_RmIs16Bits);
	ir_table[238] = ir_create(str8_lit("out"), 0, 0, 0, 0, 0, 0, 0, 2, IRFlag_HasDW | IRFlag_HasReg | IRFlag_HasRm | IRFlag_RmIs16Bits);
	ir_table[239] = ir_create(str8_lit("out"), 0, 1, 0, 0, 0, 0, 0, 2, IRFlag_HasDW | IRFlag_HasReg | IRFlag_HasRm | IRFlag_RmIs16Bits);
	// ---- Variable Port

	// ---- String Manipulation
	ir_table[164] = ir_create(str8_lit("movsb"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW);
	ir_table[165] = ir_create(str8_lit("movsw"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW);
	ir_table[166] = ir_create(str8_lit("cmpsb"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW);
	ir_table[167] = ir_create(str8_lit("cmpsw"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW);
	ir_table[170] = ir_create(str8_lit("stosb"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW);
	ir_table[171] = ir_create(str8_lit("stosw"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW);
	ir_table[172] = ir_create(str8_lit("lodsb"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW);
	ir_table[173] = ir_create(str8_lit("lodsw"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW);
	ir_table[174] = ir_create(str8_lit("scasb"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW);
	ir_table[175] = ir_create(str8_lit("scasw"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW);
	ir_table[242] = ir_create(str8_lit("repne"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasZ | IRFlag_Prefix);
	ir_table[243] = ir_create(str8_lit("rep"), 0, 0, 0, 0, 1, 0, 0, 0, IRFlag_HasZ | IRFlag_Prefix);
	// ---- String Manipulation

	// ---- Fixed Port
	ir_table[228] = ir_create(str8_lit("in"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasDW | IRFlag_HasReg | IRFlag_ParseDataLo);
	ir_table[229] = ir_create(str8_lit("in"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasDW | IRFlag_HasReg | IRFlag_ParseDataLo);
	ir_table[230] = ir_create(str8_lit("out"), 1, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasDW | IRFlag_HasReg | IRFlag_ParseDataLo);
	ir_table[231] = ir_create(str8_lit("out"), 1, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasDW | IRFlag_HasReg | IRFlag_ParseDataLo);
	// ---- Fixed Port

	// -- Static (1 Byte)


	// -- Single data byte (2 Bytes)
	
	// ---- Control Tranfer
	ir_table[112] = ir_create(str8_lit("jo"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[113] = ir_create(str8_lit("jno"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[114] = ir_create(str8_lit("jb"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[115] = ir_create(str8_lit("jae"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[116] = ir_create(str8_lit("je"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[117] = ir_create(str8_lit("jne"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[118] = ir_create(str8_lit("jbe"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[119] = ir_create(str8_lit("ja"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[120] = ir_create(str8_lit("js"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[121] = ir_create(str8_lit("jns"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[122] = ir_create(str8_lit("jp"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[123] = ir_create(str8_lit("jnp"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[124] = ir_create(str8_lit("jl"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[125] = ir_create(str8_lit("jnl"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[126] = ir_create(str8_lit("jle"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[127] = ir_create(str8_lit("jnle"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[224] = ir_create(str8_lit("loopne"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[225] = ir_create(str8_lit("loope"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[226] = ir_create(str8_lit("loop"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	ir_table[227] = ir_create(str8_lit("jcxz"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	// ---- Control Tranfer

	// ---- Unconditional Jump
	ir_table[235] = ir_create(str8_lit("jmp"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo | IRFlag_DataIsIpInc);
	// ---- Unconditional Jump

	// ---- Interrupt
	ir_table[205] = ir_create(str8_lit("int"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDataLo);
	// ---- Interrupt

	// -- Single data byte (2 Bytes)


	// -- Two data bytes (3 bytes)

	// ---- Control Transfer
	ir_table[232] = ir_create(str8_lit("call"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseData | IRFlag_DataIsIpInc);
	// ---- Control Transfer

	// ---- Unconditional Jumps
	ir_table[233] = ir_create(str8_lit("jmp"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseData | IRFlag_DataIsIpInc);
	// ---- Unconditional Jumps

	// ---- Return from CALL
	ir_table[194] = ir_create(str8_lit("ret"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseData);
	ir_table[202] = ir_create(str8_lit("retf"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseData);
	// ---- Return from CALL

	// -- Two data bytes (3 bytes)


	// -- Four data bytes (5 bytes)

	// ---- Control Transfer
	ir_table[154] = ir_create(str8_lit("call"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDisp | IRFlag_ParseData | IRFlag_DataIsCs);
	// ---- Control Transfer

	// ---- Unconditional Jumps
	ir_table[234] = ir_create(str8_lit("jmp"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_ParseDisp | IRFlag_ParseData | IRFlag_DataIsCs);
	// ---- Unconditional Jumps

	// -- Four data bytes (5 bytes)


	// -- Variable data bytes (2/3 bytes)

	// ---- Immediate to Register
	ir_table[176] = ir_create(str8_lit("mov"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[177] = ir_create(str8_lit("mov"), 0, 0, 0, 0, 0, 0, 1, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[178] = ir_create(str8_lit("mov"), 0, 0, 0, 0, 0, 0, 2, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[179] = ir_create(str8_lit("mov"), 0, 0, 0, 0, 0, 0, 3, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[180] = ir_create(str8_lit("mov"), 0, 0, 0, 0, 0, 0, 4, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[181] = ir_create(str8_lit("mov"), 0, 0, 0, 0, 0, 0, 5, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[182] = ir_create(str8_lit("mov"), 0, 0, 0, 0, 0, 0, 6, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[183] = ir_create(str8_lit("mov"), 0, 0, 0, 0, 0, 0, 7, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[184] = ir_create(str8_lit("mov"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[185] = ir_create(str8_lit("mov"), 0, 1, 0, 0, 0, 0, 1, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[186] = ir_create(str8_lit("mov"), 0, 1, 0, 0, 0, 0, 2, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[187] = ir_create(str8_lit("mov"), 0, 1, 0, 0, 0, 0, 3, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[188] = ir_create(str8_lit("mov"), 0, 1, 0, 0, 0, 0, 4, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[189] = ir_create(str8_lit("mov"), 0, 1, 0, 0, 0, 0, 5, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[190] = ir_create(str8_lit("mov"), 0, 1, 0, 0, 0, 0, 6, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	ir_table[191] = ir_create(str8_lit("mov"), 0, 1, 0, 0, 0, 0, 7, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData);
	// ---- Immediate to Register

	// ---- Immediate to Accumulator
	ir_table[4] = ir_create(str8_lit("add"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);
	ir_table[5] = ir_create(str8_lit("add"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);

	ir_table[20] = ir_create(str8_lit("adc"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);
	ir_table[21] = ir_create(str8_lit("adc"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);

	ir_table[44] = ir_create(str8_lit("sub"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);
	ir_table[45] = ir_create(str8_lit("sub"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);
	
	ir_table[28] = ir_create(str8_lit("sbb"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);
	ir_table[29] = ir_create(str8_lit("sbb"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);

	ir_table[60] = ir_create(str8_lit("cmp"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);
	ir_table[61] = ir_create(str8_lit("cmp"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);

	ir_table[36] = ir_create(str8_lit("and"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);
	ir_table[37] = ir_create(str8_lit("and"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);

	ir_table[168] = ir_create(str8_lit("test"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);
	ir_table[169] = ir_create(str8_lit("test"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);

	ir_table[12] = ir_create(str8_lit("or"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);
	ir_table[13] = ir_create(str8_lit("or"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);

	ir_table[52] = ir_create(str8_lit("xor"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);
	ir_table[53] = ir_create(str8_lit("xor"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_RegIsAccum);
	// ---- Immediate to Accumulator

	// ---- Memory to/from Accumulator
	ir_table[160] = ir_create(str8_lit("mov"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasDW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_DataIsAddress | IRFlag_RegIsAccum);
	ir_table[161] = ir_create(str8_lit("mov"), 0, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasDW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_DataIsAddress | IRFlag_RegIsAccum);
	ir_table[162] = ir_create(str8_lit("mov"), 1, 0, 0, 0, 0, 0, 0, 0, IRFlag_HasDW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_DataIsAddress | IRFlag_RegIsAccum);
	ir_table[163] = ir_create(str8_lit("mov"), 1, 1, 0, 0, 0, 0, 0, 0, IRFlag_HasDW | IRFlag_HasReg | IRFlag_ParseData | IRFlag_DataIsAddress | IRFlag_RegIsAccum);
	// ---- Memory to/from Accumulator

	// -- Variable data bytes (2/3 bytes)


	// -- Variable Disp (3-4 bytes)

	// ---- Register/memory with register to either
	ir_table[136] = ir_create(str8_lit("mov"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[137] = ir_create(str8_lit("mov"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[138] = ir_create(str8_lit("mov"), 1, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[139] = ir_create(str8_lit("mov"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	
	ir_table[0] = ir_create(str8_lit("add"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[1] = ir_create(str8_lit("add"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[2] = ir_create(str8_lit("add"), 1, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[3] = ir_create(str8_lit("add"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	
	// NOTE: first byte is the same as for the TEST instruction
	ir_table[16] = ir_create(str8_lit("adc"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[17] = ir_create(str8_lit("adc"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[18] = ir_create(str8_lit("adc"), 1, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[19] = ir_create(str8_lit("adc"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	// NOTE

	ir_table[40] = ir_create(str8_lit("sub"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[41] = ir_create(str8_lit("sub"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[42] = ir_create(str8_lit("sub"), 1, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[43] = ir_create(str8_lit("sub"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);

	ir_table[24] = ir_create(str8_lit("sbb"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[25] = ir_create(str8_lit("sbb"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[26] = ir_create(str8_lit("sbb"), 1, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[27] = ir_create(str8_lit("sbb"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);

	ir_table[56] = ir_create(str8_lit("cmp"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[57] = ir_create(str8_lit("cmp"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[58] = ir_create(str8_lit("cmp"), 1, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[59] = ir_create(str8_lit("cmp"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);

	ir_table[32] = ir_create(str8_lit("and"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[33] = ir_create(str8_lit("and"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[34] = ir_create(str8_lit("and"), 1, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[35] = ir_create(str8_lit("and"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);

	ir_table[8] = ir_create(str8_lit("or"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[9] = ir_create(str8_lit("or"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[10] = ir_create(str8_lit("or"), 1, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[11] = ir_create(str8_lit("or"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	
	ir_table[48] = ir_create(str8_lit("xor"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[49] = ir_create(str8_lit("xor"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[50] = ir_create(str8_lit("xor"), 1, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[51] = ir_create(str8_lit("xor"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);

	ir_table[132] = ir_create(str8_lit("test"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[133] = ir_create(str8_lit("test"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);

	ir_table[134] = ir_create(str8_lit("xchg"), 1, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[135] = ir_create(str8_lit("xchg"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	// ---- Register/memory with register to either

	// ---- Segment Register to/from Memory/Register
	ir_table[140] = ir_create(str8_lit("mov"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_RegIsSegReg | IRFlag_ParseDisp);
	ir_table[142] = ir_create(str8_lit("mov"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_RegIsSegReg | IRFlag_ParseDisp);
	// ---- Segment Register to/from Memory/Register

	// ---- Register/Memory
	ir_table[255] = ir_create_with_var_flags(str8_lit("inc|dec|call|call far|jmp|jmp far|push|"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_RegIsOp, IRFlag_NoExplicitSizeNeeded, IRVarReg_2 | IRVarReg_3 | IRVarReg_4 | IRVarReg_5);
	ir_table[143] = ir_create(str8_lit("pop"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_RegIsOp);
	ir_table[254] = ir_create(str8_lit("inc|dec||||||"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_RegIsOp);
	// ---- Register/Memory

	// ---- Loads
	ir_table[141] = ir_create(str8_lit("lea"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[196] = ir_create(str8_lit("les"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	ir_table[197] = ir_create(str8_lit("lds"), 1, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasDW | IRFlag_ParseModRegRm | IRFlag_ParseDisp);
	// ---- Load

	// ---- Escape to External Device
	ir_table[216] = ir_create(str8_lit("esc"), 0, 0, 0, 0, 0, 6, 0, 0, IRFlag_ParseMod | IRFlag_ParseRm | IRFlag_ParseDisp);
	ir_table[217] = ir_create(str8_lit("esc"), 0, 0, 0, 0, 0, 6, 0, 0, IRFlag_ParseMod | IRFlag_ParseRm | IRFlag_ParseDisp);
	ir_table[218] = ir_create(str8_lit("esc"), 0, 0, 0, 0, 0, 6, 0, 0, IRFlag_ParseMod | IRFlag_ParseRm | IRFlag_ParseDisp);
	ir_table[219] = ir_create(str8_lit("esc"), 0, 0, 0, 0, 0, 6, 0, 0, IRFlag_ParseMod | IRFlag_ParseRm | IRFlag_ParseDisp);
	ir_table[220] = ir_create(str8_lit("esc"), 0, 0, 0, 0, 0, 6, 0, 0, IRFlag_ParseMod | IRFlag_ParseRm | IRFlag_ParseDisp);
	ir_table[221] = ir_create(str8_lit("esc"), 0, 0, 0, 0, 0, 6, 0, 0, IRFlag_ParseMod | IRFlag_ParseRm | IRFlag_ParseDisp);
	ir_table[222] = ir_create(str8_lit("esc"), 0, 0, 0, 0, 0, 6, 0, 0, IRFlag_ParseMod | IRFlag_ParseRm | IRFlag_ParseDisp);
	ir_table[223] = ir_create(str8_lit("esc"), 0, 0, 0, 0, 0, 6, 0, 0, IRFlag_ParseMod | IRFlag_ParseRm | IRFlag_ParseDisp);
	// ---- Escape to External Device

	// ---- ACSII Adjust
	ir_table[212] = ir_create(str8_lit("aam"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_SkipByte2);
	ir_table[213] = ir_create(str8_lit("aad"), 0, 0, 0, 0, 0, 0, 0, 0, IRFlag_SkipByte2);
	// ---- ACSII Adjust

	// ---- Rotations/Shifts
	ir_table[208] = ir_create(str8_lit("rol|ror|rcl|rcr|shl|shr||sar"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_HasV | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_RegIsOp);
	ir_table[209] = ir_create(str8_lit("rol|ror|rcl|rcr|shl|shr||sar"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_HasV | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_RegIsOp);
	ir_table[210] = ir_create(str8_lit("rol|ror|rcl|rcr|shl|shr||sar"), 0, 0, 0, 1, 0, 6, 3, 0, IRFlag_HasW | IRFlag_HasV | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_RegIsOp);
	ir_table[211] = ir_create(str8_lit("rol|ror|rcl|rcr|shl|shr||sar"), 0, 1, 0, 1, 0, 6, 3, 0, IRFlag_HasW | IRFlag_HasV | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_RegIsOp);
	// ---- Rotations/Shifts

	// -- Variable Disp (3-4 bytes)


	// -- Variable Disp and Data (3-6 bytes)

	// ---- Immediate to/and Register/Memory
	ir_table[198] = ir_create(str8_lit("mov"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_ParseData | IRFlag_RegIsOp);
	ir_table[199] = ir_create(str8_lit("mov"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_ParseData | IRFlag_RegIsOp);
	ir_table[128] = ir_create(str8_lit("add|or|adc|sbb|and|sub|xor|cmp"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_HasS | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_ParseData | IRFlag_RegIsOp);
	ir_table[129] = ir_create(str8_lit("add|or|adc|sbb|and|sub|xor|cmp"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_HasS | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_ParseData | IRFlag_RegIsOp);
	ir_table[130] = ir_create(str8_lit("add||adc|sbb||sub||cmp"), 0, 0, 1, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_HasS | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_ParseData | IRFlag_RegIsOp);
	ir_table[131] = ir_create(str8_lit("add||adc|sbb||sub||cmp"), 0, 1, 1, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_HasS | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_ParseData | IRFlag_RegIsOp);
	ir_table[246] = ir_create_with_var_flags(str8_lit("test||not|neg|mul|imul|div|idiv"), 0, 0, 0, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_RegIsOp, IRFlag_ParseData, IRVarReg_0);
	ir_table[247] = ir_create_with_var_flags(str8_lit("test||not|neg|mul|imul|div|idiv"), 0, 1, 0, 0, 0, 6, 3, 0, IRFlag_HasW | IRFlag_ParseModRegRm | IRFlag_ParseDisp | IRFlag_RegIsOp, IRFlag_ParseData, IRVarReg_0);
	// ---- Immediate to/and Register/Memory
	
	// -- Variable Disp and Data (3-6 bytes)
}
// ---- IR Intialization

// ---- Decode
internalf IRArray ir_array_alloc(void) {
	IRArray result;
	result.array = array_alloc(sizeof(IR), 10);
	return result;
}

internalf b8 ir_array_push_back(IRArray* array, IR el) {
	return array_push_back(array->array, &el, 10);
}

internalf u32 asm_decode_instruction(IR* instruction, u8* bytes) {
	
	u32 inc = 0;

	b8 check_var_flags;
	u32 n_disp_bytes;
	u32 n_data_bytes;
	
	u8* byte_to_parse;

	byte_to_parse = &bytes[0];
	*instruction = ir_table[byte_to_parse[0]];
	inc = 1;


	// -- Retrieve Data
	if(instruction->mnemonic.str && mem_match_cstr8_lit(instruction->mnemonic.str, "ESC", sizeof("ESC") - 1)) {  // exception to rule
		instruction->reg = (u8)(((bytes[1] & 0x38) | (bytes[0] & 3)) << 2);
		instruction->flags |= (IRFlag_HasReg | IRFlag_RegIsOpInd);
		inc += 1;
	}
	
	if(instruction->flags & IRFlag_SkipByte2) {
		inc += 1;
	}
	
	if(instruction->flags & IRFlag_ParseModRegRm) {
		byte_to_parse = &bytes[inc];
		inc += 1;

		if(instruction->flags & IRFlag_ParseMod) {
			instruction->mod = (byte_to_parse[0] >> instruction->mod) & 3;
			instruction->flags |= IRFlag_HasMod;
		}

		if(instruction->flags & IRFlag_ParseReg) {

			if(instruction->flags & IRFlag_RegIsPrefix) {
				// NOTE: reg is already in the first byte, so this never happens (nothing to do)
			}

			if(instruction->flags & IRFlag_RegIsSegReg) {
				instruction->reg = (byte_to_parse[0] >> instruction->reg) & 3;
			} else {
				instruction->reg = (byte_to_parse[0] >> instruction->reg) & 7;
			}

			if(instruction->flags & IRFlag_RegIsOp) {
				// -- get position of mnemonic
				u64 mnemonic_pos = 0;
				if(instruction->reg) {
					mnemonic_pos = str8_find_char_pos(instruction->mnemonic, '|', instruction->reg, 0);
					instruction->mnemonic = str8_skip(instruction->mnemonic, mnemonic_pos);
				}
				// -- get position of mnemonic

				// -- get length of mnemonic
				mnemonic_pos = str8_find_char_pos(instruction->mnemonic, '|', 1, StrFindFlag_ReturnCountOnFail);
				if(mnemonic_pos) {
					instruction->mnemonic = str8_prefix(instruction->mnemonic, mnemonic_pos - 1);
				}
				// -- get length of mnemonic

			} else {  // if the parsed reg field is not to define the opcode, then it has to be a register or segment register name
				instruction->flags |= IRFlag_HasReg;
			}
		}

		if(instruction->flags & IRFlag_ParseRm) {
			instruction->rm = (byte_to_parse[0] >> instruction->rm) & 7;
			instruction->flags |= IRFlag_HasRm;
		}
	}

	if((instruction->flags & IRFlag_RegIsOp) && bitset_bit_is_set(instruction->var_regs, instruction->reg)) {
		check_var_flags = 1;
	} else {
		check_var_flags = 0;
	}

	if((instruction->flags & IRFlag_ParseDisp) || (check_var_flags && (instruction->var_flags & IRFlag_ParseDisp))) {
		n_disp_bytes = 0;

		if(instruction->flags & IRFlag_HasMod) {
			if(instruction->mod < 3) {
				if((instruction->flags & IRFlag_HasRm) && (instruction->rm == 6) && (instruction->mod == 0)) {
					n_disp_bytes = 2;
				} else if(instruction->mod > 0) {
					n_disp_bytes = instruction->mod;
				}
			}
		} else {
			if(instruction->flags & IRFlag_ParseDispHi) {  // 16 bytes
				n_disp_bytes = 2;
			} else {  // 8 bytes
				n_disp_bytes = 1;
			}
		}

		if(n_disp_bytes) {
			byte_to_parse = &bytes[inc];
			inc += n_disp_bytes;

			if(n_disp_bytes == 2) {
				instruction->disp = (s16)word_from_bytes(byte_to_parse[1], byte_to_parse[0]);
			} else {
				instruction->disp = sign_extend_u8(byte_to_parse[0]);
			}
			
			instruction->flags |= IRFlag_HasDisp;
		}
	}

	if((instruction->flags & IRFlag_ParseData) || (check_var_flags && (instruction->var_flags & IRFlag_ParseData))) {
		n_data_bytes = 0;
			
		if(!(instruction->flags & IRFlag_ParseDataHi) && (instruction->flags & IRFlag_ParseDataLo)) {
			n_data_bytes = 1;
		} else if(instruction->flags & IRFlag_HasW) {
			if((instruction->flags & IRFlag_HasS) && instruction->s) {
				n_data_bytes = 1;
			} else {
				n_data_bytes = 1 << instruction->w;	
			}
		} else {
			n_data_bytes = 2;
		}

		if(n_data_bytes) {
			byte_to_parse = &bytes[inc];
			inc += n_data_bytes;

			if(n_data_bytes == 2) {
				instruction->data = word_from_bytes(byte_to_parse[1], byte_to_parse[0]);
				if(instruction->flags & IRFlag_DataIsIpInc) {
					instruction->data += ip_reg + inc;
				}
			} else {
				u8 data = byte_to_parse[0];
				if(instruction->flags & IRFlag_HasS) {
					if(instruction->s && instruction->w) {
						instruction->data = (u16)sign_extend_u8(data);
					} else {
						instruction->data = data;
					}
				} else if(instruction->flags & IRFlag_DataIsIpInc) {
					instruction->data = (u16)sign_extend_u8(data);
				} else {
					instruction->data = data;
				}
			}

			instruction->flags |= IRFlag_HasData;
		}
	}
	// -- Retrieve Data


	// -- Get source and destination
	{
		Operand operand_reg = { 0, 0, 0 };
		Operand operand_rm = { 0, 0, 0 };
		Operand operand_data = { 0, 0, 0 };


		if(instruction->flags & IRFlag_HasReg) {
			if(!(instruction->flags & IRFlag_RegIsPrefix)) {
				operand_reg.val = instruction->reg;
				if(instruction->flags & IRFlag_RegIsOpInd) {
					operand_reg.flags |= OperandFlag_OpInd;
				} else {
					operand_reg.flags |= OperandFlag_Register;
				}
			}

			if(instruction->flags & IRFlag_RegIsSegReg) {
				operand_reg.flags |= OperandFlag_SegReg;
			}
		}

		if(instruction->flags & IRFlag_HasRm) {
			operand_rm.val = instruction->rm;

			if(instruction->flags & IRFlag_HasDisp) {
				operand_rm.disp = instruction->disp;

				operand_rm.flags |= (OperandFlag_Address | OperandFlag_Signed);
			} else {
				if((instruction->flags & IRFlag_HasMod) && (instruction->mod == 0)) {
					operand_rm.flags |= OperandFlag_Address;
				} else {
					operand_rm.flags |= OperandFlag_Register;
				}
			}

			if(instruction->flags & IRFlag_RmIs16Bits) {
				operand_rm.flags |= OperandFlag_16bits;
			}
		}

		if(instruction->flags & IRFlag_HasData) {
			operand_data.val = instruction->data;

			if(instruction->flags & IRFlag_DataIsAddress) {
				operand_data.flags |= (OperandFlag_Address | OperandFlag_Immediate);
			} else if(instruction->flags & IRFlag_DataIsCs) {
				operand_data.disp = instruction->disp;
				operand_data.flags |= OperandFlag_Cs;
			} else if(instruction->flags & IRFlag_DataIsIp) {
				operand_data.flags |= (OperandFlag_Address | OperandFlag_Immediate);
			} else if(instruction->flags & IRFlag_DataIsIpInc) {
				if(n_data_bytes == 1) {
					operand_data.flags |= OperandFlag_Signed;
				}
				operand_data.flags |= OperandFlag_Immediate;
			} else {
				operand_data.flags |= OperandFlag_Immediate;
			}
			
			if(instruction->flags & IRFlag_HasS) {
				if(instruction->s && instruction->w) {
					operand_data.flags |= OperandFlag_Signed;
				}
			}
		}

		if(instruction->flags & IRFlag_HasV) {
			assert(!operand_data.flags);
			operand_data.val = 1;
			if(instruction->v) {
				operand_data.flags |= (OperandFlag_Register | OperandFlag_8bits);
			} else {
				operand_data.flags |= OperandFlag_Immediate;
			}
		}


		if(operand_reg.flags && operand_data.flags) {  // immediate to register
			instruction->src = operand_data;
			instruction->dest = operand_reg;
		} else if(operand_rm.flags && operand_data.flags) {  // immediate to register/memory
			instruction->src = operand_data;
			instruction->dest = operand_rm;

			if(instruction->dest.flags & OperandFlag_Address) {
				if((!(instruction->flags & IRFlag_NoExplicitSizeNeeded) && !check_var_flags)
					|| (check_var_flags && !(instruction->var_flags & IRFlag_NoExplicitSizeNeeded))) {
					instruction->dest.flags |= OperandFlag_ExplicitSize;
				}
			}

		} else if(operand_reg.flags && operand_rm.flags) {
			instruction->src = operand_reg;
			instruction->dest = operand_rm;
		} else if(operand_reg.flags) {  // register
			instruction->dest = operand_reg;
		} else if(operand_rm.flags) {  // register/memory
			instruction->dest = operand_rm;

			if(instruction->dest.flags & OperandFlag_Address) {
				if((!(instruction->flags & IRFlag_NoExplicitSizeNeeded) && !check_var_flags)
					|| (check_var_flags && !(instruction->var_flags & IRFlag_NoExplicitSizeNeeded))) {
					instruction->dest.flags |= OperandFlag_ExplicitSize;
				}
			}

		} else if(operand_data.flags) {  // IP/IP Inc/Address
			instruction->dest = operand_data;

			if(instruction->dest.flags & OperandFlag_Address) {
				if((!(instruction->flags & IRFlag_NoExplicitSizeNeeded) && !check_var_flags)
					|| (check_var_flags && !(instruction->var_flags & IRFlag_NoExplicitSizeNeeded))) {
					instruction->dest.flags |= OperandFlag_ExplicitSize;
				}
			}

		}

		if((instruction->flags & IRFlag_HasD) && instruction->d) {
			Operand temp_op = instruction->src;
			instruction->src = instruction->dest;
			instruction->dest = temp_op;
		}
	}
	// -- Get source and destination


	return inc;
}

internalf IRArray asm_decode(u8* bytes, u32 count) {
  
	IRArray result = ir_array_alloc();
	IR instruction;
	u32 inc;
	
	u64 i;
	for(i = 0; i < count; i += inc) {
		inc = asm_decode_instruction(&instruction, &bytes[i]);
		ir_array_push_back(&result, instruction);
	}
  
  return result;
}
// ---- Decode

// ---- Disassembly
internalf String8 disasm_register(u8 reg, u8 w) {
	local_persist s8* register_names[8][2] = {
    { "al", "ax" },
    { "cl", "cx" },
    { "dl", "dx" },
    { "bl", "bx" },
    { "ah", "sp" },
    { "ch", "bp" },
    { "dh", "si" },
    { "bh", "di" }
  };
  
  return str8((u8*)register_names[reg][w], 2);
}

internalf String8 disasm_segreg(u8 segreg) {
	local_persist s8* segreg_names[4] = { "es", "cs", "ss", "ds" };

	return str8((u8*)segreg_names[segreg], 2);
}

internalf String8 disasm_ea_calc(Allocator* allocator, u8 rm, u8 w, u8 mod, s16 disp) {
	local_persist s8* ea_calc_strfs[8][3] = {
		{ "bx + si", "bx + si", "bx + si" },
		{ "bx + di", "bx + di", "bx + di" },
		{ "bp + si", "bp + si", "bp + si" },
		{ "bp + di", "bp + di", "bp + di" },
		{ "si", "si", "si" },
		{ "di", "di", "di" },
		{ "%d", "bp", "bp" },
		{ "bx", "bx", "bx" }
	};

	String8 result;

	if(mod == 3) {
		result = disasm_register(rm, w);
	} else if(mod == 0) {
		if(rm != 6) {
			result = str8_cstr((u8*)ea_calc_strfs[rm][0]);
		} else {
			result = str8f(allocator, ea_calc_strfs[6][0], (u16)disp);
		}
	} else {
		if(disp) {
			String8 ea_start = str8_cstr((u8*)ea_calc_strfs[rm][mod]);
			s8* fmt = " %c %d";
			String8 ea_end = str8_nil();
			if(disp < 0) {
				ea_end = str8f(allocator, fmt, '-', -1 * disp);
			} else {
				ea_end = str8f(allocator, fmt, '+', disp);
			}

			result = str8_cat(allocator, ea_start, ea_end);
		} else {
			result = str8_cstr((u8*)ea_calc_strfs[rm][mod]);
		}
	}

	return result;
}

internalf void disasm_instruction(IR instruction, b8 newline) {


	if(!bitset_is_set(instruction.flags, IRFlag_HasReg | IRFlag_RegIsPrefix | IRFlag_RegIsSegReg)) {
		ArenaTemp scratch = scratch_begin(0, 0);
		Allocator* allocator = &scratch.arena->allocator;
		String8 mnemonic_str = str8_nil();
		String8 dest_str = str8_nil();
		String8 src_str = str8_nil();
		String8 instruction_str = str8_nil();
		String8List instruction_str_list;
		String8Join instruction_str_join = { str8_nil_comp(), str8_nil_comp(), str8_nil_comp() };

		mnemonic_str = instruction.mnemonic;
			
		if(instruction.dest.flags) {
			if(instruction.dest.flags & OperandFlag_Cs) {
				dest_str = str8f(allocator, " %d:%d", instruction.dest.val, instruction.dest.disp);
			} else if(instruction.dest.flags & OperandFlag_Address) {
				String8 explicit_size;
			
				if(instruction.dest.flags & OperandFlag_Immediate) {
					if(instruction.dest.disp) {
					
						dest_str = str8f(allocator, "[%d + %d]", instruction.dest.val, instruction.dest.disp);
					} else {
						dest_str = str8f(allocator, "[%d]", instruction.dest.val);
					}
				} else {
					dest_str = str8f(allocator, "[%s]", (s8*)disasm_ea_calc(allocator, (u8)instruction.dest.val, instruction.w, instruction.mod, instruction.dest.disp).str);
				}

				if(seg_override_prefix) {
					dest_str = str8f(allocator, "%s:%s", disasm_segreg(seg_override_prefix-1).str, dest_str.str);
					seg_override_prefix = 0;
				}

				dest_str = str8_cat(allocator, str8_lit(" "), dest_str);

				if(instruction.dest.flags & OperandFlag_ExplicitSize) {
					if(instruction.w) {
						explicit_size = str8_lit(" word");
					} else {
						explicit_size = str8_lit(" byte");
					}
					dest_str = str8_cat(allocator, explicit_size, dest_str);
				}

			} else if(instruction.dest.flags & OperandFlag_Register) {
				if(instruction.dest.flags & OperandFlag_SegReg) {
					dest_str = str8f(allocator, " %s", (s8*)disasm_segreg((u8)instruction.dest.val).str);	
				} else {
					u8 temp_w = instruction.w;
					if(instruction.dest.flags & OperandFlag_16bits) {
						temp_w = 1;
					} else if(instruction.dest.flags & OperandFlag_8bits) {
						temp_w = 0;
					}
					dest_str = str8f(allocator, " %s", (s8*)disasm_register((u8)instruction.dest.val, temp_w).str);	
				}
			} else if(instruction.dest.flags & OperandFlag_OpInd) {
				dest_str = str8f(allocator, " %s", (s8*)ir_table[instruction.dest.val].mnemonic.str);
			} else if(instruction.dest.flags & OperandFlag_Immediate) {

				if(instruction.flags & IRFlag_DataIsIpInc) {
					if(instruction.dest.flags & OperandFlag_Signed) {  // signed 8-bit IP Inc
						s16 ip_inc = (s16)instruction.dest.val + 2;
						if(ip_inc < 0) {
							dest_str = str8f(allocator, " $%d+0", ip_inc);
						} else if(ip_inc > 0) {
							dest_str = str8f(allocator, " $+%d+0", ip_inc);
						} else {
							dest_str = str8_lit(" $+0");
						}
					} else {
						dest_str = str8f(allocator, " %d", instruction.dest.val);
					}
				} else {
					dest_str = str8f(allocator, " %d", (s16)instruction.dest.val);
				}
			}
		}

		if(instruction.src.flags) {
			if(instruction.src.flags & OperandFlag_Address) {
				if(instruction.src.flags & OperandFlag_Immediate) {
					if(instruction.src.disp) {
						src_str = str8f(allocator, "[%d + %d]", instruction.src.val, instruction.src.disp);
					} else {
						src_str = str8f(allocator, "[%d]", instruction.src.val);
					}
				} else {
					src_str = str8f(allocator, "[%s]", (s8*)disasm_ea_calc(allocator, (u8)instruction.src.val, instruction.w, instruction.mod, instruction.src.disp).str);
				}

				if(seg_override_prefix) {
					src_str = str8f(allocator, "%s:%s", disasm_segreg(seg_override_prefix-1).str, src_str.str);
					seg_override_prefix = 0;
				}

				src_str = str8_cat(allocator, str8_lit(", "), src_str);
			} else if(instruction.src.flags & OperandFlag_Register) {
				if(instruction.src.flags & OperandFlag_SegReg) {
					src_str = str8f(allocator, ", %s", (s8*)disasm_segreg((u8)instruction.src.val).str);	
				} else {
					u8 temp_w = instruction.w;
					if(instruction.src.flags & OperandFlag_16bits) {
						temp_w = 1;
					} else if(instruction.src.flags & OperandFlag_8bits) {
						temp_w = 0;
					}
					src_str = str8f(allocator, ", %s", (s8*)disasm_register((u8)instruction.src.val, temp_w).str);	
				}
			} else if(instruction.src.flags & OperandFlag_Immediate) {
				src_str = str8f(allocator, ", %d", (s16)instruction.src.val);
			}
		}

		if(instruction.flags & IRFlag_Prefix) {
			instruction_str_join.post = str8_lit(" ");
		} else if(newline) {
			instruction_str_join.post = str8_lit("\n");
		}


		// -- make final instructions string and output it
		str8_list_init(&instruction_str_list);
		str8_list_push(allocator, &instruction_str_list, mnemonic_str);
		str8_list_push(allocator, &instruction_str_list, dest_str);
		str8_list_push(allocator, &instruction_str_list, src_str);
		instruction_str = str8_join(allocator, &instruction_str_list, &instruction_str_join);
		printf("%s", instruction_str.str);
		// -- make final instructions string and output it

		scratch_end(&scratch);
	} else {
		seg_override_prefix = instruction.reg + 1;
	}
}


internalf void disasm(IRArray instructions) {

	IR* curr_instruction_ptr = (IR*)instructions.array->data;
	IR* final_instruction_ptr = curr_instruction_ptr + instructions.array->len;

	for(; curr_instruction_ptr != final_instruction_ptr; curr_instruction_ptr += 1) {
		disasm_instruction(*curr_instruction_ptr, 1);
	}
}
// ---- Disassembly

// ---- Execution
internalf String8 str8_substr_from_bitmask(Allocator* allocator, String8 str, u64 bitmask) {
	
	String8 result = str8_nil();
	u32 result_i;

	u64 len = 0;
	u32 bit_i;
	for(bit_i = 0; bit_i < str.len; bit_i += 1) {
		len += !!(bitmask & (bit((u8)bit_i)));
	}
	
	if(len) {
		result.str = (u8*)allocate(allocator, sizeof(u8) * (len + 1));
		result.len = len;
		result_i = 0;
		for(bit_i = 0; bit_i < str.len; bit_i += 1) {
			if(bitmask & (bit((u8)bit_i))) {
				result.str[result_i] = str.str[bit_i];
				result_i += 1;
			}
		}
	}
	
	return result;
}


internalf void set_flags_reg(u16 reg_val, u16 prev_reg_val, enum16(FlagsReg) flags) {

	if(flags & FlagsReg_Carry) {
		// TODO ...
	}

	if(flags & FlagsReg_Parity) {
		// TODO ...
	}
		
	if(flags & FlagsReg_Auxiliary) {
		// TODO ...
	}
		
	if(flags & FlagsReg_Zero) {
		if(reg_val == 0) {
			flags_reg = (u16)bitset_set(flags_reg, FlagsReg_Zero);
		} else {
			flags_reg = (u16)bitset_clear(flags_reg, FlagsReg_Zero);
		}
	}
		
	if(flags & FlagsReg_Sign) {
		if(reg_val & bit15) {
			flags_reg = (u16)bitset_set(flags_reg, FlagsReg_Sign);
		} else {
			flags_reg = (u16)bitset_clear(flags_reg, FlagsReg_Sign);
		}
	}
		
	if(flags & FlagsReg_Trap) {
		// TODO ...
	}
		
	if(flags & FlagsReg_Interrupt) {
		// TODO ...
	}
		
	if(flags & FlagsReg_Direction) {
		// TODO ...
	}
		
	if(flags & FlagsReg_Overflow) {
		// TODO ...
	}

}


internalf u32 pf_exec_ea_calc(u8 rm, u8 w, u8 mod, s16 disp, u32* clocks) {
	local_persist u8 ea_calc_table[8][2] = {
		{ 3, 6 },
		{ 3, 7 },
		{ 5, 6 },
		{ 5, 7 },
		{ 6, 0 },
		{ 7, 0 },
		{ 5, 0 },
		{ 3, 0 }
	};
	local_persist u32 ea_clocks_table[8][4] = {
		{ 7, 11, 11, 0 },
		{ 8, 12, 12, 0 },
		{ 8, 12, 12, 0 },
		{ 7, 11, 11, 0 },
		{ 5, 9, 9, 0 },
		{ 5, 9, 9, 0 },
		{ 6, 9, 9, 0 },
		{ 5, 9, 9, 0 }
	};

	u32 result = 0;

	u16 prefix;
	u16 address;

	if(seg_override_prefix) {
		prefix = segregs[seg_override_prefix];
	} else {
		prefix = segregs[3];
	}

	if((mod == 0) && (rm == 6)) {
		address = (u16)disp;
	} else {
		if(mod == 3) {
			address = registers[rm];
		} else {
			address = registers[ea_calc_table[rm][0]];

			if(ea_calc_table[rm][1]) {
				address += registers[ea_calc_table[rm][1]];
			}

			if(disp) {
				address += disp;
			}
		}
	}

	if(clocks) {
		*clocks = ea_clocks_table[rm][mod];
		if(!disp && (mod > 0 && mod < 3)) {
			*clocks = *clocks - 4;
		}
	}
	
	result = (u32)((prefix << 4) + address);
	return result;
}

internalf u32 calc_instruction_clocks(IR instruction, u32* transfers) {

	u32 result = 0;
	u32 local_transfers = 0;

	if(mem_match_cstr8_lit(instruction.mnemonic.str, "mov", sizeof("mov") - 1)) {
		if(instruction.dest.flags & OperandFlag_Register) {
			if(instruction.src.flags & OperandFlag_Register) {
				result = 2;
				local_transfers = 0;
			} else if(instruction.src.flags & OperandFlag_Address) {
				if(instruction.flags & (IRFlag_RegIsAccum | IRFlag_RmIsAccum)) {
					result = 10;
					local_transfers = 1;
				} else {
					result = 8;
					local_transfers = 1;
				}
			} else if(instruction.src.flags & OperandFlag_Immediate) {
				result = 4;
				local_transfers = 0;
			}
		} else if(instruction.dest.flags & OperandFlag_Address) {
			if(instruction.src.flags & OperandFlag_Register) {
				if(instruction.flags & (IRFlag_RegIsAccum | IRFlag_RmIsAccum)) {
					result = 10;
					local_transfers = 1;
				} else {
					result = 9;
					local_transfers = 1;
				}
			} else if(instruction.src.flags & OperandFlag_Immediate) {
				result = 10;
				local_transfers = 1;
			}
		}
	} else if(mem_match_cstr8_lit(instruction.mnemonic.str, "add", sizeof("add") - 1)) {
		if(instruction.dest.flags & OperandFlag_Register) {
			if(instruction.src.flags & OperandFlag_Register) {
				result = 3;
				local_transfers = 0;
			} else if(instruction.src.flags & OperandFlag_Address) {
				result = 9;
				local_transfers = 1;
			} else if(instruction.src.flags & OperandFlag_Immediate) {
				if(instruction.flags & IRFlag_RegIsAccum) {
					result = 4;
					local_transfers = 0;
				} else {
					result = 4;
					local_transfers = 0;
				}
			}
		} else if(instruction.dest.flags & OperandFlag_Address) {
			if(instruction.src.flags & OperandFlag_Register) {
				result = 16;
				local_transfers = 2;
			} else if(instruction.src.flags & OperandFlag_Immediate) {
				result = 17;
				local_transfers = 2;
			}
		}
	}

	if(transfers) {
		*transfers = local_transfers;
	}
	
	return result;
}

internalf void exec_instruction(IR instruction, b8 is_8086) {

	String8 dest_str = str8_nil();
	u16 prev_dest;

	u16* dest;
	u16* src;
	b8 word_op = (b8)instruction.w;
	u32 op_clocks;
	u32 ea_clocks = 0;
	u32 transfers = 0;
	u32 transfer_clocks = 0;

	op_clocks = calc_instruction_clocks(instruction, &transfers);
	if(instruction.dest.flags & OperandFlag_SegReg) {
		assert(word_op);
		dest = &segregs[instruction.dest.val];
		dest_str = disasm_segreg((u8)instruction.dest.val);
	} else if(instruction.dest.flags & OperandFlag_Register) {
		u16 temp_instruction_dest = instruction.dest.val;
		if(!word_op && (temp_instruction_dest > 3)) {
			temp_instruction_dest -= 4;
		}

		dest = &registers[temp_instruction_dest];
		dest_str = disasm_register((u8)temp_instruction_dest, 1);
	} else if(instruction.dest.flags & OperandFlag_Immediate) {
		if(instruction.flags & IRFlag_DataIsIpInc) {
			dest = &instruction.dest.val;
		}
	} else if(instruction.dest.flags & OperandFlag_Address) {
		u32 address = pf_exec_ea_calc((u8)instruction.dest.val, instruction.w, instruction.mod, instruction.dest.disp, &ea_clocks);
		dest = (u16*)&memory[address];

		if((instruction.flags & IRFlag_HasW) && word_op) {
			if(is_8086) {
				if(address & bit0) {
					transfer_clocks = transfers * 4;
				}
			} else {
				transfer_clocks = transfers * 4;
			}
		}
	}

	if(instruction.src.flags & OperandFlag_SegReg) {
		assert(word_op);
		src = &segregs[instruction.src.val];
	} else if(instruction.src.flags & OperandFlag_Register) {
		u16 temp_instruction_src = instruction.src.val;
		if(!word_op && (temp_instruction_src > 3)) {
			temp_instruction_src -= 4;
		}

		src = &registers[temp_instruction_src];
	} else if(instruction.src.flags & OperandFlag_Immediate) {
		src = &instruction.src.val;
	} else if(instruction.src.flags & OperandFlag_Address) {
		u32 address = pf_exec_ea_calc((u8)instruction.src.val, instruction.w, instruction.mod, instruction.src.disp, &ea_clocks);
		src = (u16*)&memory[address];

		if((instruction.flags & IRFlag_HasW) && word_op) {
			if(is_8086) {
				if(address & bit0) {
					transfer_clocks = transfers * 4;
				}
			} else {
				transfer_clocks = transfers * 4;
			}
		}
	}

	prev_dest = *dest;
	if(instruction.flags & IRFlag_HasW) {
		if(instruction.w) {
			if(mem_match_cstr8_lit(instruction.mnemonic.str, "mov", sizeof("mov") - 1)) {
				*dest = *src;
			} else if(mem_match_cstr8_lit(instruction.mnemonic.str, "add", sizeof("add") - 1)) {
				*dest += *src;
				set_flags_reg(*dest, prev_dest, FlagsReg_Zero | FlagsReg_Sign);
			} else if(mem_match_cstr8_lit(instruction.mnemonic.str, "sub", sizeof("sub") - 1)) {
				*dest -= *src;
				set_flags_reg(*dest, prev_dest, FlagsReg_Zero | FlagsReg_Sign);
			} else if(mem_match_cstr8_lit(instruction.mnemonic.str, "cmp", sizeof("cmp") - 1)) {
				u16 cmp_val = *dest - *src;
				set_flags_reg(cmp_val, prev_dest, FlagsReg_Zero | FlagsReg_Sign);
			}
		} else {
			u8* bit_dest = (u8*)dest;
			u8* bit_src = (u8*)src;

			if(instruction.dest.flags & OperandFlag_Address) {
			} else {
				dest += (u64)(instruction.dest.val > 3);
			}

			if(instruction.src.flags & OperandFlag_Immediate) {
			} else {
				bit_src += (u64)(instruction.src.val > 3);
			}
		
			if(mem_match_cstr8_lit(instruction.mnemonic.str, "mov", sizeof("mov") - 1)) {
				*bit_dest = *bit_src;
			} else if(mem_match_cstr8_lit(instruction.mnemonic.str, "add", sizeof("add") - 1)) {
				*bit_dest += *bit_src;
				set_flags_reg(*dest, prev_dest, FlagsReg_Zero | FlagsReg_Sign);
			} else if(mem_match_cstr8_lit(instruction.mnemonic.str, "sub", sizeof("sub") - 1)) {
				*bit_dest -= *bit_src;
				set_flags_reg(*dest, prev_dest, FlagsReg_Zero | FlagsReg_Sign);
			} else if(mem_match_cstr8_lit(instruction.mnemonic.str, "cmp", sizeof("cmp") - 1)) {
				u8 cmp_val = *bit_dest - *bit_src;
				set_flags_reg((u16)sign_extend_u8(cmp_val), prev_dest, FlagsReg_Zero | FlagsReg_Sign);
			}
		}
	} else {
		if(mem_match_cstr8_lit(instruction.mnemonic.str, "jne", sizeof("jne") - 1)) {
			if(!(flags_reg & FlagsReg_Zero)) {
				ip_reg += (s16)(*dest);
			}
		} else if(mem_match_cstr8_lit(instruction.mnemonic.str, "jp", sizeof("jp") - 1)) {
			// TODO ...
		} else if(mem_match_cstr8_lit(instruction.mnemonic.str, "jb", sizeof("jb") - 1)) {
			// TODO ...
		} else if(mem_match_cstr8_lit(instruction.mnemonic.str, "loopne", sizeof("loopne") - 1)) {
			registers[1] -= 1;
			if((registers[1] != 0) && (flags_reg & FlagsReg_Zero)) {
				ip_reg += (s16)(*dest);
			}
		} else if(mem_match_cstr8_lit(instruction.mnemonic.str, "loop", sizeof("loop") - 1)) {
			registers[1] -= 1;
			if(registers[1] != 0) {
				ip_reg += (s16)(*dest);
			}
		}
	}

	{
		ArenaTemp scratch = scratch_begin(0, 0);
		String8 print_str = str8_nil();

		s8 machine_str[5];
		machine_str[4] = '\0';
		if(is_8086) {
			mem_cpy(machine_str, "8086", 4 * sizeof(u8));
		} else {
			mem_cpy(machine_str, "8088", 4 * sizeof(u8));
		}

		clocks_total += op_clocks + ea_clocks + transfer_clocks;
		if(dest_str.str) {
			print_str = str8f(&scratch.arena->allocator, "%s:0x%x->0x%x ; clocks %s: +%d=%d (%d + %dea + %dp) ; ip: 0x%x ; flags: %s", dest_str.str, prev_dest, *dest, 
												machine_str, op_clocks + ea_clocks + transfer_clocks, clocks_total, op_clocks, ea_clocks, transfer_clocks, ip_reg, 
												str8_substr_from_bitmask(&scratch.arena->allocator, flags_reg_str, flags_reg).str);
		} else {
			print_str = str8f(&scratch.arena->allocator, " ; clocks %s: +%d=%d (%d + %dea + %dp) ; ip:0x%x ; flags: %s", 
												machine_str, op_clocks + ea_clocks + transfer_clocks, clocks_total, op_clocks, ea_clocks, transfer_clocks, ip_reg, 
												str8_substr_from_bitmask(&scratch.arena->allocator, flags_reg_str, flags_reg).str);
		}

		printf("%s\n", print_str.str);
		
		scratch_end(&scratch);
	}
}

internalf void print_registers(void) {

	ArenaTemp scratch = scratch_begin(0, 0);

	u32 register_i;
	printf("Final Registers:\n");
	for(register_i = 0; register_i < REGISTER_N; register_i += 1) {
		printf("\t%s: 0x%04x (%d)\n", disasm_register((u8)register_i, 1).str, registers[register_i], registers[register_i]);
	}
	
	printf("\nFinal Segment Registers:\n");
	for(register_i = 0; register_i < SEGREG_N; register_i += 1) {
		printf("\t%s: 0x%04x (%d)\n", disasm_segreg((u8)register_i).str, segregs[register_i], segregs[register_i]);
	}

	printf("\nIP Register: 0x%04x (%d)\n", ip_reg, ip_reg);

	printf("\nFLAGS: %s\n", str8_substr_from_bitmask(&scratch.arena->allocator, flags_reg_str, flags_reg).str);

	printf("\n");

	scratch_end(&scratch);
}
// ---- Execution

// -- Functions

internalf void entry_point_udf(void) {

	File asm_file;

	String8 assembled_file_path = cmd_line_args_get_value(str8_lit("-disasm8086"), 0);
	b8 disasm = str8_is_valid(assembled_file_path);
	b8 exec = cmd_line_arg_is_set(str8_lit("-exec8086"));
	
	assert(disasm);

	ir_instructions_init();

	{
		ArenaParams params = arena_params_default_static();
		Arena* file_arena = arena_allocate(&params);
		asm_file = file_load(&file_arena->allocator, assembled_file_path);
	}

	{
		IR instruction;
		
		u8* curr_byte = (u8*)asm_file.content;
		u8* final_byte = curr_byte + asm_file.info.size;

		// -- Instruction Decoding
		for(; curr_byte != final_byte; curr_byte = (u8*)asm_file.content + ip_reg) {
			ip_reg += asm_decode_instruction(&instruction, curr_byte);
			disasm_instruction(instruction, !exec);

			if(exec) {
				printf(" ; ");
				exec_instruction(instruction, 0);
			}
		}
		// -- Instruction Decoding

		if(exec) {
			print_registers();
		}
	}
}

