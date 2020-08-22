// Copyright 2020 XGSTeam, NaCaTeam
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef NACA_POW_H
#define NACA_POW_H

#include <arith_uint256.h>

#define NB_BYTES_DG 28

void nacapow(const char*, char*);

uint256 swap_bits(arith_uint256, unsigned int, unsigned int);

uint256 permute_bits(const uint256&, uint32_t);

// The NacaEntry has to be 256 bits
struct NacaEntry{
  unsigned char dg[NB_BYTES_DG]; 
  short sd1;
  short sd2;
};

struct NacaOutput{
  float xcomp[7];
  uint32_t subhash;
};

#endif // NACA_POW_H
