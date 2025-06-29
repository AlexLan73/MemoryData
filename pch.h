// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#pragma once

// Добавьте сюда заголовочные файлы для предварительной компиляции
#include "framework.h"

#include <windows.h> // Для WinAPI
#include <string>
#include <vector>
#include <thread>      // Для std::thread
#include <atomic>      // Для std::atomic
#include <iostream>    // Для std::cout, std::cerr
#include <stdexcept>   // Для std::runtime_error
#include <cstdint>
#include <map>
#include <functional> // Для CallbackDataMetaData
#include <sstream>    // Для std::stringstream
