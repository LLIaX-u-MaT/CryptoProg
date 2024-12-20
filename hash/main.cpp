#include <boost/program_options.hpp>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <fstream>
#include <iostream>

using namespace std;

namespace po = boost::program_options;

template <typename HashType> std::string goHash(const std::string &filename) {
  namespace CPP = CryptoPP;
  HashType hash;
  std::string digest;
  CPP::FileSource fileSource(
      filename.c_str(), true,
      new CPP::HashFilter(hash,
                          new CPP::HexEncoder(new CPP::StringSink(digest))));

  return digest;
}

int main(int argc, char **argv) {
  try {
    po::options_description desc("Доступные опции");
    desc.add_options()("help,h", "Вызов справки")(
        "input,I", po::value<std::string>(), "Файл для хэширования")(
        "hash,H", po::value<std::string>()->default_value("SHA224"),
        "Тип хэширования");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (argc < 2) {
      std::cout << desc << std::endl;
      std::cout << "Пример: " << argv[0]
                << " -H SHA1|SHA224|SHA256|SHA384|SHA512 -I файл"
                << "\n";
      return 0;
    }

    if (vm.count("help")) {
      std::cout << desc << "\n";
      std::cout << "Пример: " << argv[0]
                << " -H SHA1|SHA224|SHA256|SHA384|SHA512 -I файл"
                << "\n";
      return 0;
    }

    if (!vm.count("input")) {
      throw po::required_option("input");
    }

    std::string filename = vm["input"].as<std::string>();
    std::string hashType = vm["hash"].as<std::string>();
    std::string digest;

    if (hashType == "SHA1") {
      digest = goHash<CryptoPP::SHA1>(filename);
    } else if (hashType == "SHA224") {
      digest = goHash<CryptoPP::SHA224>(filename);
    } else if (hashType == "SHA256") {
      digest = goHash<CryptoPP::SHA256>(filename);
    } else if (hashType == "SHA384") {
      digest = goHash<CryptoPP::SHA384>(filename);
    } else if (hashType == "SHA512") {
      digest = goHash<CryptoPP::SHA512>(filename);
    } else {
      throw std::invalid_argument("Неподдерживаемый хэш: " + hashType);
    }
    std::cout << digest << std::endl;
    return 0;

  } catch (const po::error &e) {
    std::cerr << "Ошибка Boost: " << e.what() << std::endl;
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "Ошибка: " << e.what() << std::endl;
    return 1;
  }
}
