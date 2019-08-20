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

#include <bitcoin/bitcoin/coinninja/wallet/data_signing.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/key_factory.hpp>

namespace coinninja {
namespace wallet {

std::string data_signing::signature_signing_data(const bc::data_chunk &data, const bc::wallet::hd_private &master_private_key)
{
    bc::data_chunk signed_data = sign_data(data, master_private_key);
    if (signed_data.size() == 0) 
    {
        return "";
    }
    return bc::encode_base16(signed_data);
}

bool data_signing::verify_signed_data(const bc::data_chunk &data, const bc::data_chunk &signature, const bc::wallet::hd_private &master_private_key)
{
    bc::hash_digest msg = bc::bitcoin_hash(data);
    bc::ec_compressed point{coinninja::wallet::key_factory::signing_key(master_private_key).to_public().point()};

    bc::ec_signature sig;
    if (bc::parse_signature(sig, signature, true))
    {
        return bc::verify_signature(point, msg, sig);
    }
    return false;
}

bc::data_chunk data_signing::sign_data(const bc::data_chunk &data, const bc::wallet::hd_private &master_private_key)
{
    bc::hash_digest msg = bc::bitcoin_hash(data);
    bc::ec_signature signature;
    const auto signing_secret{coinninja::wallet::key_factory::signing_key(master_private_key).secret()};
    if (bc::sign(signature, signing_secret, msg))
    {
        bc::der_signature der_signature;
        if (bc::encode_signature(der_signature, signature))
        {
            return der_signature;
        }
    }
    return {};
}

} // namespace wallet
} // namespace coinninja
