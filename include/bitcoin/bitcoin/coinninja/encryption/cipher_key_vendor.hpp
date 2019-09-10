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
    /**
     * decryption_cipher_keys
     * Do not pass in the master private key, this should be the private key associated with the public key used to encrypt.
     * @param private_key The private key for the corresponding key needed to decrypt, NOT the master private key.
     * @param public_key_data The counterparty's public key.
     * @return A cipher_keys instance containing encryption key and hmac key.
     */
    static coinninja::encryption::cipher_keys decryption_cipher_keys(const bc::wallet::hd_private &private_key, const bc::data_chunk &public_key_data);

    /**
     * encryption_cipher_keys_for_uncompressed_public_key
     * @param public_key_data The uncompressed public key of the recipient of the encrypted payload.
     * @param entropy Client-provided secure random entropy to be used for creating the ephemeral key-pair.
     * @return An encryption_cipher_keys instance containing the encryption key, hmac key, and ephemeral public key.
     */
    static coinninja::encryption::encryption_cipher_keys encryption_cipher_keys_for_uncompressed_public_key(const bc::data_chunk &public_key_data, const bc::data_chunk &entropy);

    /**
     * encryption_cipher_keys_for_uncompressed_public_key
     * @param public_key_data The uncompressed public key of the recipient of the encrypted payload.
     * @param private_key The private key needed to generate the shared secret, if known ahead of time and not ephemeral.
     * @return An encryption_cipher_keys instance containing the encryption key, hmac key, and associated public key.
     */
    static coinninja::encryption::encryption_cipher_keys encryption_cipher_keys_for_uncompressed_public_key(const bc::data_chunk &public_key_data, const bc::wallet::hd_private &private_key);

private:
    static coinninja::encryption::cipher_keys cipher_keys_with_secret_key_and_public_key(const bc::ec_secret &secret_key, const bc::data_chunk &public_key_data);
    static bc::data_chunk generate_shared_secret(const bc::ec_secret &secret_key, bc::ec_uncompressed &uncompressed_public_key);
    static bc::ec_uncompressed uncompressed_public_key(const bc::data_chunk &public_key_data);
    static coinninja::encryption::encryption_cipher_keys encryption_cipher_keys(const bc::ec_secret &secret, const bc::data_chunk &public_key_data);

};

} // namespace encryption
} // namespace coinninja

#endif