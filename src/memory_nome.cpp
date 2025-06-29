#include "pch.h"
//  memory_nome.cpp

#include "MemoryData/memory_nome.h"
#include "MemoryData/memory_base.h" // Должен быть подключен для определения MemoryBase
#include <iostream> // Для std::cout, std::cerr

constexpr size_t DEFAULT_DATA_SIZE = 64 * 1024; // 64 KB

MemoryNome::MemoryNome(const std::string& nameMemory, ServerClient role, CallbackDataMetaData callback) {
  std::string readChannelName;
  std::string writeChannelName;

  if (role == ServerClient::Server) {
    readChannelName = nameMemory + "Read";
    writeChannelName = nameMemory + "Write";
    std::cout << "[MemoryNome Server] Reading from: " << readChannelName << ", Writing to: " << writeChannelName << '\n';
  }
  else { // Client
    readChannelName = nameMemory + "Write";
    writeChannelName = nameMemory + "Read";
    std::cout << "[MemoryNome Client] Reading from: " << readChannelName << ", Writing to: " << writeChannelName << '\n';
  }

  // Использование make_unique
  _memoryRead = std::make_unique<MemoryBase>(readChannelName, TypeBlockMemory::Read, DEFAULT_DATA_SIZE, callback);
  _memoryWrite = std::make_unique<MemoryBase>(writeChannelName, TypeBlockMemory::Write, DEFAULT_DATA_SIZE, nullptr);
}

void MemoryNome::WriteDataToMemory(const std::vector<uint8_t>& bytes, const MetadataMap& map) {
  try {
    _memoryWrite->WriteData(bytes, map);
  }
  catch (const std::exception& e) {
    std::cerr << "Error writing data in MemoryNome: " << e.what() << std::endl;
  }
}
MetadataMap MemoryNome::CheckWriteChannelControl() {
  if (!_memoryWrite) return {};
  // Просто вызываем GetCommandControl у нашего канала записи
  return _memoryWrite->GetCommandControl();
}

void MemoryNome::ClearReadChannelControl() {
  if (!_memoryRead) return;
  // Вызываем ClearCommandControl у нашего канала чтения
  _memoryRead->ClearCommandControl();
}