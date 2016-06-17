/* Generated automatically by the program `genconstants'
   from the machine description file `md'.  */

#ifndef GCC_INSN_CONSTANTS_H
#define GCC_INSN_CONSTANTS_H

#define RETURN_ADDR_REGNUM 1
#define T1_REGNUM 6
#define T0_REGNUM 5

enum unspec {
  UNSPEC_LOAD_LOW = 0,
  UNSPEC_LOAD_HIGH = 1,
  UNSPEC_STORE_WORD = 2,
  UNSPEC_EH_RETURN = 3,
  UNSPEC_ADDRESS_FIRST = 4,
  UNSPEC_LOAD_GOT = 5,
  UNSPEC_TLS = 6,
  UNSPEC_TLS_LE = 7,
  UNSPEC_TLS_IE = 8,
  UNSPEC_TLS_GD = 9,
  UNSPEC_GPR_SAVE = 10,
  UNSPEC_GPR_RESTORE = 11,
  UNSPEC_BLOCKAGE = 12,
  UNSPEC_FENCE = 13,
  UNSPEC_FENCE_I = 14,
  UNSPEC_COMPARE_AND_SWAP = 15,
  UNSPEC_SYNC_OLD_OP = 16,
  UNSPEC_SYNC_EXCHANGE = 17,
  UNSPEC_ATOMIC_STORE = 18,
  UNSPEC_MEMORY_BARRIER = 19
};
#define NUM_UNSPEC_VALUES 20
extern const char *const unspec_strings[];

#endif /* GCC_INSN_CONSTANTS_H */
