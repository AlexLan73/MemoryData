#pragma once
// memory_base.h

#include "SharedDataTypes.h" // Для CallbackDataMetaData, MetadataMap, RecDataMetaData
#include <string>
#include <vector>
#include <windows.h> // Для WinAPI
#include <thread>      // Для std::thread
#include <atomic>      // Для std::atomic
#include <iostream>    // Для std::cout, std::cerr
#include <stdexcept>   // Для std::runtime_error

// Важно: Отменить макрос unpack из Windows.h, если он определен
#ifdef unpack
#undef unpack
#endif

class MemoryBase {
public:
  MemoryBase(const std::string& nameMemory, TypeBlockMemory typeBlockMemory,
    size_t dataSegmentSize, CallbackDataMetaData callBack);
  ~MemoryBase();

  MemoryBase(const MemoryBase&) = delete;
  MemoryBase& operator=(const MemoryBase&) = delete;

  void WriteData(const std::vector<uint8_t>& data, const MetadataMap& metadata);
  MetadataMap GetCommandControl();
  void SetCommandControl(const MetadataMap& metadata);
  // ClearCommandControl был в memory_base.cpp, но не в .h, выносим сюда
  void ClearCommandControl();

private:
  void event_loop();
  std::wstring to_wstring(const std::string& str); // Добавлен сюда, чтобы быть членом класса
  MetadataMap parse_control_string(const char* str);
  std::string format_control_string(const MetadataMap& metadata);

  HANDLE hControlMapFile = nullptr;
  HANDLE hDataMapFile = nullptr;
  HANDLE hEvent = nullptr;

  std::string _nameMemoryData;
  std::string _nameMemoryDataControl;

  CallbackDataMetaData _callBack;
  std::thread _eventThread;
  std::atomic<bool> _running{ false };

  const size_t CONTROL_SIZE = 8 * 1024;
  size_t _dataSegmentSize; // Исправлен тип на size_t
};

