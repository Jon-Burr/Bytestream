/**
 * @file ByteArrayView.h
 * @author Jon Burr (jon.burr@cern.ch)
 * @brief Modifiable view onto a byte array
 * @version 0.1
 * @date 2022-06-18
 *
 * @copyright Copyright (c) 2022
 */

#include "Bytestream/ConstByteArrayView.h"
#include <iterator>

namespace Bytestream
{
    class ByteArrayView : public ConstByteArrayView
    {
    public:
        using iterator = std::reverse_iterator<std::byte *>;
        using reverse_iterator = std::byte *;
        ByteArrayView(void *data, std::size_t nBytes);

        /**
         * @brief Copy the data held in other to our view
         *
         * Will copy as much data from other as will fit in this. If other is
         * shorter than this, fill bytes from the right.
         */
        void copyFrom(const ConstByteArrayView &other);

        std::byte *data()
        {
            // NB: The const_cast here is entirely safe: we know that in truth
            // we were handed a non-const pointer. It's only to share it with
            // the const base class that we ever had to make it const.
            return const_cast<std::byte *>(ConstByteArrayView::data());
        }
        using ConstByteArrayView::data;

        iterator begin() { return std::make_reverse_iterator(data() + size()); }
        using ConstByteArrayView::begin;
        iterator end() { return std::make_reverse_iterator(data()); }
        using ConstByteArrayView::end;

        reverse_iterator rbegin() { return data(); }
        using ConstByteArrayView::rbegin;
        reverse_iterator rend() { return data() + size(); }
        using ConstByteArrayView::rend;

        std::byte &operator[](std::size_t idx)
        {
            return *(data() + size() - idx - 1);
        }
        using ConstByteArrayView::operator[];

        std::byte &at(std::size_t idx);
        using ConstByteArrayView::at;

        ByteArrayView view(std::size_t idx, std::size_t nBytes);
        using ConstByteArrayView::view;
    };
} // namespace Bytestream