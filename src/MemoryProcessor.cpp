#include "pch.h"
// "MemoryProcessor.cpp"
#include "MemoryData/MemoryProcessor.h"
#include <iostream>      // Для std::cerr
#include <stdexcept>     // Для std::invalid_argument, std::runtime_error

MemoryProcessor::MemoryProcessor(const std::string& name, ServerClient role, IMemoryDataHandler* handler)
  : _handler(handler) {
  if (!_handler) {
    throw std::invalid_argument("Handler (IMemoryDataHandler) cannot be nullptr.");
  }
  // Передаем лямбду, которая вызывает наш внутренний OnRawDataReceived
  _memoryNome = std::make_unique<MemoryNome>(name, role,
    [this](const RecDataMetaData& meta) {
      this->OnRawDataReceived(meta);
    });
}

void MemoryProcessor::OnRawDataReceived(const RecDataMetaData& dMetaData) {
  // 1. Проверяем, является ли это подтверждением (ACK)
  auto cmd_it = dMetaData.MetaData.find("command");
  if (cmd_it != dMetaData.MetaData.end() && cmd_it->second == "ok") {
    if (_handler) _handler->OnAckReceived();
    return;
  }

  // 2. Если это не ACK, то это данные. Ищем ID типа.
  auto type_it = dMetaData.MetaData.find("type");
  if (type_it == dMetaData.MetaData.end()) {
    std::cerr << "[MemoryProcessor] Received data without 'type' ID in metadata.\n";
    return;
  }

  try {
    uint32_t typeId = std::stoul(type_it->second);
    msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(dMetaData.Bytes.data()), dMetaData.Bytes.size());

    if (_handler) { // Проверяем, что есть обработчик
      // Используем switch для вызова соответствующего метода обработчика
      switch (typeId) {
      case eLogger:
        _handler->OnLoggerData(oh.get().as<std::vector<Logger>>());
        break;
      case eCudaVector:
        _handler->OnCudaVectorData(oh.get().as<std::vector<CudaVector>>());
        break;
      case eCudaValue:
        _handler->OnCudaValueData(oh.get().as<std::vector<CudaValue>>());
        break;
      case eCudaDateTimeVariable:
        _handler->OnCudaDateTimeVariableData(oh.get().as<std::vector<CudaDateTimeVariable>>());
        break;
      case eCudaMatrix:
        _handler->OnCudaMatrixData(oh.get().as<std::vector<CudaMatrix>>());
        break;
      case eRecResult:
        _handler->OnRecResultData(oh.get().as<std::vector<RecResult>>());
        break;
      case eCudaDtRecord:
        _handler->OnCudaDtRecordData(oh.get().as<std::vector<CudaDtRecord>>());
        break;
      default:
        std::cerr << "[MemoryProcessor] Received data with unknown type ID: " << typeId << "\n";
        break;
      }
    }
  }
  catch (const std::exception& e) {
    std::cerr << "[MemoryProcessor] Deserialization error: " << e.what() << '\n';
  }

  // ЭТО КЛЮЧЕВАЯ ДОРАБОТКА ДЛЯ СЕРВЕРА
  if (!dMetaData.MetaData.empty()) { // Очищаем только если были метаданные
    std::cout << "[MemoryProcessor] Данные обработаны, очищаю контрольный блок чтения...\n";
    ClearReadChannel();
  }
}

MetadataMap MemoryProcessor::CheckWriteChannel() {
  if (!_memoryNome) return {};
  return _memoryNome->CheckWriteChannelControl();
}

void MemoryProcessor::ClearReadChannel() {
  if (!_memoryNome) return;
  _memoryNome->ClearReadChannelControl();
}