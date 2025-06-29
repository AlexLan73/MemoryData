// memory_base.cpp

#include "pch.h"
#include "MemoryData/memory_base.h"

//#include <sstream>    // Для std::stringstream
//#include <algorithm>  // Для std::min
//#include <vector>   // Для std::vector

// Вспомогательная функция для конвертации std::string (UTF-8) в std::wstring (UTF-16) для WinAPI
// Теперь это метод класса
std::wstring MemoryBase::to_wstring(const std::string& str) {
  if (str.empty()) return std::wstring();
  int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);
  std::wstring wstr(size_needed, 0);
  MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), &wstr[0], size_needed);
  return wstr;
}

// --- КОНСТРУКТОР ---
MemoryBase::MemoryBase(const std::string& nameMemory, TypeBlockMemory typeBlockMemory, size_t dataSegmentSize,
  CallbackDataMetaData callBack)
  : _dataSegmentSize(dataSegmentSize), // Исправлен тип
  _callBack(callBack) {

  std::string nameMemoryControl = nameMemory + "Control";
  std::wstring wNameControl = to_wstring(nameMemoryControl);
  std::wstring wNameData = to_wstring(nameMemory);
  std::wstring wEventName = L"Global\\Event" + to_wstring(nameMemory); // Используем to_wstring как метод

  hControlMapFile = CreateFileMappingW(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, static_cast<DWORD>(CONTROL_SIZE), wNameControl.c_str());
  if (hControlMapFile == nullptr) throw std::runtime_error("Failed to create control file mapping.");

  hDataMapFile = CreateFileMappingW(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, static_cast<DWORD>(dataSegmentSize), wNameData.c_str());
  if (hDataMapFile == nullptr) {
    CloseHandle(hControlMapFile);
    throw std::runtime_error("Failed to create data file mapping.");
  }

  hEvent = CreateEventW(nullptr, FALSE, FALSE, wEventName.c_str());
  if (hEvent == nullptr) {
    CloseHandle(hControlMapFile);
    CloseHandle(hDataMapFile);
    throw std::runtime_error("Failed to create event.");
  }

  if (typeBlockMemory == TypeBlockMemory::Read && _callBack) {
    _running = true;
    _eventThread = std::thread(&MemoryBase::event_loop, this);
  }
}

// --- ДЕСТРУКТОР ---
MemoryBase::~MemoryBase() {
  if (_running.load()) {
    _running = false;
    if (hEvent) SetEvent(hEvent); // Убеждаемся, что хэндл валиден
    if (_eventThread.joinable()) {
      _eventThread.join();
    }
  }
  if (hEvent) CloseHandle(hEvent);
  if (hControlMapFile) CloseHandle(hControlMapFile);
  if (hDataMapFile) CloseHandle(hDataMapFile);
}

// --- ПУБЛИЧНЫЕ МЕТОДЫ ---
void MemoryBase::WriteData(const std::vector<uint8_t>& data, const MetadataMap& metadata) {
  if (data.size() > _dataSegmentSize) {
    throw std::runtime_error("Размер данных превышает размер выделенного сегмента памяти.");
  }

  LPVOID pDataBuf = MapViewOfFile(hDataMapFile, FILE_MAP_ALL_ACCESS, 0, 0, _dataSegmentSize);
  if (pDataBuf == nullptr) {
    throw std::runtime_error("Не удалось получить View Of File для записи данных.");
  }

  ZeroMemory(pDataBuf, _dataSegmentSize); // Очистка памяти
  CopyMemory(pDataBuf, data.data(), data.size()); // Копирование данных
  UnmapViewOfFile(pDataBuf);

  SetCommandControl(metadata); // Запись метаданных и подача сигнала
}

MetadataMap MemoryBase::GetCommandControl() {
  LPVOID pBuf = MapViewOfFile(hControlMapFile, FILE_MAP_READ, 0, 0, CONTROL_SIZE);
  if (pBuf == nullptr) return {};
  std::string controlStr(static_cast<char*>(pBuf)); // Считываем весь контрольный буфер до нуль-терминатора
  UnmapViewOfFile(pBuf);
  return parse_control_string(controlStr.c_str());
}

