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

#ifndef COINNINJA_DATA_SIGNING_HPP
#define COINNINJA_DATA_SIGNING_HPP

#include <bitcoin/bitcoin.hpp>
#include <string>

namespace coinninja {
namespace wallet {

class data_signing {
public:
    /// Converts the call to sign_data to base16 encoded string.
    static std::string signature_signing_data(const bc::data_chunk &data, const bc::wallet::hd_private &master_private_key);

    /// Verifies a given data chunk against its signed version.
    static bool verify_signed_data(const bc::data_chunk &data, const bc::data_chunk &signature, const bc::wallet::hd_private &master_private_key);

    /// Signs a given data chunk, provided a master private key.
    static bc::data_chunk sign_data(const bc::data_chunk &data, const bc::wallet::hd_private &master_private_key);
};

} // namespace wallet
} // namespace coinninja

#endif