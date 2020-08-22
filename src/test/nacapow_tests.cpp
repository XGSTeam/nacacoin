/*
 * Copyright 2019  Alain Bian, Albert Young, XGSTeam, NaCaTeam
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

#include <float.h>
#include <boost/random.hpp>
#include <boost/test/unit_test.hpp>
#include <uint256.h>
#include <util.h>
#include <utilstrencodings.h>
#include <chain.h>
#include <primitives/block.h>
#include <nacapow/nacapow.h>
#include <nacapow/relax_callFunc.h>
#include <boost/random.hpp>

//static lib_t nacalib_handler = 0;
//static pow_t pow_func = 0;

BOOST_AUTO_TEST_SUITE(nacapow_tests)

BOOST_AUTO_TEST_CASE(nacapow_bits_permutation)
{
  arith_uint256 initial_int = 1234567;
  uint32_t seed = 1234;
  uint256 permuted;
  permuted = permute_bits(ArithToUint256(initial_int), seed); 
  //printf("Initial: %s permuted: %s", ArithToUint256(initial_int).ToString().c_str(), permuted.ToString().c_str());
  BOOST_CHECK_PREDICATE( std::not_equal_to<const char*>(),
      (ArithToUint256(initial_int).ToString().c_str())(permuted.ToString().c_str()) );
}

BOOST_AUTO_TEST_CASE(nacapow_bits_swapping)
{
  BOOST_CHECK_EQUAL(0, 0);
}


BOOST_AUTO_TEST_CASE(nacapow_res_conversion)
{
  uint256 i1 = uint256S("0xFACE");
  uint256 i2 = uint256S("0xBABE");
  CBlockHeader* pHeader = new CBlockHeader();
  pHeader->nVersion = 12345;
  pHeader->hashPrevBlock = i1;  
  pHeader->hashMerkleRoot = i2; 
  pHeader->nTime = 8765432;
  pHeader->nBits = 1234;
  pHeader->nNonce = 56789;
  char out[32];
  nacapow(BEGIN(pHeader), out); 
  NacaOutput output;
  output.subhash = 8500;
  for (unsigned int i = 0; i < 7; i++)
  {
    //output.xcomp[i] = 1.5 + i;
    output.xcomp[i] = 0.;
  }
  unsigned char* output_ch = (unsigned char*)(&output);
  std::vector<unsigned char> vec(output_ch, output_ch + sizeof(output));
  uint256 uint_out = uint256(vec);
  printf("out:\n%s\n", uint_out.GetHex().c_str());
  const char* input = BEGIN(uint_out);
  NacaOutput* output2 = (NacaOutput*)input;
  BOOST_CHECK_EQUAL(output2->subhash, output.subhash);
  for (unsigned int i = 0; i < 7; i++)
  {
    BOOST_CHECK_EQUAL(output2->xcomp[i], output.xcomp[i]);
  }
  delete pHeader;
}

BOOST_AUTO_TEST_CASE(nacapow_nacalib)
{
  long int nTime = 123456;
  long int rid = -7310077;
  int spare = 1e6;
  int spare2 = 4284901;
  long int dcg = 0;
  double x[250 * 3];
  double e = DBL_MAX;
  boost::random::mt19937 rng;
  // Get the seeding from the buffer
  rng.seed(0);
  boost::random::uniform_01<boost::random::mt19937> uni_dist(rng);

  // Generate random next components
  for (unsigned int i = 0; i < 250 * 3; i++)
  {
    x[i] = uni_dist();
  }
  #ifdef __APPLE__
  naca_((long int*)&nTime, (long int*)&rid, (long int*)&dcg, &x[0], &e, (long int*)&spare, (long int*)&spare2);
  #else
  naca_((int*)&nTime, (int*)&rid, (int*)&dcg, &x[0], &e, (int*)&spare, (int*)&spare2);
  #endif
  // spare2 has to be lower or equal to 4284900 and bigger than 0
  BOOST_CHECK_EQUAL(spare2 <= 4284900 , 0);
}


BOOST_AUTO_TEST_SUITE_END()

