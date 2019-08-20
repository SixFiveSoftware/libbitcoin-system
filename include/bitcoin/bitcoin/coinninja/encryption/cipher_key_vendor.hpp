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

#ifndef COINNINJA_ENCRYPTION_CIPHER_KEY_VENDOR_HPP
#define COINNINJA_ENCRYPTION_CIPHER_KEY_VENDOR_HPP

#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/coinninja/encryption/cipher_keys.hpp>
#include <bitcoin/bitcoin/coinninja/encryption/encryption_cipher_keys.hpp>

namespace coinninja {
namespace encryption {

class cipher_key_vendor {
public:
  static coinninja::encryption::cipher_keys decryption_cipher_keys(const bc::wallet::hd_private &master_private_key, const bc::data_chunk &public_key_data);
  static coinninja::encryption::encryption_cipher_keys encryption_cipher_keys_for_uncompressed_public_key(const bc::data_chunk &public_key_data, const bc::data_chunk &entropy);

private:
  static coinninja::encryption::cipher_keys cipher_keys_with_secret_key_and_public_key(const bc::ec_secret &secret_key, const bc::data_chunk &public_key_data);
  static bc::data_chunk generate_shared_secret(const bc::ec_secret &secret_key, bc::ec_uncompressed &uncompressed_public_key);
  static bc::ec_uncompressed uncompressed_public_key(const bc::data_chunk &public_key_data);

};

} // namespace encryption
} // namespace coinninja

#endif