/**
 * Copyright (c) 2019 CoinNinja developers
 *
 * This file is added on to libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <bitcoin/bitcoin/coinninja/wallet/hd_wallet.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/mnemonic.hpp>

namespace coinninja {
namespace wallet {

// static methods
std::vector<std::string> hd_wallet::all_bip_39_words()
{
    bc::wallet::dictionary words = bc::wallet::language::en;
    const int length = words.size();
    std::vector<std::string> result(length);
    for (size_t i{0}; i < length; ++i)
    {
        std::string word = std::string(words[i]);
        result.at(i) = word;
    }
    return result;
}

// constructors
hd_wallet::hd_wallet(coinninja::wallet::base_coin coin, bc::data_chunk const &entropy)
    : hd_wallet{coin, coinninja::wallet::create_mnemonic(entropy)} {}

hd_wallet::hd_wallet(coinninja::wallet::base_coin coin, std::vector<std::string> mnemonic_words)
    : coin{coin}, mnemonic_words{mnemonic_words}
{
    auto seed = to_chunk(decode_mnemonic(mnemonic_words));
    auto net = (coin.get_coin() == MainNet) ? hd_private::mainnet : hd_private::testnet;
    master_private_key = bc::wallet::hd_private(seed, net);
}

// instance methods
std::vector<std::string> hd_wallet::create_mnemonic_words(bc::data_chunk const &entropy)
{
    mnemonic_words = coinninja::wallet::create_mnemonic(entropy);
    // auto seed = to_chunk(decode_mnemonic(this->mnemonic_words)); //bc::wallet::hd_private(entropy);
    auto net = (coin.get_coin() == MainNet) ? hd_private::mainnet : hd_private::testnet;
    master_private_key = bc::wallet::hd_private(entropy, net);
    return mnemonic_words;
}

std::vector<std::string> hd_wallet::get_current_words()
{
    return mnemonic_words;
}

} // namespace wallet
} // namespace coinninja
