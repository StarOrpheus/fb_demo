#include <iostream>
#include <string>
#include <ranges>
#include <iomanip>

#include "schema_includes/schema_generated.h"

#include <zstd.h>

template<std::ranges::range Rng,
         typename = std::enable_if_t<
           std::is_same_v<
             std::remove_reference_t<decltype(*(std::declval<Rng>().begin()))>,
             uint8_t>>>
void printSpan(Rng rng)
{
  std::cout << std::dec << std::distance(rng.begin(), rng.end()) << " bytes: ";
  for (auto&& x: rng)
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int) x;
  std::cout << std::endl;
}


auto main() -> int {
  flatbuffers::FlatBufferBuilder builder;
  std::vector<std::string> env {"asd", "foo", "asd"};
  std::vector<std::string> args {"asd", "hello", "world"};
  auto bc = schema::CreateBuildCommand(builder,
                                       true,
                                       42,
                                       builder.CreateVectorOfStrings(env),
                                       builder.CreateString("."),
                                       builder.CreateString("."),
                                       builder.CreateVectorOfStrings(args),
                                       42);
  builder.Finish(bc);

  auto data = builder.GetBufferSpan();
  std::cout << "Raw: " << std::endl;
  printSpan(data);


  std::vector<uint8_t> result(ZSTD_compressBound(data.size()));
  auto compress_size =
    ZSTD_compress(result.data(), result.size(), data.data(),
                  data.size(), ZSTD_c_compressionLevel);
  result.resize(compress_size);

  std::cout << "Compressed: " << std::endl;
  printSpan(result);
}
