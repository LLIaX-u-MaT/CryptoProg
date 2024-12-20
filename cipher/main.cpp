#include "AES.h"

namespace po = boost::program_options;

void checkFileExists(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.good()) {
        throw std::runtime_error("Файл не найден: " + filename);
    }
}

void checkUniqueFileNames(const std::initializer_list<std::string>& fileNames) {
    std::set<std::string> uniqueNames(fileNames);
    if (uniqueNames.size() < fileNames.size()) {
        throw std::runtime_error("Ошибка: Имена файлов не должны совпадать!");
    }
}

int main() {
    AES aes;
    std::string action;

    while (true) {
        std::cout << "Вы хотите зашифровать или расшифровать файл? (1 - зашифрование, 2 - расшифрование, 0 - выход): ";
        std::cin >> action;

        try {
            if (action == "1") {
                std::string orig_file, key_file, iv_file, encr_file, password;

                std::cout << "Введите имя файла для шифрования: ";
                std::cin >> orig_file;
                checkFileExists(orig_file);

                std::cout << "Введите имя файла сохранения ключа: ";
                std::cin >> key_file;

                std::cout << "Введите имя файла сохранения вектора инициализации: ";
                std::cin >> iv_file;

                std::cout << "Введите имя сохранения зашифрованного файла: ";
                std::cin >> encr_file;

                checkUniqueFileNames({orig_file, key_file, iv_file, encr_file});

                aes.encrypt(password, orig_file, key_file, iv_file, encr_file);
                std::cout << "Файл успешно зашифрован!" << std::endl;

            } else if (action == "2") {
                std::string key_file, iv_file, decr_file, output_file;

                std::cout << "Введите имя файла с ключом: ";
                std::cin >> key_file;
                checkFileExists(key_file);

                std::cout << "Введите имя файла с вектором инициализации: ";
                std::cin >> iv_file;
                checkFileExists(iv_file);

                std::cout << "Введите имя зашифрованного файла: ";
                std::cin >> decr_file;
                checkFileExists(decr_file);

                std::cout << "Введите имя выходного файла сохранения расшифрованного файла: ";
                std::cin >> output_file;

                checkUniqueFileNames({key_file, iv_file, decr_file, output_file});

                aes.decrypt(key_file, iv_file, decr_file, output_file);
                std::cout << "Файл успешно расшифрован!" << std::endl;

            } else if (action == "0") {
                std::cout << "Произведён выход из программы." << std::endl;
                break;

            } else {
                std::cerr << "Ошибка: Неизвестное действие. Доступные опции: '1', '2', '0'." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }

    return 0;
}
