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

#include <bitcoin/bitcoin/coinninja/wallet/derivation_path.hpp>

namespace coinninja {
namespace wallet {

    derivation_path::derivation_path(int purpose)
    {
        this->purpose = purpose;
    }

    derivation_path::derivation_path(int purpose, int coin, int account, int change, int index)
    {
        this->purpose = purpose;
        this->coin = coin;
        this->account = account;
        this->change = change;
        this->index = index;
    }

    bool derivation_path::has_coin() {
        return coin >= 0;
    }

    int derivation_path::get_coin() {
        return coin;
    }

    int derivation_path::get_change() {
        return change;
    }

    int derivation_path::get_index() {
        return index;
    }

    int derivation_path::get_hardened_purpose() {
        return purpose + hardened_offset;
    }

    int derivation_path::get_hardened_coin() {
        return coin + hardened_offset;
    }

    int derivation_path::get_hardened_account() {
        return account + hardened_offset;
    }

} // namespace wallet
} // namespace coinninja