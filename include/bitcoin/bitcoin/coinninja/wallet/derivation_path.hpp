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
#ifndef COINNINJA_DERIVATION_PATH_HPP
#define COINNINJA_DERIVATION_PATH_HPP

#include <bitcoin/bitcoin.hpp>

namespace coinninja {
namespace wallet {

    class derivation_path {
    public:
        derivation_path(int purpose);
        derivation_path(int purpose, int coin, int account, int change, int index);

        bool has_coin();
        int get_coin();
        int get_change();
        int get_index();
        int get_hardened_purpose();
        int get_hardened_coin();
        int get_hardened_account();

    private:
        uint32_t hardened_offset{bc::wallet::hd_first_hardened_key};
        int purpose{-1};
        int coin{-1};
        int account{-1};
        int change{-1};
        int index{-1};
    };

} // namespace wallet
} // namespace coinninja

#endif