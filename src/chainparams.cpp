// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << CScriptNum(42) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = 0;
    txNew.vout[0].scriptPubKey.clear();

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.vtx[0].nTime = nTime;
    genesis.vtx[0].UpdateHash();
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion)
{
    const char* pszTimestamp = "VectoriumPlus - In CryptoEnergy we trust";
    return CreateGenesisBlock(pszTimestamp, nTime, nNonce, nBits, nVersion);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 128 * 60; // 128 minutes
        consensus.nPowTargetSpacing = 240;
        consensus.nLastPOWBlock = 5000;
        consensus.nStakeMinAge = 12 * 60 * 60;
        consensus.nModifierInterval = 10 * 60;
        consensus.nStakeSplitAge = 24 * 60 * 60;
        consensus.timeLimitChange = 1554076800;
        consensus.nStakeCombineThreshold = 2000 * COIN;
        consensus.checkpointPubKey = "04db0456702458c454ba9f0dcaa872d872b79f713e8881f913d470c098bd77a12d85c0048982ab645e7e19d5494e2b453b8b1dee6fdfdb0f0b6b829488f5f63f8a";
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1533081600; // August 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1564617600; // August 1st, 2019

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1533081600; // August 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1564617600; // August 1st, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xa6;
        pchMessageStart[1] = 0xae;
        pchMessageStart[2] = 0xcb;
        pchMessageStart[3] = 0xfa;
        nDefaultPort = 18884;
        nPruneAfterHeight = 10000;

        genesis = CreateGenesisBlock(1550147522, 39471, 0x1e0fffff, 1);
        
  /*        consensus.hashGenesisBlock = uint256S("0x0000045c4fb6799ddce4ad8c977f2ce968f8ea8edca25bd67072ac990562404d");
        printf("min nBit:  %08x\n", UintToArith256(consensus.powLimit).GetCompact());

       if (false && genesis.GetHash() != consensus.hashGenesisBlock)
                {
                    printf("recalculating params for mainnet.\n");
                    printf("old mainnet genesis nonce: %d\n", genesis.nNonce);
                    printf("old mainnet genesis hash:  %s\n", consensus.hashGenesisBlock.ToString().c_str());

                    // deliberately empty for loop finds nonce value.
                    for(genesis.nNonce == 0; UintToArith256(genesis.GetHash()) > UintToArith256(consensus.powLimit); genesis.nNonce++){ 
                        if(genesis.nNonce%1000000 == 0) { 
                            printf("current mainnet genesis nonce: %d --> %s\n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                            
                        };
                    } 
                    printf("new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
                    printf("new mainnet genesis nonce: %d\n", genesis.nNonce);
                    printf("new mainnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
                }
  */
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000045c4fb6799ddce4ad8c977f2ce968f8ea8edca25bd67072ac990562404d"));
        assert(genesis.hashMerkleRoot == uint256S("0x1ad07a62b346532c3c2806e8ba0e462704859d590f7df86a3b8d79993f9b2e5c"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.push_back(CDNSSeedData("seed1-vctp.vectorium.co", "seed1-vctp.vectorium.co", false));
        vSeeds.push_back(CDNSSeedData("seed2-vctp.vectorium.co", "seed2-vctp.vectorium.co", false));
        vSeeds.push_back(CDNSSeedData("seed3-vctp.vectorium.co", "seed3-vctp.vectorium.co", false));
        vSeeds.push_back(CDNSSeedData("seed4-vctp.vectorium.co", "seed4-vctp.vectorium.co", false));
        vSeeds.push_back(CDNSSeedData("seed5-vctp.vectorium.co", "seed5-vctp.vectorium.co", false));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,56);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (0, uint256S("0x0000045c4fb6799ddce4ad8c977f2ce968f8ea8edca25bd67072ac990562404d")),
            1550147522, // * UNIX timestamp of last checkpoint block
            0,   // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.0     // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 24 * 60 * 60; // 1 day
        consensus.nPowTargetSpacing = 5;
        consensus.nLastPOWBlock = 4000;
        consensus.nStakeMinAge = 30;
        consensus.nModifierInterval = 60;
        consensus.nStakeSplitAge = 24 * 60 * 60;
        consensus.timeLimitChange = 1615825600;
        consensus.nStakeCombineThreshold = 5000 * COIN;
        consensus.checkpointPubKey = "04d6b41194f42be5400edf8413db009add93ae18e4913738ef6967658afb0564392d708c6985ebfbf8c06e128f13a7d4a5938f351ac76df5f0742c2c9b9d68b276";
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1533081600; // August 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1564617600; // August 1st, 2019

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1533081600; // August 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1564617600; // August 1st, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        pchMessageStart[0] = 0x72;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x22;
        pchMessageStart[3] = 0x05;
        nDefaultPort = 28884;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1550147523, 912031, 0x1e0fffff, 1);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000047f593dd89b973b27fd1dcadbd300532729b035252d06193eeae56bb8c0"));
        assert(genesis.hashMerkleRoot == uint256S("0xdc060c995245e6eeb09965a9967ae91c35c2399bc2e9f481cbe8f81d6c5979a3"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("VectoriumPlus.net", "testnet-seed1.VectoriumPlus.net", false));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;


        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x0000047f593dd89b973b27fd1dcadbd300532729b035252d06193eeae56bb8c0")),
            1550147523,
            0,
            0
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 16 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 5;
        consensus.nLastPOWBlock = 4000;
        consensus.nStakeMinAge = 12 * 60 * 60;
        consensus.nModifierInterval = 10 * 60;
        consensus.nStakeSplitAge = 24 * 60 * 60;
        consensus.timeLimitChange = 0;
        consensus.nStakeCombineThreshold = 10000 * COIN;
        consensus.checkpointPubKey = "04d6b41194f42be5400edf8413db009add93ae18e4913738ef6967658afb0564392d708c6985ebfbf8c06e128f13a7d4a5938f351ac76df5f0742c2c9b9d68b276";
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xbf;
        pchMessageStart[3] = 0x22;
        nDefaultPort = 28884;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1550147526, 0, 0x207fffff, 1);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x1faee44d8e369aebe1784d8ce28585df60b52e981beaf2917ec26cafaacffcdb"));
        assert(genesis.hashMerkleRoot == uint256S("0x877403eaca83fc075bddf74af4403413761d7397a030f81b8b93b4e4aa3e2f5b"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x1faee44d8e369aebe1784d8ce28585df60b52e981beaf2917ec26cafaacffcdb")),
            1550147526,
            0,
            0
        };
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
 
