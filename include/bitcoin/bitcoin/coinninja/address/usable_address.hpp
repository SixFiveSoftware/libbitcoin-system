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

#ifndef COINNINJA_USABLE_ADDRESS_HPP
#define COINNINJA_USABLE_ADDRESS_HPP

#include <string>
#include <vector>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/derivation_path.hpp>
#include <bitcoin/bitcoin/coinninja/address/meta_address.hpp>

namespace coinninja {
namespace address {

class usable_address {
public:
    /**
     * Constructors
     */

    /**
     * Creates an instance of a usable_address.
     * 
     * @param private_key The master private key of a wallet. Not a derived child from another private key.
     * @param path derivation_path object defining a particular path, also used to determine if mainnet/testnet.
     */
    usable_address(bc::wallet::hd_private private_key, coinninja::wallet::derivation_path path);

    /**
     * Member methods
     */
    bc::wallet::payment_address build_payment_address();
    bc::ec_compressed build_compressed_public_key();
    bc::chain::script build_p2wpkh_script();
    bc::wallet::hd_private build_index_private_key();
    coinninja::address::meta_address build_receive_address();
    coinninja::address::meta_address build_change_address();

private:
    bc::wallet::hd_private private_key;
    coinninja::wallet::derivation_path path{49};
    bool is_testnet();
    std::string generate_address();
    bc::wallet::payment_address p2wpkh_in_p2sh_address();
    std::string p2wpkh_address();
    bc::machine::operation::list witness_program();
};

} // namespace transaction
} // namespace coinninja

#endif
