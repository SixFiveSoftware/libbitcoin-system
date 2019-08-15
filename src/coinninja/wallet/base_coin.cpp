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

#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>

namespace coinninja {
namespace wallet {

base_coin::base_coin(coin_derivation_purpose purpose, coin_derivation_coin coin, int account)
    : purpose{purpose}, coin{coin}, account{account} { }

coin_derivation_purpose base_coin::get_purpose()
{
    return purpose;
}

coin_derivation_coin base_coin::get_coin() const
{
    return coin;
}

int base_coin::get_account()
{
    return account;
}

std::string base_coin::get_bech32_hrp()
{
    if (purpose != BIP84) {
        return "";
    }
    return (coin == MainNet) ? "bc" : "tb";
}

} // namespace wallet
} // namespace coinninja
