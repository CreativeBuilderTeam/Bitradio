// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
// Ref: syn

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (0,	Params().HashGenesisBlock() )
        (1,	uint256("0xa4d48b199c05f7634d72bb8d72cb32ee1c1f51ddf7ff9570224b5d0cb3d74f68"))
	      (100, uint256("0x6fee5258583c9544a00d957ee82a3dd8e9590df90c03aa067be4c7c3e6034811"))
	      (1000, uint256("0x685987496a95c9d594cb1f7e8d76f9810fd8c04e6bfb3aea74f0f27185036fc2"))
	      (2000, uint256("0xc2261a03513256758ea1a0bd46fa1b20c64628340e2ded229c249f64f7033d56"))
	      (3000, uint256("0x5f393ae0e175c3bbe433a23199615d6f289758e1032e225a7260385363b3957b"))
        (5000, uint256("0x31e1e38572fec991c34f9d00a8f377239b8eeef7963415a82e60e48a545568ec"))
	      (6000, uint256("0xb372d87113aed293da896b3d83d805ab20c48ada7c84182aa71e78d6d5914e19"))
        (10000, uint256 ("0xeaf3ff28c2ddc93e6e3a48974abf4296bf500c21ed8ebbe26770797c5a1cee58"))
        (100000, uint256("0xfa9df7c6a65fb3e98511a5bdaad1d7cb11f84f0eebddbd6593c75d141e4a5927"))
	      (110000, uint256("0x15b0d4df570f252e6dad6e998322180176628775b6bed32aaf01f10f5d99c75a"))
        (111000, uint256("0x677def7ec4f1061a14c29a47e1f19e54e99fecddf789827a8475da58fc6164ef"))
	      (120000, uint256("0x2fc118b15104bb5e3bd8e7e2cfc9c23ba9f5f8b3144acae08e07fcb936c4c4d5"))
      	(120010, uint256("0x65a0656a31ba1dbcc02a21acfb9404ec3fd3f3dd345ac048377ed7582ca9823d"))
        (300000, uint256("0x6d6913c2511cd0ba85aa453c8eb9ce3a55edbf52d973bd8162d2e6f1b839e252"))
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
