#pragma once
#include <array>

namespace aob
{
    using Byte = unsigned char;
    static_assert(sizeof(Byte) == 1);

    enum ByteMask : Byte
    {
        NMask = 0, // NO MASK
        LMask = 1, // LEFT MASK
        RMask = 2, // RIGHT MASK
        FMask = 3, // FULL MASK
    };

    constexpr aob::Byte toByte(aob::Byte c)
    {
        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;
        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        if (c >= '0' && c <= '9')
            return c - '0' + 0;

        return c;
    }

    constexpr aob::Byte createByteMask(aob::Byte Byte, ByteMask Mask)
    {
        if (Mask == LMask)
            Byte |= 0xF0;
        else if (Mask == RMask)
            Byte |= 0x0F;
        else if (Mask == FMask)
            Byte = 0xFF;

        return Byte;
    }

    struct MaskedByte
    {
        explicit constexpr MaskedByte(aob::Byte Byte = 0xFF, ByteMask Mask = FMask) : Mask(Mask)
        {
            this->Byte = createByteMask(Byte, Mask);
        }

        [[nodiscard]] constexpr bool compare(aob::Byte OtherByte) const
        {
            return Byte == createByteMask(OtherByte, Mask);
        }

        static constexpr MaskedByte createMaskedByteFromChars(aob::Byte L, aob::Byte R)
        {
            L = toByte(L);
            R = toByte(R);
            aob::Byte LR = (L << 4) | (R & 0x0F);
            aob::Byte Mask = NMask;

            if (L == '?')
                Mask |= LMask;
            if (R == '?')
                Mask |= RMask;

            return MaskedByte(LR, static_cast<aob::ByteMask>(Mask));
        }

        aob::Byte Byte;
        aob::ByteMask Mask;
    };

    template <std::size_t N>
    struct AobLiteral
    {
        constexpr AobLiteral(const char (&Str)[N]) : Bytes(1), Data{}, Size(N)
        {
            for (int i = 0; i < N; ++i)
            {
                if (Str[i] == ' ')
                    ++Bytes;

                Data[i] = Str[i];
            }
        }

        std::size_t Bytes;
        std::size_t Size;
        aob::Byte Data[N];
    };

    template <AobLiteral Lit>
    constexpr auto toAob()
    {
        static_assert(Lit.Size / Lit.Bytes == 3);
        std::array<MaskedByte, Lit.Bytes> res;

        for (std::size_t i = 0, j = 0; i < Lit.Size; i += 3, ++j)
            res[j] = aob::MaskedByte::createMaskedByteFromChars(Lit.Data[i], Lit.Data[i + 1]);

        return res;
    }
} // namespace aob
