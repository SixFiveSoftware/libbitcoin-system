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

#include <bitcoin/bitcoin/coinninja/wallet/key_factory.hpp>

namespace coinninja {
namespace wallet {

/**
 * static methods
 */
bc::wallet::hd_private key_factory::index_private_key(bc::wallet::hd_private const &master_private_key, coinninja::wallet::derivation_path &path)
{
    const auto purpose_key = master_private_key.derive_private(path.get_hardened_purpose());
    const auto coin_key = purpose_key.derive_private(path.get_hardened_coin());
    const auto account_key = coin_key.derive_private(path.get_hardened_account());
    const auto change_key = account_key.derive_private(path.get_change());
    const auto index_key = change_key.derive_private(path.get_index());
    return index_key;
}

bc::wallet::hd_public key_factory::index_public_key(bc::wallet::hd_private const &master_private_key, coinninja::wallet::derivation_path &path)
{
    return key_factory::index_private_key(master_private_key, path).to_public();
}

bc::wallet::hd_private key_factory::signing_key(const bc::wallet::hd_private &master_private_key)
{
    static const int signing_purpose{42};
    return master_private_key.derive_private(signing_purpose);
}

std::string key_factory::coinninja_verification_key_hex_string(const bc::wallet::hd_private &master_private_key)
{
    return bc::encode_base16(signing_key(master_private_key).to_public().point());
}

} // namespace wallet
} // namespace coinninja