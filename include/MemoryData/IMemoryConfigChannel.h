#pragma once
#pragma once

enum class TypeBlockMemory { Read, Write };
enum class ServerClient { Server, Client };

using MetadataMap = std::map<std::string, std::string>;

struct RecDataMetaData {
  std::vector<uint8_t> Bytes;
  MetadataMap MetaData;
};

using CallbackDataMetaData = std::function<void(const RecDataMetaData&)>;

// --- Структуры данных ---
enum class LoggerSendEnumMemory { Error = -1, Info = 0, Warning = 1 };

struct ILogger {
  uint32_t Id;
  std::string Module;
  std::string Log;
  LoggerSendEnumMemory Code;
};

struct ICudaVector {
  uint32_t Id;
  std::vector<double> Values;
};

struct ICudaValue {
  uint32_t Id;
  double Value;
};


struct ICudaDateTimeVariable {
  uint32_t Id;
  std::string DateTime;
  float Variable;
};

struct ICudaMatrix {
  uint32_t Id;
  uint32_t I;
  uint32_t J;
  std::vector<double> Values; // Все элементы матрицы по порядку
};

struct IRecResult {
  uint32_t Id;
  uint32_t NFft;
  uint32_t MChannel;
  double TimeFft;
  double TimeLoadData;
  double TimeWaiteData;
};

