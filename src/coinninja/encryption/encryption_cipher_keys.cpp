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

#include <bitcoin/bitcoin/coinninja/encryption/encryption_cipher_keys.hpp>

namespace coinninja {
namespace encryption {

encryption_cipher_keys::encryption_cipher_keys(bc::hash_digest &encryption_key, bc::hash_digest &hmac_key, bc::data_chunk &associated_public_key)
    : cipher_keys{encryption_key, hmac_key}, associated_public_key{associated_public_key}
{ }

bc::data_chunk encryption_cipher_keys::get_associated_public_key() const
{
    return associated_public_key;
}

} // namespace encryption
} // namespace coinninja