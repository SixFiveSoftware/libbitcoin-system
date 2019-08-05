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

#include "address_helper.hpp"
#include <bitcoin/bitcoin/wallet/payment_address.hpp>
#include <bitcoin/bitcoin/coinninja/address/segwit_address.hpp>
#include <bitcoin/bitcoin/coinninja/wallet/base_coin.hpp>

#define kP2KHOutputSize 34
#define kP2SHOutputSize 32
#define kP2WPKHOutputSize 31
#define kDefaultOutputSize 32
#define kP2SHSegWitInputSize 91
#define kP2WPKHSegWitInputSize 68
#define kBaseTxBytes 11

namespace coinninja {
namespace address {

address_helper::address_helper(coinninja::wallet::base_coin coin)
    : coin{coin} {}

coinninja::address::payment_output_type address_helper::address_type_for_address(std::string address)
{
    if (address_is_p2wpkh(address)) {
        return P2WPKH;
    } else if (address_is_p2wsh(address)) {
        return P2WSH;
    } else {
        bc::wallet::payment_address payment_address{address};
        return address_type_for(payment_address);
    }
}

uint16_t address_helper::bytes_per_input()
{
    switch (coin.get_purpose())
    {
    case coinninja::wallet::coin_derivation_purpose::BIP84:
        return kP2WPKHSegWitInputSize;

    default:
        return kP2SHSegWitInputSize;
    }
}

uint16_t address_helper::bytes_per_change_output()
{
    switch (coin.get_purpose())
    {
    case coinninja::wallet::coin_derivation_purpose::BIP84:
        return kP2WPKHOutputSize;
    
    default:
        return kP2SHOutputSize;
    }
}

uint16_t address_helper::total_bytes(uint8_t number_of_inputs, std::string address, bool include_change_address)
{
    auto input_size = bytes_per_input();
    auto change_size = (include_change_address ? kP2SHOutputSize : 0);
    auto output_size = bytes_per_output_address(address);
    auto base_size = kBaseTxBytes;
    return (input_size * number_of_inputs) + output_size + change_size + base_size;
}

// private
bool address_helper::address_version_is_p2kh(uint8_t version)
{
    return version == bc::wallet::payment_address::mainnet_p2kh || 
        version == bc::wallet::payment_address::testnet_p2kh;
}

bool address_helper::address_version_is_p2sh(uint8_t version)
{
    return version == bc::wallet::payment_address::mainnet_p2sh || 
        version == bc::wallet::payment_address::testnet_p2sh;
}

bool address_helper::address_is_p2wpkh(std::string address)
{
    return coinninja::address::segwit_address::is_valid_p2wpkh_address(address);
}

bool address_helper::address_is_p2wsh(std::string address)
{
    return coinninja::address::segwit_address::is_valid_p2wsh_address(address);
}

coinninja::address::payment_output_type address_helper::address_type_for(bc::wallet::payment_address address)
{
    if (address_version_is_p2kh(address.version)) {
        return P2PKH;
    } else if (address_version_is_p2sh(address.version)) {
        return P2SH;
    } else {
        return P2PKH;
    }
}

uint8_t address_helper::bytes_per_output_address(std::string address)
{
    if (address_is_p2wpkh(address) || address_is_p2wsh(address)) {
        return kP2WPKHOutputSize;
    } else {
        bc::wallet::payment_address payment_address{address};
        uint8_t version = payment_address.version;
        if (address_version_is_p2kh(version)) {
            return kP2KHOutputSize;
        } else if (address_version_is_p2sh(version)) {
            return kP2SHOutputSize;
        } else {
            return kDefaultOutputSize;
        }
    }
}

} // namespace address
} // namespace coinninja
