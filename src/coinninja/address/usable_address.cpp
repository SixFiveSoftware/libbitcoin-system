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

#include <bitcoin/bitcoin/coinninja/address/usable_address.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/key_factory.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>
#include <bitcoin/bitcoin/coinninja/address/segwit_address.hpp>

namespace coinninja {
namespace address {

usable_address::usable_address(bc::wallet::hd_private private_key, coinninja::wallet::derivation_path path)
    : private_key{private_key}, path{path} 
    { }

// public methods
bc::wallet::payment_address usable_address::build_payment_address()
{
    return p2wpkh_in_p2sh_address();
}

bc::ec_compressed usable_address::build_compressed_public_key()
{
    bc::wallet::hd_public index_public_key{coinninja::wallet::key_factory::index_public_key(private_key, path)};
    return index_public_key.point();
}

bc::chain::script usable_address::build_p2wpkh_script()
{
    return bc::chain::script(witness_program());
}

bc::wallet::hd_private usable_address::build_index_private_key()
{
    return coinninja::wallet::key_factory::index_private_key(private_key, path);
}

coinninja::address::meta_address usable_address::build_receive_address()
{
    auto address{generate_address()};
    auto index_public_key{coinninja::wallet::key_factory::index_public_key(private_key, path)};
    auto compressed{bc::wallet::ec_public{index_public_key}};
    bc::ec_uncompressed uncompressed;
    compressed.to_uncompressed(uncompressed);
    auto uncompressed_chunk{bc::to_chunk(uncompressed)};
    auto encoded_chunk{bc::encode_base16(uncompressed_chunk)};
    meta_address meta_address{address, path, encoded_chunk};
    return meta_address;
}

coinninja::address::meta_address usable_address::build_change_address()
{
    auto address{generate_address()};
    meta_address meta_address{address, path};
    return meta_address;
}

// private methods
bc::machine::operation::list usable_address::witness_program()
{
    bc::short_hash key_hash{bc::bitcoin_short_hash(build_compressed_public_key())};
    return {bc::machine::operation(bc::machine::opcode(0)), bc::machine::operation(bc::to_chunk(key_hash))};
}

bool usable_address::is_testnet()
{
    return path.get_coin() == 1;
}

std::string usable_address::generate_address()
{
    uint32_t purpose{path.get_purpose()};
    switch (purpose)
    {
    case coinninja::wallet::coin_derivation_purpose::BIP49:
    {
        return p2wpkh_in_p2sh_address().encoded();
    }

    case coinninja::wallet::coin_derivation_purpose::BIP84:
    {
        return p2wpkh_address();
    }
    
    default:
    {
        return "";
    }
    }

}

bc::wallet::payment_address usable_address::p2wpkh_in_p2sh_address()
{
    bc::chain::script witness_script{bc::chain::script(witness_program())};
    auto net = (path.get_coin() == 0) ? bc::wallet::payment_address::mainnet_p2sh : bc::wallet::payment_address::testnet_p2sh;
    bc::wallet::payment_address payment_address{witness_script, net};
    return payment_address;
}

std::string usable_address::p2wpkh_address()
{
    bc::short_hash key_hash{bc::bitcoin_short_hash(build_compressed_public_key())};
    auto script_pub_key{bc::to_chunk(key_hash)};
    coinninja::wallet::base_coin coin{path};
    auto hrp{coin.get_bech32_hrp()};
    const auto witver{0};
    std::string address{coinninja::address::segwit_address::encode(hrp, witver, script_pub_key)};
    return address;
}

} // namespace transaction
} // namespace coinninja
