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

#include <bitcoin/bitcoin/coinninja/encryption/cipher_key_vendor.hpp>

namespace coinninja {
namespace encryption {

coinninja::encryption::cipher_keys cipher_key_vendor::decryption_cipher_keys(const bc::wallet::hd_private &master_private_key, const bc::data_chunk &public_key_data)
{
    bc::ec_secret secret_key{master_private_key.secret()};
    coinninja::encryption::cipher_keys keys{cipher_keys_with_secret_key_and_public_key(secret_key, public_key_data)};
    return keys;
}

coinninja::encryption::encryption_cipher_keys cipher_key_vendor::encryption_cipher_keys_for_uncompressed_public_key(const bc::data_chunk &public_key_data, const bc::data_chunk &entropy)
{
    // generate ephemeral key
    auto entropy_byte_array = bc::to_array<bc::ec_secret_size>(entropy);
    bc::ec_secret secret_key{entropy_byte_array};

    cipher_keys keys{cipher_keys_with_secret_key_and_public_key(secret_key, public_key_data)};

    // get uncompressed public key from ephemeral secret
    bc::ec_uncompressed ephemeral_public_key;
    bc::wallet::ec_public(secret_key).to_uncompressed(ephemeral_public_key);
    bc::data_chunk eph_pubkey_output(sizeof(ephemeral_public_key));
    std::copy(ephemeral_public_key.begin(), ephemeral_public_key.end(), eph_pubkey_output.begin());

    // coinninja::encryption::encryption_cipher_keys encryption_keys{keys.get_encryption_key(), keys.get_hmac_key(), eph_pubkey_output};
    auto enc{keys.get_encryption_key()};
    auto hmac{keys.get_hmac_key()};
    coinninja::encryption::encryption_cipher_keys encryption_keys{enc, hmac, eph_pubkey_output};
    return encryption_keys;
}

// private
coinninja::encryption::cipher_keys cipher_key_vendor::cipher_keys_with_secret_key_and_public_key(const bc::ec_secret &secret_key, const bc::data_chunk &public_key_data)
{
    // get uncompressed public key
    bc::ec_uncompressed recipient_uncompressed_pubkey{uncompressed_public_key(public_key_data)};

    // generate ecdh_key
    bc::data_chunk ecdh_key{generate_shared_secret(secret_key, recipient_uncompressed_pubkey)};

    // derive key from ecdh_key
    bc::data_slice slice{ecdh_key};
    bc::long_hash derived_key{bc::sha512_hash(slice)};

    // get keyE and keyM
    bc::hash_digest keyE;  // sizeof = 256 bits
    bc::hash_digest keyM;

    std::copy(derived_key.begin(), derived_key.begin() + bc::hash_size, keyE.begin());
    std::copy(derived_key.begin() + bc::hash_size, derived_key.end(), keyM.begin());

    cipher_keys keys(keyE, keyM);
    return keys;
}

bc::data_chunk cipher_key_vendor::generate_shared_secret(const bc::ec_secret &secret_key, bc::ec_uncompressed &uncompressed_public_key)
{
    bc::ec_multiply(uncompressed_public_key, secret_key);
    return bc::to_chunk(uncompressed_public_key);
}

bc::ec_uncompressed cipher_key_vendor::uncompressed_public_key(const bc::data_chunk &public_key_data)
{
    bc::wallet::ec_public ec_public_key{public_key_data};
    bc::ec_uncompressed recipient_uncompressed_pubkey;
    ec_public_key.to_uncompressed(recipient_uncompressed_pubkey);
    return recipient_uncompressed_pubkey;
}

} // namespace encryption
} // namespace coinninja
