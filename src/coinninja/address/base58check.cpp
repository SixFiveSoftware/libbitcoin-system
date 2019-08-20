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

#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/coinninja/address/base58check.hpp>

namespace coinninja {
namespace address {

bool base58check::verify_base_58_check_encoding(const std::string &address)
{
    bc::data_chunk chunk;
    bc::decode_base58(chunk, address);

    static const int checksum_size{4};

    if (chunk.size() < checksum_size)
    {
        return false;
    }

    bc::data_chunk checksum_chunk(checksum_size);
    std::copy(chunk.end() - checksum_size, chunk.end(), checksum_chunk.begin());
    const std::string last_four{bc::encode_base16(checksum_chunk)};

    bc::data_chunk chunk_to_hash(chunk.size() - checksum_size);
    std::copy(chunk.begin(), chunk.end() - checksum_size, chunk_to_hash.begin());
    bc::hash_digest double_hashed_address = bc::bitcoin_hash(chunk_to_hash);

    bc::data_chunk first_four_from_hash(checksum_size);
    std::copy(double_hashed_address.begin(), double_hashed_address.begin() + checksum_size, first_four_from_hash.begin());
    const std::string first_four{bc::encode_base16(first_four_from_hash)};

    return first_four == last_four;
}

} // namespace address
} // namespace coinninja
