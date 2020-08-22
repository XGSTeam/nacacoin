/*
 * Copyright 2020 XGSTeam, NaCaTeam
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
#include <boost/random.hpp>
#include <float.h>
#include <utilstrencodings.h>
#include <primitives/block.h>
#include <arith_uint256.h>
#include <uint256.h>
#include <util.h>
#include <cmath>
#include <nacapow/nacapow.h>
#include <nacapow/relax_callFunc.h>

// This will be dynamic among time and concensus parameters
#define MAX_NB_COMPONENTS   250 * 3
#define NB_OUTPUT_XCOMP     7

void nacapow(const char* input, char* output)
{
  CBlockHeader* pHeader;
  pHeader = (CBlockHeader*)input;

  uint32_t seed = pHeader->nNonce;
  uint256 iinput = pHeader->hashMerkleRoot;

  boost::random::mt19937 rng;
  
  if (UintToArith256(iinput) == 0)
  {
    boost::random::uniform_int_distribution<> dist(0, 512); 
    rng.seed(seed);
    unsigned char randbuf[32];
    for (int i = 0; i < 32; i++)
    {
      randbuf[i] = (char)(dist(rng));
    }
    std::vector<unsigned char> vec(randbuf, randbuf + sizeof(randbuf));
    iinput = uint256(vec); 
  }
  // Randomly permute the bits of the input using nonce value
  uint256 perm_output = permute_bits(iinput, seed);
  //printf("Permuted input: %s\n", UintToArith256(perm_output).ToString().c_str());

  double components[MAX_NB_COMPONENTS];
  const unsigned char* buffer = perm_output.begin();
  NacaEntry* pNacaEntry = (NacaEntry*)buffer;

  // Generate components based on seeding extracted from buffer
  unsigned int index = 0; 
  for (int i=0; i < NB_BYTES_DG; i++)
  {
    // Generate few components with the current seed value
    //printf("For seeding #%d int value generate: %d\n", i, (unsigned int)(pNacaEntry->dg[i]));
    rng.seed((unsigned int)(pNacaEntry->dg[i]));
    boost::random::uniform_01<boost::random::mt19937> uni_dist(rng);
    int nb_comp = MAX_NB_COMPONENTS / NB_BYTES_DG + 1;
    for (int j=0; j < nb_comp && index < MAX_NB_COMPONENTS; j++)
    {
      components[index] = uni_dist(); 
      index += 1; 
    }
  }
  // Compute idum value with sd1 and sd2 from Naca entry
  int32_t idum = (unsigned int)pNacaEntry->sd1 + (unsigned int)pNacaEntry->sd2;
  if (idum > 0) idum = -idum;
  int32_t nTime = (int32_t)pHeader->nTime;
  int run_ls = 0;
  double nrj = DBL_MAX;
  int32_t spare = 1e6;
  int32_t spare2 = 0;

  #ifdef __APPLE__
  naca_((long int*)&nTime, (long int*)&idum, (long int*)&run_ls, components, &nrj, (long int*)&spare, (long int*)&spare2);
  #else
  naca_((int*)&nTime, (int*)&idum, (int*)&run_ls, components, &nrj, (int*)&spare, (int*)&spare2);
  #endif

  // Fill the output buffer with resuls of the computation as a 256 hash value
  NacaOutput naca_output;
  naca_output.subhash = (uint32_t)spare;
  for(unsigned int i = 0; i < NB_OUTPUT_XCOMP; i++)
  {
    naca_output.xcomp[i] = (int)(components[i] * 1.e4) / 1.e4;
  }
  memcpy(output, &naca_output, sizeof(NacaOutput));
  arith_uint256 target;
  target.SetCompact(pHeader->nBits);
  LogPrint(BCLog::BENCH, "NACA DONE - nBits: %#08x - Target: %s", pHeader->nBits, target.ToString().c_str());
}

uint256 swap_bits(arith_uint256 n, unsigned int p1, unsigned int p2)
{
    arith_uint256 b1;
    arith_uint256 b2;
    /* Move p1'th to rightmost side */
    b1 =  (n >> p1) & 1;

    /* Move p2'th to rightmost side */
    b2 =  (n >> p2) & 1;

    /* XOR the two bits */
    arith_uint256 x = (b1 ^ b2);

    /* Put the xor bit back to their original positions */
    x = (x << p1) | (x << p2);

    /* XOR 'x' with the original number so that the
       two sets are swapped */
    arith_uint256 result = n ^ x;
    return ArithToUint256(result);
}

// Permutation implementation using the Fisher–Yates shuffle
// Check for any buffer size here
uint256 permute_bits(const uint256& input, uint32_t seed)
{
    boost::random::mt19937 rng;
    rng.seed(seed);
    uint256 output(input);
    arith_uint256 arith_out = UintToArith256(output);

    for (uint32_t i = 256; i > 0; i--)
    {
        boost::random::uniform_int_distribution<> perm_range(i, 256);
        uint32_t j = (uint32_t)(perm_range(rng));
        output = swap_bits(arith_out, (uint32_t)j, (uint32_t)(i-1));
        arith_out = UintToArith256(output);
    }
    return ArithToUint256(arith_out);
}
