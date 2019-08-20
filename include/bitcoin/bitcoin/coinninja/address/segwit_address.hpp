/* Copyright (c) 2017 Pieter Wuille
 *
 * Modified for use by CoinNinja.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef COINNINJA_ADDRESS_SEGWIT_ADDRESS_HPP
#define COINNINJA_ADDRESS_SEGWIT_ADDRESS_HPP

#include <stdint.h>
#include <vector>
#include <string>

namespace coinninja {
namespace address {
namespace segwit_address {

    /** Decode a SegWit address. Returns (witver, witprog). witver = -1 means failure. */
    std::pair<int, std::vector<uint8_t> > decode(const std::string& hrp, const std::string& addr);

    /** Encode a SegWit address. Empty string means failure. */
    std::string encode(const std::string& hrp, int witver, const std::vector<uint8_t>& witprog);

    /// Determine if address is P2WPKH
    bool is_valid_p2wpkh_address(std::string address);

    /// Determine if address is P2WSH
    bool is_valid_p2wsh_address(std::string address);

} // namespace segwit_address
} // namespace address
} // namespace coinninja

#endif