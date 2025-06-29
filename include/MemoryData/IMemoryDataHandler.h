#pragma once
// IMemoryDataHandler.h

#include "SharedDataTypes.h" 
#include <iostream>    // Для std::cout
#include <vector>      // Для std::vector

// IMemoryDataHandler - это "контракт" или интерфейс.
// Любой класс, который хочет обрабатывать данные, передаваемые через MemoryData,
// должен унаследовать этот интерфейс и переопределить нужные методы.
class IMemoryDataHandler {
public:
  // Виртуальный деструктор ОБЯЗАТЕЛЕН для базовых классов с виртуальными функциями.
  virtual ~IMemoryDataHandler() = default;

  // --- Виртуальные функции-заглушки (stubs) ---
  // Они уже имеют реализацию по умолчанию, поэтому их не обязательно переопределять.

  virtual void OnAckReceived() {
    std::cout << "[IMemoryDataHandler] Получено подтверждение (ACK), но обработчик не переопределен.\n";
  }

  virtual void OnLoggerData(const std::vector<Logger>& data) {
    std::cout << "[IMemoryDataHandler] Получены данные Logger, но обработчик не переопределен. Количество: " << data.size() << "\n";
  }

  virtual void OnCudaVectorData(const std::vector<CudaVector>& data) {
    std::cout << "[IMemoryDataHandler] Получены данные CudaVector, но обработчик не переопределен. Количество: " << data.size() << "\n";
  }

  virtual void OnCudaValueData(const std::vector<CudaValue>& data) {
    std::cout << "[IMemoryDataHandler] Получены данные CudaValue, но обработчик не переопределен. Количество: " << data.size() << "\n";
  }

  virtual void OnCudaDateTimeVariableData(const std::vector<CudaDateTimeVariable>& data) {
    std::cout << "[IMemoryDataHandler] Получены данные CudaDateTimeVariable, но обработчик не переопределен. Количество: " << data.size() << "\n";
  }

  virtual void OnCudaMatrixData(const std::vector<CudaMatrix>& data) {
    std::cout << "[IMemoryDataHandler] Получены данные CudaMatrix, но обработчик не переопределен. Количество: " << data.size() << "\n";
  }

  virtual void OnRecResultData(const std::vector<RecResult>& data) {
    std::cout << "[IMemoryDataHandler] Получены данные RecResult, но обработчик не переопределен. Количество: " << data.size() << "\n";
  }

  virtual void OnCudaDtRecordData(const std::vector<CudaDtRecord>& data) {
    std::cout << "[IMemoryDataHandler] Получены данные CudaDtRecord, но обработчик не переопределен. Количество: " << data.size() << "\n";
  }
  // Добавьте другие виртуальные методы для каждого типа данных, если они нужны в интерфейсе.


};
