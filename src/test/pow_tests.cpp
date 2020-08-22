// Copyright (c) 2015-2018 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chain.h>
#include <chainparams.h>
#include <validation.h>
#include <pow.h>
#include <random.h>
#include <util.h>
#include <nacapow/nacapow.h>
#include <test/test_bitcoin.h>

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(pow_tests, BasicTestingSetup)

/* Test calculation of next difficulty target with no constraints applying */

// What to implement:
// 1. Generate 2016 blocks
// 2. Then, lets use. Case A:  c(seq(0.3 * 60, 0.4 * 60, length=2013), 0.499, 0.5 * 60, 0.5 * 60) for the nTime of blocks
// The target would be c(seq(5284900, 6284900, length=2013), 3284901, 3284900, 3284900)
// Case A, the found target value should be 3284901


BOOST_AUTO_TEST_CASE(get_next_work_case_a)
{
  const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
  uint64_t nTimes[2016];
  uint32_t targets[2016];
  double delta_time = (0.4 - 0.3) * 60. / 2012.;
  double delta_target = (6284900. - 5284900.) / 2012.;
  unsigned int counter = 0;
  for (int i=0; i < 2016; i++)
  {
    if (i < 2012)
    {
      nTimes[i] = (uint64_t)(delta_time * i + (0.3 * 60)) ;
      targets[i] = delta_target * i + 5284900;
    }
    else if (i == 2013)
    {
      nTimes[i] = (uint64_t)(0.499 * 60);
      targets[i] = 3284901;
    }
    else
    {
      nTimes[i] = (uint64_t)(0.5 * 60);
      targets[i] = 3284900;
    }
  }
  // Initialize the chain with a Genesis block
  CBlockIndex* genesis = new CBlockIndex();
  genesis->pprev = nullptr;
  genesis->nBits = (uint32_t)526378596;  
  genesis->nTime = 1566247964 + nTimes[0];
  chainActive.SetTip(genesis);
  // arith_uint256 biggestTarget = arith_uint256(
      // "0x005FE664FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
  while (chainActive.Tip()->nHeight < 2015) {
    CBlockIndex* prev = chainActive.Tip();
    CBlockIndex* next = new CBlockIndex();
    NacaOutput output;
    output.subhash = targets[counter];
    for (unsigned int i = 0; i < 7; i++)
    {
      output.xcomp[i] = 1.5 + i;
      //output.xcomp[i] = 0.; 
    }
    unsigned char* output_ch = (unsigned char*)(&output);
    std::vector<unsigned char> vec(output_ch, output_ch + sizeof(output));
    uint256 uint_out = uint256(vec);
    next->pprev = prev;
    next->pprev->phashBlock = &uint_out;
    next->nHeight = prev->nHeight + 1;
    next->nBits = 526378596;  
    next->nTime = prev->nTime + nTimes[counter]; 
    //next->BuildSkip();
    chainActive.SetTip(next);
    counter += 1;
  }
  BOOST_CHECK_EQUAL(chainActive.Tip()->nHeight, 2015);
  unsigned int calculatedNBits;
  arith_uint256 computedTarget;
  CBlockIndex* pindexLast = chainActive.Tip();
  calculatedNBits = CalculateNextWorkRequired(pindexLast, genesis->GetBlockTime(), chainParams->GetConsensus()); 
  printf("nBits adjusted: %u\n", calculatedNBits);
  //computedTarget.SetCompact(calculatedNBits);
  //printf("nBits original value: %u\n", genesis->nBits);
  //printf("nBits adjusted value AFTER: %u\n", calculatedNBits);
  //printf("Corresponing target is: %s\n", computedTarget.ToString().c_str());
  // Here check that the new nBits value computed has decreased (make it easier to generated a block) 
  BOOST_CHECK(calculatedNBits < genesis->nBits);
}

