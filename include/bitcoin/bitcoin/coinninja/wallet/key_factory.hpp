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

#ifndef COINNINJA_KEY_FACTORY_HPP
#define COINNINJA_KEY_FACTORY_HPP

#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/derivation_path.hpp>
#include <vector>
#include <string>

namespace coinninja {
namespace wallet {

    class key_factory {
        static bc::wallet::hd_private index_private_key(bc::wallet::hd_private const &master_key, coinninja::wallet::derivation_path &path);
        static bc::wallet::hd_public index_public_key(bc::wallet::hd_private const &master_key, coinninja::wallet::derivation_path &path);
    };

} // namespace wallet
} // namespace coinninja

#endif