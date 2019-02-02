// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

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
        (       0, uint256("0x0000003f3059ea857d194fd8f3ca5ada1d274d749410d08fdfb2ab4dff0a8419") )
        (       1, uint256("0x94e89e3f698b49c73a758554f5932de6216d9e6d7ee9428835191d90110b1959") )
        (       2, uint256("0xebee40a0a5919c3aeb812bba56ecd072cfa2944a02a3b20d2fcb35e3edf14403") )
        (       3, uint256("0x7390236f4ec5464d1d8034f85964c814f22edafa1389f66cf21f0e5e19d87c36") )
        (       4, uint256("0x686951fa987eb496a4e726697ead9a2aac40fe0ca2c695c320cf9ba6c4e01ad9") )
        (       5, uint256("0xf497faa61a4bbbb5399745a5040b55bf7af260d62d2b25501cb19c1b8884da6f") )
        (       6, uint256("0x5dc68e1b3bd239e54c597fb5b5e1f3bfc395ac88f3c877b5c5ae5a6ce2838312") )
        (       7, uint256("0x69d68bed13234b25593a80bbd3399b4976bb624bf157330af9392d28b89de619") )
        (       8, uint256("0xb4b4a73bc205633333bcb7c58618e0a9f2c04b95cfb44c33b9eac6f0dc6e4070") )
        (       9, uint256("0xbde9bcd469aef8cf3234bbf336cdb47ab3fda2870ffd8524f0e6594de229aabe") )
        (      10, uint256("0x2a205442048f9a9474592beca18b156186ba6cdce3fd88b50698aae0a477e163") )
		(     100, uint256("0xeef90b98368945ef89e04fee0ba2ab0458c8491b8315eac4e2bd49dca595bb80") )
		(    1000, uint256("0xbfabd0343d33b4922603caae1c3c9ed4d8a2576e975f320e0c7ca93a6a8449f1") )
		(   10000, uint256("0x5e202ccd7c5ddeeb47229c67863e37cfed3d7324595d6a63096f063e8fb0c333") )
		(  100000, uint256("0x906f0f78934e0f71d8210e4f194dc39d2db7ac9d85bce6ce75248b1882c157da") )
		(  200000, uint256("0xf37148607a840c3429b65a6d645cdb9e23a536a5d50b706f122d58fd16d02b99") )
		(  300000, uint256("0x30256056620e14a3df90f1a28ab0b2306258de823e5d3bd822bf4fe82b52e354") )
		(  400000, uint256("0x1f44b186db8a01609958ff32884e65957e8889e626cb32169d9c0f04f4bbc800") )
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
