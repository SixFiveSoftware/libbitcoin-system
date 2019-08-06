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

#include <bitcoin/bitcoin/coinninja/address/meta_address.hpp>

namespace coinninja {
namespace address {

meta_address::meta_address(std::string address, coinninja::wallet::derivation_path path, std::string uncompressed_public_key)
    : address{address}, path{path}, uncompressed_public_key{uncompressed_public_key} { }

std::string meta_address::get_address()
{
    return address;
}

coinninja::wallet::derivation_path meta_address::get_derivation_path()
{
    return path;
}

std::string meta_address::get_uncompressed_public_key()
{
    return uncompressed_public_key;
}

} // namespace address
} // namespace coinninja
