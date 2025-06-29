//#pragma once
//// MemoryProcessor.h
//

#pragma once

#include "memory_nome.h"      // Подключаем MemoryNome
#include "IMemoryDataHandler.h" // Подключаем интерфейс
#include "SharedDataTypes.h"  // Подключаем SharedDataTypes
#include <memory>             // Для std::unique_ptr
#include <string>             // Для std::string
#include <sstream>            // Для std::stringstream
#include <vector>             // Для std::vector

// Этот класс инкапсулирует всю логику приема, десериализации и диспетчеризации.
class MemoryProcessor {
public:
  // Конструктор принимает указатель на реализацию интерфейса.
  MemoryProcessor(const std::string& name, ServerClient role, IMemoryDataHandler* handler);

  // Упрощенный метод для отправки данных (скрывает детали сериализации)
  template<typename T>
  void SendData(DataTypeIds typeId, const std::vector<T>& data);

  MetadataMap CheckWriteChannel();
  void ClearReadChannel();

private:
  // Внутренний метод, который вызывается MemoryNome при получении сырых данных
  void OnRawDataReceived(const RecDataMetaData& dMetaData);

  std::unique_ptr<MemoryNome> _memoryNome;
  IMemoryDataHandler* _handler; // Указатель на внешний обработчик
};

// Реализацию шаблонного метода лучше оставить в заголовочном файле
template<typename T>
void MemoryProcessor::SendData(DataTypeIds typeId, const std::vector<T>& data) {
  if (data.empty() || !_memoryNome) return;

  std::stringstream buffer;
  msgpack::pack(buffer, data); // Сериализация
  std::string packed_str = buffer.str();
  std::vector<uint8_t> bytes(packed_str.begin(), packed_str.end());

  MetadataMap metadata;
  metadata["type"] = std::to_string(typeId);
  metadata["size"] = std::to_string(bytes.size());

  _memoryNome->WriteDataToMemory(bytes, metadata); // Запись в MMF
}





//#pragma once
//
//#include "memory_nome.h"        // Подключаем MemoryNome
//#include "IMemoryDataHandler.h" // Подключаем интерфейс
//#include "SharedDataTypes.h"    // Подключаем SharedDataTypes
////#include <memory>               // Для std::unique_ptr
////#include <string>               // Для std::string
//#include <sstream>              // Для std::stringstream
//#include <vector>               // Для std::vector
//
//// Этот класс инкапсулирует всю логику приема, десериализации и диспетчеризации.
//class MemoryProcessor {
//public:
//  // Конструктор принимает указатель на реализацию интерфейса.
//  MemoryProcessor(const std::string& name, ServerClient role, IMemoryDataHandler* handler);
//
//  // Упрощенный метод для отправки данных (скрывает детали сериализации)
//  template<typename T>
//  void SendData(DataTypeIds typeId, const std::vector<T>& data);
//
//private:
//  void OnRawDataReceived(const RecDataMetaData& dMetaData);
//
//  std::unique_ptr<MemoryNome> _memoryNome;
//  IMemoryDataHandler* _handler; // Указатель на внешний обработчик
//};
//
//// Реализацию шаблонного метода лучше оставить в заголовочном файле
//template<typename T>
//void MemoryProcessor::SendData(DataTypeIds typeId, const std::vector<T>& data) {
//  if (data.empty() || !_memoryNome) return; // Убедиться, что _memoryNome валиден
//
//  std::stringstream buffer;
//  msgpack::pack(buffer, data);
//  std::string packed_str = buffer.str();
//  std::vector<uint8_t> bytes(packed_str.begin(), packed_str.end());
//
//  MetadataMap metadata;
//  metadata["type"] = std::to_string(typeId);
//  metadata["size"] = std::to_string(bytes.size());
//
//  _memoryNome->WriteDataToMemory(bytes, metadata);
//}
//
