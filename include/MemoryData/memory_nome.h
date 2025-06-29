#pragma once
//  memory_nome.h

#include "SharedDataTypes.h" // Подключаем, чтобы видеть все типы данных
#include "memory_base.h"

#include <string>
#include <memory>       // Для std::unique_ptr
#include <functional>   // Для CallbackDataMetaData
#include <iostream>     // Для std::cout, std::cerr (хотя эти чаще в .cpp)

//class MemoryBase; // Forward declaration для MemoryBase

class MemoryNome {
public:
  MemoryNome(const std::string& nameMemory, ServerClient role, CallbackDataMetaData callback);
  ~MemoryNome() = default;

  MemoryNome(const MemoryNome&) = delete;
  MemoryNome& operator=(const MemoryNome&) = delete;

  void WriteDataToMemory(const std::vector<uint8_t>& bytes, const MetadataMap& map);

  // Проверяет метаданные на канале, в который мы ПИШЕМ.
      // Позволяет узнать, прочитал ли получатель наши данные.
  MetadataMap CheckWriteChannelControl();

  // Очищает метаданные на канале, из которого мы ЧИТАЕМ.
  // Это нужно делать после того, как мы обработали входящие данные.
  void ClearReadChannelControl();
private:
  std::unique_ptr<MemoryBase> _memoryRead; // Используем unique_ptr
  std::unique_ptr<MemoryBase> _memoryWrite; // Используем unique_ptr
};




/*
//class MemoryNome {
//public:
//  MemoryNome(const std::string& nameMemory, ServerClient role, CallbackDataMetaData callback);
//  ~MemoryNome() = default;
//
//  MemoryNome(const MemoryNome&) = delete;
//  MemoryNome& operator=(const MemoryNome&) = delete;
//
//  void WriteDataToMemory(const std::vector<uint8_t>& bytes, const MetadataMap& map);
//
//private:
//  std::shared_ptr<MemoryBase> _memoryRead;
//  std::shared_ptr<MemoryBase> _memoryWrite;
//};
//
*/

