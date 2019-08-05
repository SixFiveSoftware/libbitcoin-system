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

#ifndef COINNINJA_META_ADDRESS_HPP
#define COINNINJA_META_ADDRESS_HPP

#include <string>
#include <bitcoin/bitcoin/coinninja/wallet/derivation_path.hpp>

namespace coinninja {
namespace address {

class meta_address {
public:
    meta_address(std::string address, coinninja::wallet::derivation_path path, std::string uncompressed_public_key);
    std::string get_address();
    coinninja::wallet::derivation_path get_derivation_path();
    std::string get_uncompressed_public_key();

private:
    std::string address;
    coinninja::wallet::derivation_path path;
    std::string uncompressed_public_key;
};

} // namespace address
} // namespace coinninja

#endif