bool TimeComp(const uint64_t& i, const uint64_t& j) { return(i < j);}
BOOST_AUTO_TEST_CASE(get_next_work_case_b)
{
// For case B:
// Then, lets use. Case B:  c(0.566 * 60, 0.567 * 60, 0.569 * 60, seq(0.6 * 60, 0.8 * 60, length=2013)) for the nTime of blocks
// The target would be c(seq(1284900, 2284900, length=2013), 3284901, 3284902, 3284900)
// For case B, the target value should be 3284900 * 1.2 (nBits with GetCompact() to be checked)
// 3. Each block have the genesis nBits value
  const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
  uint64_t nTimes[2016];
  uint32_t targets[2016];
  double delta_time = (0.8 - 0.6) * 60. / 2012.;
  double delta_target = (2284900. - 1284900.) / 2012.;
  unsigned int counter = 0;
  for (int i=0; i < 2016; i++)
  {
    if (i < 2012)
    {
      nTimes[i] = (uint64_t)(delta_time * i + (0.6 * 60)) ;
      targets[i] = delta_target * i + 1284900;
    }
    else if (i == 2013)
    {
      nTimes[i] = (uint64_t)(0.566 * 60);
      targets[i] = 3284901;
    }
    else if (i == 2014)
    {
      nTimes[i] = (uint64_t)(0.567 * 60);
      targets[i] = 3284902;
    } else
    {
      nTimes[i] = (uint64_t)(0.569 * 60);
      targets[i] = 3284900;
    }
  }
  // Reordering nTimes
  std::vector<uint64_t>nTimesVec(nTimes, std::end(nTimes));
  //printf("nTimes generated - First: %u - Last: %u\n", nTimesVec[0], nTimesVec[10]);
  std::sort(nTimesVec.begin(), nTimesVec.end(), TimeComp);

  // Initialize the chain with a Genesis block
  CBlockIndex* genesis = new CBlockIndex();
  genesis->pprev = nullptr;
  genesis->nBits = (uint32_t)523378598;  
  genesis->nTime = 1566247964 + nTimesVec[0];
  chainActive.SetTip(genesis);
  while (chainActive.Tip()->nHeight < 2015) {
    CBlockIndex* prev = chainActive.Tip();
    CBlockIndex* next = new CBlockIndex();
    NacaOutput output;
    output.subhash = targets[counter];
    for (unsigned int i = 0; i < 7; i++)
    {
      output.xcomp[i] = 1.5 + i;
    }
    unsigned char* output_ch = (unsigned char*)(&output);
    std::vector<unsigned char> vec(output_ch, output_ch + sizeof(output));
    uint256 uint_out = uint256(vec);
    next->pprev = prev;
    next->pprev->phashBlock = &uint_out;
    next->nHeight = prev->nHeight + 1;
    next->nBits = 523378598;  
    next->nTime = prev->nTime + nTimesVec[counter]; 
    //next->BuildSkip();
    chainActive.SetTip(next);
    counter += 1;
  }
  BOOST_CHECK_EQUAL(chainActive.Tip()->nHeight, 2015);
  unsigned int calculatedNBits;
  arith_uint256 computedTarget;
  CBlockIndex* pindexLast = chainActive.Tip();
  calculatedNBits = CalculateNextWorkRequired(pindexLast, genesis->GetBlockTime(), chainParams->GetConsensus()); 
  printf("nBits adjusted value BEFORE: %u\n", calculatedNBits);
  //computedTarget.SetCompact(calculatedNBits);
  //printf("nBits original value: %u\n", genesis->nBits);
  //printf("nBits adjusted value AFTER: %u\n", calculatedNBits);
  // Here check that the new nBits value computed has increased (make it harder to generated a block) 
  BOOST_CHECK(calculatedNBits > genesis->nBits);
}


BOOST_AUTO_TEST_CASE(GetBlockProofEquivalentTime_test)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    std::vector<CBlockIndex> blocks(10000);
    for (int i = 0; i < 10000; i++) {
        blocks[i].pprev = i ? &blocks[i - 1] : nullptr;
        blocks[i].nHeight = i;
        blocks[i].nTime = 1269211443 + i * chainParams->GetConsensus().nPowTargetSpacing;
        blocks[i].nBits = 0x207fffff; /* target 0x7fffff000... */
        blocks[i].nChainWork = i ? blocks[i - 1].nChainWork + GetBlockProof(blocks[i - 1]) : arith_uint256(0);
    }

    for (int j = 0; j < 1000; j++) {
        CBlockIndex *p1 = &blocks[InsecureRandRange(10000)];
        CBlockIndex *p2 = &blocks[InsecureRandRange(10000)];
        CBlockIndex *p3 = &blocks[InsecureRandRange(10000)];

        int64_t tdiff = GetBlockProofEquivalentTime(*p1, *p2, *p3, chainParams->GetConsensus());
        BOOST_CHECK_EQUAL(tdiff, p1->GetBlockTime() - p2->GetBlockTime());
    }
}

BOOST_AUTO_TEST_SUITE_END()
