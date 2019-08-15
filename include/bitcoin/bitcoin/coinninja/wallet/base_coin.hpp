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
#ifndef COINNINJA_BASE_COIN_HPP
#define COINNINJA_BASE_COIN_HPP

#include <string>

namespace coinninja {
namespace wallet {

    enum coin_derivation_purpose {
        BIP32 = 32,
        BIP39 = 39,
        BIP44 = 44,
        BIP49 = 49,
        BIP84 = 84
    };

    enum coin_derivation_coin {
        MainNet,
        TestNet
    };

    class base_coin {
        public:
            base_coin(coin_derivation_purpose purpose = BIP49, coin_derivation_coin coin = MainNet, int account = 0);
            coin_derivation_purpose get_purpose();
            coin_derivation_coin get_coin() const;
            int get_account();
            std::string get_bech32_hrp();

        private:
            coin_derivation_purpose purpose;
            coin_derivation_coin coin;
            int account;
    };

} // namespace wallet
} // namespace coinninja

#endif