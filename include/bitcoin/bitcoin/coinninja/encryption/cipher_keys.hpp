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
#ifndef COINNINJA_ENCRYPTION_CIPHER_KEYS_HPP
#define COINNINJA_ENCRYPTION_CIPHER_KEYS_HPP

#include <bitcoin/bitcoin.hpp>

namespace coinninja {
namespace encryption {

class cipher_keys {
public:
    /// Constructors
    cipher_keys(bc::hash_digest &encryption_key, bc::hash_digest &hmac_key);

    /// Member methods
    bc::hash_digest get_encryption_key() const;
    bc::hash_digest get_hmac_key() const;

protected:
    bc::hash_digest encryption_key;
    bc::hash_digest hmac_key;

};

} // namespace encryption
} // namespace coinninja

#endif
