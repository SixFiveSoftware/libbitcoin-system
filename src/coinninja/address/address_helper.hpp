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

#ifndef COINNINJA_ADDRESS_HELPER_HPP
#define COINNINJA_ADDRESS_HELPER_HPP

#include <string>
#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>

namespace coinninja {
namespace address {

enum payment_output_type {
    P2PKH = 0,
    P2SH = 1,
    P2WPKH = 2,
    P2WSH = 3
};

class address_helper {

public:
    /**
     * constructors
     */
    address_helper(coinninja::wallet::base_coin coin);

    /**
     * instance methods
     */
    /// Return payment type enum case for what the given address is.
    payment_output_type address_type_for_address(std::string address);

    /// bytes per input
    uint16_t bytes_per_input();

    /// bytes per change output
    uint16_t bytes_per_change_output();

    /// total bytes with number of inputs, destination payment address, optionally including change address
    uint16_t total_bytes(uint8_t number_of_inputs, std::string address, bool include_change_address);

private:
    coinninja::wallet::base_coin coin;

    bool address_version_is_p2kh(uint8_t version);
    bool address_version_is_p2sh(uint8_t version);
    bool address_is_p2wpkh(std::string);
    bool address_is_p2wsh(std::string);
    payment_output_type address_type_for(bc::wallet::payment_address);
    uint8_t bytes_per_output_address(std::string address);
};

} // namespace address
} // namespace coinninja

#endif