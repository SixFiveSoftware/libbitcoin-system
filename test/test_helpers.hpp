#ifndef COINNINJA_TEST_HELPERS
#define COINNINJA_TEST_HELPERS

#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>

using namespace bc::wallet;
using namespace coinninja::wallet;

static hd_private private_key_for(const word_list &words, const base_coin &coin)
{
    auto seed = bc::to_chunk(decode_mnemonic(words));
    auto net = (coin.get_coin() == 0) ? hd_private::mainnet : hd_private::testnet;
    return hd_private(seed, net);
}

static word_list test_only_words {
    std::string{"abandon"},
    std::string{"abandon"},
    std::string{"abandon"},
    std::string{"abandon"},
    std::string{"abandon"},
    std::string{"abandon"},
    std::string{"abandon"},
    std::string{"abandon"},
    std::string{"abandon"},
    std::string{"abandon"},
    std::string{"abandon"},
    std::string{"about"}
};

#endif