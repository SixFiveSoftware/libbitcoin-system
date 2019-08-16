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

derivation_path::derivation_path()
    : derivation_path{49,0,0,0,0} { }

derivation_path::derivation_path(uint32_t purpose, uint32_t coin, uint32_t account, uint32_t change, uint32_t index)
    : purpose{purpose}, coin{coin}, account{account}, change{change}, index{index} { }

uint32_t derivation_path::get_purpose() {
    return purpose;
}

uint32_t derivation_path::get_coin() {
    return coin;
}

uint32_t derivation_path::get_account() {
    return account;
}

uint32_t derivation_path::get_change() {
    return change;
}

uint32_t derivation_path::get_index() {
    return index;
}

uint32_t derivation_path::get_hardened_purpose() {
    return purpose + hardened_offset;
}

uint32_t derivation_path::get_hardened_coin() {
    return coin + hardened_offset;
}

uint32_t derivation_path::get_hardened_account() {
    return account + hardened_offset;
}

} // namespace wallet
} // namespace coinninja