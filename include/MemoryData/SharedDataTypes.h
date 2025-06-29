#pragma once
// SharedDataTypes.h


#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <functional> // Для CallbackDataMetaData
#include <msgpack.hpp>

  // --- Базовые типы для IPC ---
  enum class TypeBlockMemory { Read, Write };
enum class ServerClient { Server, Client };

using MetadataMap = std::map<std::string, std::string>;

struct RecDataMetaData {
  std::vector<uint8_t> Bytes;
  MetadataMap MetaData;
};

using CallbackDataMetaData = std::function<void(const RecDataMetaData&)>;

// --- Идентификаторы типов данных ---
enum DataTypeIds : uint32_t {
  eLogger = 0,
  eCudaDateTimeVariable = 1,
  eCudaVector = 2,
  eCudaMatrix = 3,
  eRecResult = 4,
  eCudaDtRecord = 5,
  eCudaValue = 6
};

// --- Структуры данных ---
enum class LoggerSendEnumMemory { Error = -1, Info = 0, Warning = 1 };
MSGPACK_ADD_ENUM(LoggerSendEnumMemory);

struct Logger {
  uint32_t Id;
  std::string Module;
  std::string Log;
  LoggerSendEnumMemory Code;
  MSGPACK_DEFINE_ARRAY(Id, Module, Log, Code); // Исправлено: убран лишний символ
};

struct CudaVector {
  uint32_t Id;
  std::vector<double> Values;
  MSGPACK_DEFINE_ARRAY(Id, Values);
};

struct CudaValue {
  uint32_t Id;
  double Value;
  MSGPACK_DEFINE_ARRAY(Id, Value);
};

struct CudaDateTimeVariable {
  uint32_t Id;
  std::string DateTime;
  float Variable;
  MSGPACK_DEFINE_ARRAY(Id, DateTime, Variable);
};

struct CudaMatrix {
  uint32_t Id;
  uint32_t I;
  uint32_t J;
  std::vector<double> Values; // Все элементы матрицы по порядку
  MSGPACK_DEFINE_ARRAY(Id, I, J, Values); // Исправлено: Id был дважды
};

struct RecResult {
  uint32_t Id;
  uint32_t NFft;
  uint32_t MChannel;
  double TimeFft;
  double TimeLoadData;
  double TimeWaiteData;
  MSGPACK_DEFINE_ARRAY(Id, NFft, MChannel, TimeFft, TimeLoadData, TimeWaiteData);
};

struct CudaDtRecord {
  int32_t Id;
  std::string DateTime;
  std::vector<RecResult> DtRecord;
  MSGPACK_DEFINE_ARRAY(Id, DateTime, DtRecord);
};
