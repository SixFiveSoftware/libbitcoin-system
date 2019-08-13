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

#include <bitcoin/bitcoin/coinninja/wallet/mnemonic.hpp>
#include <bitcoin/bitcoin/wallet/mnemonic.hpp>

namespace coinninja {
namespace wallet {

word_list all_bip_39_words()
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

word_list create_mnemonic(const data_slice &entropy, const dictionary &lexicon)
{
    word_list mnemonic{bc::wallet::create_mnemonic(entropy, lexicon)};
    if (bc::wallet::validate_mnemonic(mnemonic))
    {
        return mnemonic;
    } else {
        throw "Invalid mnemonic";
    }
}

} // namespace wallet
} // namespace coinninja
