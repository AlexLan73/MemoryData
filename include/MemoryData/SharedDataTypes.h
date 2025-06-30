#pragma once
// SharedDataTypes.h


#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <functional> // Для CallbackDataMetaData
#include "IMemoryConfigChannel.h"
#include <msgpack.hpp>

  // --- Базовые типы для IPC ---

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
//enum class LoggerSendEnumMemory { Error = -1, Info = 0, Warning = 1 };
MSGPACK_ADD_ENUM(LoggerSendEnumMemory);

struct Logger :public ILogger {
  MSGPACK_DEFINE_ARRAY(Id, Module, Log, Code); // Исправлено: убран лишний символ
};

//struct Logger {
//  uint32_t Id;
//  std::string Module;
//  std::string Log;
//  LoggerSendEnumMemory Code;
//  MSGPACK_DEFINE_ARRAY(Id, Module, Log, Code); // Исправлено: убран лишний символ
//};

struct CudaVector:public ICudaVector{
  MSGPACK_DEFINE_ARRAY(Id, Values);
};

struct CudaValue:public ICudaValue {
  MSGPACK_DEFINE_ARRAY(Id, Value);
};

//struct CudaValue {
//  uint32_t Id;
//  double Value;
//  MSGPACK_DEFINE_ARRAY(Id, Value);
//};

struct CudaDateTimeVariable : public ICudaDateTimeVariable {
  MSGPACK_DEFINE_ARRAY(Id, DateTime, Variable);
};

struct CudaMatrix :public ICudaMatrix {
  MSGPACK_DEFINE_ARRAY(Id, I, J, Values); // Исправлено: Id был дважды
};

struct RecResult :public IRecResult{
  MSGPACK_DEFINE_ARRAY(Id, NFft, MChannel, TimeFft, TimeLoadData, TimeWaiteData);
};

struct CudaDtRecord {
  int32_t Id;
  std::string DateTime;
  std::vector<RecResult> DtRecord;
  MSGPACK_DEFINE_ARRAY(Id, DateTime, DtRecord);
};
