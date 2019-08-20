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

#include <bitcoin/bitcoin/coinninja/encryption/cipher_keys.hpp>

namespace coinninja {
namespace encryption {

cipher_keys::cipher_keys(bc::hash_digest &encryption_key, bc::hash_digest &hmac_key)
    : encryption_key{encryption_key}, hmac_key{hmac_key}
    {}

bc::hash_digest cipher_keys::get_encryption_key() const
{
    return encryption_key;
}

bc::hash_digest cipher_keys::get_hmac_key() const
{
    return hmac_key;
}

} // namespace encryption
} // namespace coinninja