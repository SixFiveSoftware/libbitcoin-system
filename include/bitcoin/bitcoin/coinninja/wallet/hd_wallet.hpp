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

#ifndef COINNINJA_HD_WALLET_HPP
#define COINNINJA_HD_WALLET_HPP

#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>
#include <vector>
#include <string>

namespace coinninja {
namespace wallet {

    class hd_wallet {
    public:
        /**
         * static methods
         */
        static std::vector<std::string> all_bip_39_words();

        /**
         *  constructors
         */
        /// Init with coin and entropy. Creates new mnemonic words from entropy.
        hd_wallet(coinninja::wallet::base_coin coin, bc::data_chunk const &entropy);

        /// Init with coin and mnemonic words. For use when a user's wallet exists.
        hd_wallet(coinninja::wallet::base_coin coin, std::vector<std::string> mnemonic_words = {});

        /**
         * instance methods
         */
        /// Creates new mnemonic words and returns them.
        std::vector<std::string>create_mnemonic_words(bc::data_chunk const &entropy);

        /// Return current 12 mnemonic words for this wallet.
        std::vector<std::string>get_current_words();
    private:
        std::vector<std::string> mnemonic_words;
        coinninja::wallet::base_coin coin;
    };

} // namespace wallet
} // namespace coinninja

#endif
