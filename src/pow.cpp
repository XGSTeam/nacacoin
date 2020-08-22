// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>
#include <util.h>
#include <algorithm>

bool TargetComp(const arith_uint256& i, const arith_uint256& j) { return(i > j);}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
            {
                LogPrint(BCLog::BENCH, "[block: %d] Using min difficulty: 0x%08x\n", pindexLast->nHeight, nProofOfWorkLimit);
                return nProofOfWorkLimit;
            }
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        LogPrint(BCLog::BENCH, "Using nBits value of the last block: 0x%08x\n", pindexLast->nBits);
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    // NaCacoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = params.DifficultyAdjustmentInterval()-1;
    if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentInterval())
        blockstogoback = params.DifficultyAdjustmentInterval();

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;

    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

// Modified from the original litecoin logic, the target/difficulty is changing linearly with the POW
// So, when difficulty is adjusted, it has to be done based on the nbits values and block time
// using the 2016 last blocks
unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
  // Doing the actual adjustment because we have more than 2016 blocks available
  double timeIntervalExpected = 0.5 * 60; // 30 seconds per block
  double totalTimeSpan = 0.0;
  std::vector<arith_uint256> targets(params.DifficultyAdjustmentInterval());
  std::vector<double> timePoints(params.DifficultyAdjustmentInterval());
  totalTimeSpan = pindexLast->nTime - nFirstBlockTime;
  LogPrint(BCLog::BENCH, "*************************************************\n");
  LogPrint(BCLog::BENCH, "ADJUSTING DIFFICULTY - totalTimeSpan: %f\n", totalTimeSpan);
  // Retrieve targets for the last blocks in the interval
  // and compute total time span for the last 2016 blocks
  for (unsigned int i = 0; i < params.DifficultyAdjustmentInterval(); i++)
  {
    LogPrint(BCLog::BENCH, "i: %d heigth: %d block hash: %s\n", i, pindexLast->nHeight, pindexLast->phashBlock->ToString().c_str());
    const uint256* phash = pindexLast->phashBlock;
    assert(phash != nullptr);
    uint256 powRes = pindexLast->GetBlockPoWHash();
    targets[i] = UintToArith256(powRes);
    pindexLast = pindexLast->pprev;
  }
  // Sort the target vector in decreasing order (from the most
  // easy to the most difficult target)
  std::sort(targets.begin(), targets.end(), TargetComp);

  // Generate linear time points vector based on total time spent for 2016 blocks
  for (unsigned int i = 0; i < params.DifficultyAdjustmentInterval(); i++)
  {
    timePoints[i] = totalTimeSpan / (params.DifficultyAdjustmentInterval() - i);
  }

  // Computing the right target
  arith_uint256 rightTarget;
  unsigned int i = 0;
  LogPrint(BCLog::BENCH, "timeIntervalExpected: %f time(0): %f time(end): %f\n", timeIntervalExpected, timePoints[0], timePoints.back());
  LogPrint(BCLog::BENCH, "Target(0): %s Target(%d): %s\n", targets[0].ToString().c_str(), targets.size() - 1, targets.back().ToString().c_str());
  if (timeIntervalExpected <= timePoints[0])
  {
    arith_uint256 coefficient = arith_uint256((uint64_t)(101));
    arith_uint256 divider = arith_uint256((uint64_t)(100));
    rightTarget = targets[0] * coefficient;
    rightTarget = rightTarget / divider;
    LogPrint(BCLog::BENCH, "================ TOO HARD ================\n");
    LogPrint(BCLog::BENCH, "Adjusted target will be: %s\n", rightTarget.ToString().c_str());
    LogPrint(BCLog::BENCH, "Corrected nBits is: %#08x\n", rightTarget.GetCompact());
    return rightTarget.GetCompact();
  }
  for(i = 0; i < params.DifficultyAdjustmentInterval() - 1 ; i++)
  {
    if ((timeIntervalExpected >= timePoints[i]) && (timeIntervalExpected <= timePoints[i + 1]))
    {
      break;
    }
  }
  if (i==params.DifficultyAdjustmentInterval()-1)
  {
    rightTarget = targets[i-2];
  } else
  {
    rightTarget = targets[i];
  }
  LogPrint(BCLog::BENCH, "================ TOO EASY ================\n");
  LogPrint(BCLog::BENCH, "Adjusted target will be: %s\n", rightTarget.ToString().c_str());
  LogPrint(BCLog::BENCH, "Corrected nBits is: %#08x\n", rightTarget.GetCompact());
  return rightTarget.GetCompact();
}


bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
    {
        // LogPrintf("%s: FAILED-1 fNegative: %d fOverflow: %d Target is: %s Hash is: %s nBits is: %u\n", __func__, fNegative, fOverflow, ArithToUint256(bnTarget).ToString().c_str(),
            // hash.ToString().c_str(), nBits);
        return false;
    }

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
    {
        // LogPrintf("%s: FAILED-2 Target is: %s Hash is: %s nBits is: %u\n", __func__, ArithToUint256(bnTarget).ToString().c_str(),
            // hash.ToString().c_str(), nBits);
        return false;
    }

    return true;
}