void MemoryBase::SetCommandControl(const MetadataMap& metadata) {
  std::string controlStr = format_control_string(metadata);
  // Проверяем, чтобы строка метаданных не превышала размер контрольного буфера
  if (controlStr.length() + 1 > CONTROL_SIZE) { // +1 для нуль-терминатора
    throw std::runtime_error("Метаданные превышают размер контрольного буфера.");
  }

  LPVOID pBuf = MapViewOfFile(hControlMapFile, FILE_MAP_ALL_ACCESS, 0, 0, CONTROL_SIZE);
  if (pBuf == nullptr) {
    throw std::runtime_error("Не удалось получить View Of File для записи метаданных.");
  }

  ZeroMemory(pBuf, CONTROL_SIZE); // Очищаем весь буфер
  CopyMemory(pBuf, controlStr.c_str(), controlStr.length()); // Копируем данные
  // Не нужно явно ставить нуль-терминатор, ZeroMemory уже все обнулила за пределами controlStr.length()
  UnmapViewOfFile(pBuf);
  ::SetEvent(hEvent); // Сигнализируем о новых данных
}

// Добавлен из старого memory_base.cpp
void MemoryBase::ClearCommandControl() {
  std::cout << "[MemoryBase] Очистка контрольной памяти..." << std::endl;
  LPVOID pBuf = MapViewOfFile(hControlMapFile, FILE_MAP_ALL_ACCESS, 0, 0, CONTROL_SIZE);
  if (pBuf == nullptr) {
    throw std::runtime_error("Не удалось получить View Of File для очистки.");
  }
  ZeroMemory(pBuf, CONTROL_SIZE);
  UnmapViewOfFile(pBuf);
  std::cout << "[MemoryBase] Память очищена. Подаю сигнал." << std::endl;
  ::SetEvent(hEvent);
}

// --- ПРИВАТНЫЕ МЕТОДЫ ---
void MemoryBase::event_loop() {
  while (_running.load()) {
    // Ожидаем событие с таймаутом в 1 секунду
    if (WaitForSingleObject(hEvent, 1000) == WAIT_OBJECT_0) {
      // Двойная проверка, если был получен сигнал о завершении работы
      if (!_running.load()) break;

      MetadataMap metadata = GetCommandControl();

      // Ситуация 1: Событие пришло, но метаданных нет (или они пустые).
      // Это может быть просто сигнал пробуждения или очистки.
      // Мы все равно должны уведомить подписчика, передав пустые данные.
      if (metadata.empty()) {
        if (_callBack) {
          _callBack({ {}, metadata });
        }
        continue; // Переходим к следующей итерации цикла
      }

      // Ситуация 2: Метаданные есть, нужно прочитать основной блок данных.
      auto it = metadata.find("size");
      size_t dataSize = (it != metadata.end()) ? std::stoul(it->second) : 0;

      std::vector<uint8_t> data;
      if (dataSize > 0) {
        // Выделяем память в векторе ПЕРЕД чтением для эффективности
        data.resize(dataSize);

        LPVOID pDataBuf = MapViewOfFile(hDataMapFile, FILE_MAP_READ, 0, 0, dataSize);
        if (pDataBuf != nullptr) {
          // Копируем данные напрямую в память вектора
          memcpy(data.data(), pDataBuf, dataSize);
          UnmapViewOfFile(pDataBuf);
        }
        else {
          // Если не удалось прочитать, очищаем вектор на всякий случай
          data.clear();
          std::cerr << "Ошибка: не удалось получить MapViewOfFile для данных размером " << dataSize << std::endl;
        }
      }

      // Вызываем колбэк с полученными данными (даже если они пустые из-за ошибки)
      if (_callBack) {
        _callBack({ data, metadata });
      }
    }
  }
}

MetadataMap MemoryBase::parse_control_string(const char* control_str) {
  MetadataMap metadata;
  if (control_str == nullptr) return metadata;
  std::string str(control_str);
  std::stringstream ss(str);
  std::string segment;
  while (std::getline(ss, segment, ';')) {
    if (segment.empty()) continue;
    std::string::size_type pos = segment.find('=');
    if (pos != std::string::npos) {
      metadata[segment.substr(0, pos)] = segment.substr(pos + 1);
    }
  }
  return metadata;
}

std::string MemoryBase::format_control_string(const MetadataMap& metadata) {
  std::stringstream ss;
  for (const auto& pair : metadata) { // Исправлено: использован ranged-based for loop
    ss << pair.first << "=" << pair.second << ";";
  }
  return ss.str();
}

