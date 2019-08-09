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

#include <bitcoin/bitcoin/coinninja/transaction/usable_address.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/key_factory.hpp>

namespace coinninja {
namespace transaction {

usable_address::usable_address(bc::wallet::hd_private private_key, coinninja::wallet::derivation_path path)
    : private_key{private_key}, path{path} 
    {
        if (path.get_coin() == 1)
        {
            testnet = true;
        }
    }

// public methods
bc::wallet::payment_address usable_address::build_payment_address()
{
    using namespace bc::wallet;
    bc::ec_compressed compressed_pub_key{build_compressed_public_key()};
    auto net = (path.get_coin() == 0) ? payment_address::mainnet_p2sh : payment_address::testnet_p2sh;
    bc::chain::script p2wpkh_script{build_p2wpkh_script()};
    return bc::wallet::payment_address(p2wpkh_script, net);
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

// private methods
bc::machine::operation::list usable_address::witness_program()
{
    bc::short_hash key_hash{bc::bitcoin_short_hash(build_compressed_public_key())};
    return {bc::machine::operation(bc::machine::opcode(0)), bc::machine::operation(bc::to_chunk(key_hash))};
}

} // namespace transaction
} // namespace coinninja
