#include <cstdint>
#include <limits.h>

namespace maske {
    using Mask = std::uint64_t;

    namespace core {
        constexpr std::size_t BITS = sizeof(Mask) * CHAR_BIT;

        template <std::size_t N>
        struct Loop {
            template <typename F>
            constexpr void operator()(F& f, Mask upper, std::size_t start) {
                for (std::size_t i { start }; i <= BITS - N; i++) {
                    Mask mask = (Mask(1) << i) | upper;
                    Loop<N - 1>()(f, mask, i + 1);
                }
            }
        };

        template<>
        struct Loop<1> {
            template <typename F>
            constexpr void operator()(F& f, Mask upper, std::size_t start) {
                for (std::size_t i { start }; i < BITS; i++) {
                    Mask mask = (Mask(1) << i) | upper;
                    f(mask);
                }
            }
        };
    };

    template<std::size_t N>
    struct Permutator {
        static_assert(0 < N && N <= core::BITS / 2);

        template <typename F>
        constexpr void operator()(F& f) {
            core::Loop<N>()(f, 0, 0);
        }
    };
};
