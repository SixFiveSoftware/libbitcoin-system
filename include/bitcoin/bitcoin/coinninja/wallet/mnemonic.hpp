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
#ifndef COINNINJA_MNEMONIC_HPP
#define COINNINJA_MNEMONIC_HPP

#include <bitcoin/bitcoin/wallet/mnemonic.hpp>
#include <bitcoin/bitcoin/utility/data.hpp>

using namespace bc;
using namespace wallet;

namespace coinninja {
namespace wallet {

    word_list all_bip_39_words();

    /**
     * Creates a mnemonic word list, given a source of secure random entropy.
     * @param entropy Bytes of secure random entropy provided by device. DO NOT use pseudo_random_fill in production.
     * @param lexicon Language of words, default is en.
     * @return A word_list (std::vector<std::string>) of words, relative to the size of entropy received, or an empty vector if checksum failed. If empty, try again with new entropy.
     */
    word_list create_mnemonic(const data_slice &entropy, const dictionary &lexicon=language::en);

} // namespace wallet
} // namespace coinninja


#endif